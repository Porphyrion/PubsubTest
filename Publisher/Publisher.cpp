#include "Publisher.h"

#include <QUuid>
#include <QDebug>
#include <QMutex>
#include <QTimer>

#include <thread>


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
        monitorThread.join();
    }

    zmq::context_t ctx;
    zmq::socket_t  publisher;
    std::thread    proxyThread;
    std::thread    monitorThread;
    QString        inprocUuid;
    uint           port;
    QMutex         guard;
    Monitoring     mon;


    void proxyWorker() {

        try {
            using namespace std::placeholders;

            zmq::socket_t router(ctx, zmq::socket_type::xsub);
            zmq::socket_t dealer(ctx, zmq::socket_type::xpub);

            router.connect(inprocUuid.toUtf8().constData());

            dealer.bind(QString("tcp://*:%1").arg(port).toUtf8().constData());

            monitorThread = std::thread(std::bind(&Monitoring::monitor, &mon, std::ref(dealer)));
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
    connect(&_d->mon.connectedMonitor.signal, &MonitorSignalingDevice::connected, this, &Publisher::startProduce, Qt::QueuedConnection);
    connect(&_d->mon.connectedMonitor.signal, &MonitorSignalingDevice::disconnected, this, &Publisher::stopProduce, Qt::QueuedConnection);
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


