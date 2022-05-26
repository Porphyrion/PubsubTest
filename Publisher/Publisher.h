#pragma once


#include <QDebug>
#include <QObject>
#include <QString>
#include <QByteArray>

#include <zmq.hpp>
#include <zmq_addon.hpp>

struct PublisherPrivate;



class MonitorSignalingDevice : public QObject
{
    Q_OBJECT;
public:
    void startProduce(){
        emit connected();
    }

    void stopProduce(){
        emit disconnected();
    }

signals:
    void connected();
    void disconnected();
};


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
        qDebug() << "Client connected from " << addr;
        signal.startProduce();
    }   


    void on_event_disconnected(const zmq_event_t& event,
                               const char* addr) override
    {
        qDebug()<< "Client disconnected" << addr;
        signal.stopProduce();

    }

    void on_event_closed(const zmq_event_t& event,
                           const char* addr) override
    {
         qDebug()<< "Client disconnected" << addr;
    }

    void on_monitor_started() override
    {
        qDebug()<<"Monitor started";
    }


    MonitorSignalingDevice signal;
};


class Monitoring
{
public:
    void monitor(zmq::socket_t& soc){
        connectedMonitor.monitor(soc, "inproc://common", ZMQ_EVENT_ALL);
    }

    connect_monitor_t connectedMonitor;
};



class Publisher
    : public QObject
{
    Q_OBJECT
public:

    Publisher(uint port = 0, QObject* parent = nullptr);
    ~Publisher() override;

    uint    port() const;

public slots:
    void publish(QByteArray const& msg, QString channel = QString{});

signals:
    void startProduce();
    void stopProduce();

private:
    PublisherPrivate*   _d = nullptr;
};
