#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <QCoreApplication>

#include "Subscriber.h"

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    Subscriber s("127.0.0.1", 666);

    qDebug() << "subscribe test1";
    s.subscribe("test1");

    return a.exec();
}
