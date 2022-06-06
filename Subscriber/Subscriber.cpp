#include "Subscriber.h"

#include <atomic>

#include <QUuid>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>
#include <map>
#include <algorithm>

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
        monitoringThread.join();
    }


    zmq::context_t  ctx;
    zmq::socket_t   controller;
    QString         controllerBind;
    std::thread     subscriberThread;
    std::thread     monitoringThread;
    QString         inprocUuid;
    QString         host;
    uint            port;
    QStringList     channels;
    Subscriber*     parent;

    std::map<QString, std::atomic_int> counter;
    std::map<QString, std::atomic_int> lastMessages;

    void subscribeWorker() {

        zmq::socket_t controller = zmq::socket_t(ctx, zmq::socket_type::pull);
        zmq::socket_t subscriber = zmq::socket_t(ctx, zmq::socket_type::sub);

        controller.connect(controllerBind.toUtf8().constData());
        subscriber.connect(QString("tcp://%1:%2").arg(host).arg(port).toUtf8().constData());

        subscriber.set(zmq::sockopt::subscribe, "NoMessage");

        std::vector<zmq::pollitem_t> items;

        items.push_back(zmq::pollitem_t{ controller.handle(), 0, ZMQ_POLLIN, 0 });
        items.push_back(zmq::pollitem_t{ subscriber.handle(), 0, ZMQ_POLLIN, 0 });

        auto monitor = new Monitoring;
        monitoringThread = std::thread(std::bind(&Monitoring::monitor, monitor, std::ref(subscriber)));

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
                    counter[QString::fromStdString(channel)] = 0;
                    lastMessages[QString::fromStdString(channel)] = 0;
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


                auto data = recv_msgs[1].to_string();
                auto messID = QString::fromStdString(data.substr(data.rfind(":")+1)).toInt();
                auto pubCounter = recv_msgs[1].to_string();
                QString channel = QString::fromStdString(recv_msgs[0].to_string());
                if(counter[channel])
                {
                    if((lastMessages[channel] + 1) != messID)
                         qWarning()<< "Sequence failed. Received "<< messID << "previous message id"<<lastMessages[channel];
                    lastMessages[channel] = messID;
                }
                else
                {
                    qInfo()<<channel<<"first message received:" <<messID<< "at" <<QDateTime::currentDateTime().toMSecsSinceEpoch();
                    lastMessages[channel]= messID;
                }

                ++counter[channel];
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

        std::for_each(counter.begin(), counter.end(), [](std::pair<const QString, std::atomic_int>& puk){
            qInfo()<<puk.first << "received"<<puk.second;
        });
        std::for_each(lastMessages.begin(), lastMessages.end(), [](std::pair<const QString, std::atomic_int>& puk){
            qInfo()<<puk.first <<"last received message:" <<puk.second;
        });
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


void Subscriber::stopReceiving()
{
    _d->stop();
}
