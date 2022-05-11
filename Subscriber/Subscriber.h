#pragma once

#include <QEvent>
#include <QObject>
#include <QString>
#include <QByteArray>


struct SubscriberPrivate;

class Subscriber
    : public QObject
{
    Q_OBJECT
public:

    Subscriber(QString host, uint port, QObject* parent = nullptr);

    void subscribe(QString channel);

signals:
    void received(QByteArray data, QString channel);

protected:

    void customEvent(QEvent* event) override;

private:

    SubscriberPrivate* _d = nullptr;
};

class ReceiveEvent
    : public QEvent
{
public:

    static constexpr int Type = QEvent::User + 100;

    ReceiveEvent(QByteArray data, QString channel)
        : QEvent(static_cast<QEvent::Type>(Type)), data(data), channel(channel) {}

    QByteArray data;
    QString    channel;
};