#include "publisherapp.h"

#include <QDebug>

PublisherApp::PublisherApp(int producerAmount, int producerAmountFast, int producerAmounBig, int producerAmountFB, QObject *parent)
    : QObject{parent}, _publisher(std::make_shared<Publisher>(666, this))
{
    for(auto i = 0; i < producerAmount; ++i)
        _createProducer(PublisherApp::casual);

    for(auto i = 0; i < producerAmountFast; ++i)
        _createProducer(PublisherApp::fast);

    for(auto i = 0; i < producerAmounBig; ++i)
        _createProducer(PublisherApp::big);

    for(auto i = 0; i < producerAmountFB; ++i)
        _createProducer(PublisherApp::fast_and_big);
}


void PublisherApp::messageHandle(QString name)
{
    _publisher->publish(name.toUtf8(), "test1");
}


void PublisherApp::_createProducer(PublisherApp::producer_type type)
{
    ProducerPtr p;
    switch(type){
    case casual:
        p = std::make_shared<DataProducer>(std::string(COMMOM_MESSAGE), COMMON_SPEED, this);
        break;
    case fast:
        p = std::make_shared<DataProducer>(std::string(COMMOM_MESSAGE), FAST_SPEED, this);
        break;
    case big:
        p = std::make_shared<DataProducer>(std::string(BIG_MESSAGE), COMMON_SPEED, this);
        break;
    case fast_and_big:
        p = std::make_shared<DataProducer>(std::string(BIG_MESSAGE), FAST_SPEED, this);
        break;
    }

    _appendProducer(p);

}


void PublisherApp::_appendProducer(ProducerPtr prod)
{
    connect(prod.get(), &DataProducer::sendMessage, this, &PublisherApp::messageHandle);
    _producers.append(prod);
}
