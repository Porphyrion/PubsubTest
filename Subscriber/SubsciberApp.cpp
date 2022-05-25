#include "SubsciberApp.h"

SubsciberApp::SubsciberApp(QStringList args, QObject* parent) : _parser(args)
{

}


bool SubsciberApp::init()
{
    if(_parser.parse())
    {
        auto data = _parser.data();
        _subscriber = std::make_shared<Subscriber>(data.host, data.port);
        _subscriber->subscribe(data.chanel);
    }
    return true;
}
