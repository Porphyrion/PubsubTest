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

private:

    SubscriberPrivate* _d = nullptr;
};
