#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <QCoreApplication>

#include "SubsciberApp.h"

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    SubsciberApp s(a.arguments());
    s.init();

    return a.exec();
}
