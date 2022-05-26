#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <QCoreApplication>

#include "PublisherApp.h"

#include "CommandLineParser.h"

#include <string>
#include <iostream>


int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    PublisherApp tester(a.arguments());
    QObject::connect(&tester, &PublisherApp::exit, &a, &QCoreApplication::quit, Qt::QueuedConnection);

    tester.init();

    return a.exec();
}
