#pragma once

#include <QEvent>
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QDebug>

#include <zmq.hpp>
#include <zmq_addon.hpp>



class connect_monitor_t : public zmq::monitor_t {
public:
    void on_event_connected(const zmq_event_t& event,
                            const char* addr) override
    {
        qDebug() << "Got connection from " << addr;
    }

    void on_event_accepted(const zmq_event_t& event,
                           const char* addr) override
    {
        qDebug() << "Got accepting from " << addr;
    }


    void on_event_disconnected(const zmq_event_t& event,
                               const char* addr) override
    {
        qDebug()<< "Server disconnected";
    }


    void on_monitor_started() override
    {
        qDebug()<<"Monitor started";
    }
};


class Monitoring
{
public:
    void monitor(zmq::socket_t& soc){
        connectedMonitor.monitor(soc, "inproc://common", ZMQ_EVENT_ALL);
    }

private:
    connect_monitor_t connectedMonitor;
};







struct SubscriberPrivate;

class Subscriber
    : public QObject
{
    Q_OBJECT
public:

    Subscriber(QString host, uint port, QObject* parent = nullptr);

    void subscribe(QString channel);

public slots:
    void stopReceiving();

signals:
    void received(QByteArray data, QString channel);

private:
    SubscriberPrivate* _d = nullptr;

};
