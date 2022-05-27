#include "SubsciberApp.h"

#include <QTimer>

SubsciberApp::SubsciberApp(QStringList args, QObject* parent) : _parser(args)
{

}


bool SubsciberApp::init()
{
    if(_parser.parse())
    {
        auto data = _parser.data();
        _subscriber = std::make_shared<Subscriber>(data.host, data.port);
        for(auto chanel : data.chanels)
        {
            _subscriber->subscribe(chanel);
            qInfo()<< "Client have subscribed on chanel: " << chanel;
        }

        QTimer::singleShot(data.timeout, this, &SubsciberApp::stop);

        connect(this, &SubsciberApp::stop, _subscriber.get(), &Subscriber::stopReceiving, Qt::QueuedConnection);
    }
    return true;
}
