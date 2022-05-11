#include "Publisher.h"

#include <QUuid>
#include <QDebug>
#include <QMutex>

#include <thread>

#include <zmq.hpp>
#include <zmq_addon.hpp>

struct PublisherPrivate
{
    PublisherPrivate(uint port)
        : ctx(2), publisher(ctx, zmq::socket_type::pub), port(port)
    {
        const auto uuid = QUuid::createUuid().toString();
        
        inprocUuid = QString("inproc://%1").arg(uuid);

        publisher.bind(inprocUuid.toUtf8().constData());

        proxyThread = std::thread(std::bind(&PublisherPrivate::proxyWorker, this));
    }

    ~PublisherPrivate() {
        proxyThread.join();
    }

    zmq::context_t ctx;
    zmq::socket_t  publisher;
    std::thread    proxyThread;
    QString        inprocUuid;
    uint           port;
    QMutex         guard;

    void proxyWorker() {

        try {
            zmq::socket_t router(ctx, zmq::socket_type::xsub);
            zmq::socket_t dealer(ctx, zmq::socket_type::xpub);

            router.connect(inprocUuid.toUtf8().constData());

            if (port)
                dealer.bind(QString("tcp://*:%1").arg(port).toUtf8().constData());
            else {
                dealer.bind("tcp://*:*");

                const auto endpoint =
                    dealer.get(zmq::sockopt::last_endpoint);

                auto sep = endpoint.find_last_of(':');
                auto sPort = endpoint.substr(sep + 1, endpoint.size() - sep - 1);

                port = std::atoi(sPort.c_str());
            }

            zmq::proxy(router, dealer);

        } catch (const std::exception& e) {

            qCritical() << "Publisher:" << e.what();
        }
        
    }

    void publish(QByteArray const& data, QString channel) 
    {
        QMutexLocker lock(&guard);

        if (!channel.isEmpty()) {
            const auto channelResult = publisher.send(zmq::buffer(channel.toUtf8().constData(), channel.size()), zmq::send_flags::sndmore);

            if (channelResult && channelResult.value() < 0) {
                qCritical() << "Publisher:" << "failed channel name sending";
            }

        }

        const auto msgResult = publisher.send(zmq::buffer(data.constData(), data.size()));

        if (msgResult && msgResult.value() < 0) {
            qCritical() << "Publisher:" << "failed message data sending";
        }
    }
};

Publisher::Publisher(uint port, QObject* parent /*= nullptr*/)
    : QObject(parent), _d(new PublisherPrivate(port))
{

}

Publisher::~Publisher()
{
    delete _d;
}

uint Publisher::port() const
{
    return _d->port;
}

void Publisher::publish(QByteArray const& msg, QString channel)
{
    _d->publish(msg, channel);
}
