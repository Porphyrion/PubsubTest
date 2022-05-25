#include "Subscriber.h"

#include <QUuid>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>

#include <thread>

#include <zmq.hpp>
#include <zmq_addon.hpp>

struct SubscriberPrivate
{
    static constexpr const char* MSG_TYPE = "type";
    static constexpr const char* CHANNEL_NAME = "channel";
    static constexpr const char* MSG_SUBSCRIBE = "subscribe";
    static constexpr const char* MSG_UNSUBSCRIBE = "unsubscribe";
    static constexpr const char* MSG_STOP = "stop";

    SubscriberPrivate(Subscriber* parent, QString host, uint port)
        : ctx(2), host(host), port(port), parent(parent)
    {
        controllerBind = "inproc://#controller";
        controller = zmq::socket_t(ctx, zmq::socket_type::push);
        controller.bind(controllerBind.toUtf8().constData());

        subscriberThread = std::thread(std::bind(&SubscriberPrivate::subscribeWorker, this));
    }

    ~SubscriberPrivate() {
        stop();
        subscriberThread.join();
    }


    zmq::context_t ctx;
    zmq::socket_t  controller;
    QString        controllerBind;
    std::thread    subscriberThread;
    QString        inprocUuid;
    QString        host;
    uint           port;
    QStringList    channels;
    Subscriber*    parent;

    void subscribeWorker() {

        zmq::socket_t controller = zmq::socket_t(ctx, zmq::socket_type::pull);
        zmq::socket_t subscriber = zmq::socket_t(ctx, zmq::socket_type::sub);

        controller.connect(controllerBind.toUtf8().constData());
        subscriber.connect(QString("tcp://%1:%2").arg(host).arg(port).toUtf8().constData());

        subscriber.set(zmq::sockopt::subscribe, "NoMessage");

        std::vector<zmq::pollitem_t> items;

        items.push_back(zmq::pollitem_t{ controller.handle(), 0, ZMQ_POLLIN, 0 });
        items.push_back(zmq::pollitem_t{ subscriber.handle(), 0, ZMQ_POLLIN, 0 });

        while (true) {

            zmq::poll(items);

            if (items[0].revents & ZMQ_POLLIN) {

                std::vector<zmq::message_t> recv_msgs;
                zmq::recv_result_t result =
                    zmq::recv_multipart(controller, std::back_inserter(recv_msgs));

                assert(result && "recv failed");
                assert(*result == 2);

                const auto msgType = recv_msgs[0].to_string();
                const auto channel = recv_msgs[1].to_string();

                if (msgType == MSG_STOP) {
                    break;
                } else if (msgType == MSG_SUBSCRIBE) {
                    subscriber.set(zmq::sockopt::subscribe, channel.c_str());
                } else if (msgType == MSG_UNSUBSCRIBE) {
                    subscriber.set(zmq::sockopt::unsubscribe, channel.c_str());
                }
            }

            if (items[1].revents & ZMQ_POLLIN) {

                std::vector<zmq::message_t> recv_msgs;
                zmq::recv_result_t result =
                    zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));

                assert(result && "recv failed");
                assert(*result == 2);

                QByteArray data(static_cast<const char*>(recv_msgs[1].data()), recv_msgs[1].size());
                QByteArray channel(static_cast<const char*>(recv_msgs[0].data()), recv_msgs[0].size());
            }
        }
    }

    void subscribe(QString channel) 
    {
        QByteArray channelData = channel.toUtf8();

        std::array<zmq::const_buffer, 2> msg = {
            zmq::buffer(MSG_SUBSCRIBE, strlen(MSG_SUBSCRIBE)),
            zmq::buffer(channelData.constData(), channel.size())
        };

        zmq::send_multipart(controller, msg);
    }

    void stop() {
        std::array<zmq::const_buffer, 2> msg = {
            zmq::buffer(MSG_STOP, strlen(MSG_STOP)),
            zmq::buffer(static_cast<const void*>(nullptr), 0)
        };

        zmq::send_multipart(controller, msg);
    }
};



Subscriber::Subscriber(QString host, uint port, QObject* parent)
    : QObject(parent), _d(new SubscriberPrivate(this, host, port))
{
    qInfo()<<"Subsciber listen to: "<<host<<":"<<port;
}


void Subscriber::subscribe(QString channel)
{
    _d->subscribe(channel);
}

