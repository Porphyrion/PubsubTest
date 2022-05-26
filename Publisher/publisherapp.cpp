#include "publisherapp.h"

#include <QDebug>
#include <QTimer>

PublisherApp::PublisherApp(QStringList args, QObject *parent)
    : QObject{parent}, _cmdParser(args), _manager(nullptr)
{

}


void PublisherApp::init()
{
    if(_cmdParser.parse())
    {
        auto data = _cmdParser.data();
        _processData(data);
    }
    else
    {
        QTimer::singleShot(1000, [=](){QCoreApplication::exit(0);});
        qInfo(_cmdParser.help().toUtf8());
    }

    return;
}



void PublisherApp::_processData(CommandLineParser::AppData data)
{
    _manager = std::make_shared<PublisherManager>(data.port, this);

    for(auto channel : data._chanelNames)
        _manager->appendChannel(channel, data._producers);
}


