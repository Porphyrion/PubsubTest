#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <QCoreApplication>

#include "Publisher.h"
#include "Subscriber.h"

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    Publisher p;

    QThread::msleep(200);

    Subscriber s("127.0.0.1", p.port());

    QObject::connect(&s, &Subscriber::received, [](QByteArray data, QString channel) {
        qInfo() << "Channel:" << channel
            << "Data:" << QString::fromUtf8(data);
    });

    QThread::msleep(200);


    qDebug() << "subscribe test1";
    s.subscribe("test1");

    QThread::msleep(200);

    p.publish("1 msg", "test1");
    p.publish("2 msg", "test1");
    p.publish("3 msg", "test33");

    
    QThread::msleep(200);

    p.publish("21 msg", "test1");
    p.publish("22 msg", "test1");
    p.publish("23 msg", "test1");

    p.publish("31 msg", "test2");
    p.publish("32 msg", "test2");
    p.publish("33 msg", "test2");

    QThread::msleep(200);

    qDebug() << "subscribe test2";
    s.subscribe("test2");

    QThread::msleep(200);

    p.publish("41 msg", "test1");
    p.publish("42 msg", "test1");
    p.publish("43 msg", "test1");

    p.publish("51 msg", "test2");
    p.publish("52 msg", "test2");
    p.publish("53 msg", "test2");


    QTimer timer1;
    QObject::connect(&timer1, &QTimer::timeout, [&]() {
        const auto msg = QString("Msg at %1").arg(QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs));
        p.publish(msg.toUtf8(), "test1");
    });
    timer1.start(1000);

    QTimer timer2;
    QObject::connect(&timer2, &QTimer::timeout, [&]() {
        const auto msg = QString("Msg at %1").arg(QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs));
        p.publish(msg.toUtf8(), "test2");
    });
    timer2.start(1000);

    return a.exec();
}
