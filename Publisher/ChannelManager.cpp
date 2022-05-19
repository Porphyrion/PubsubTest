#include "ChannelManager.h"

ChannelManager::ChannelManager(ChannelData data, std::shared_ptr<Publisher> publisher, QObject *parent) :
    QObject{parent},
    _publisher(publisher),
    _channelName(std::get<0>(data)),
    _messageSize(std::get<1>(data)),
    _speed(std::get<2>(data)),
    _messagesCounter(0)
{
    auto producerCount = _speed/20;
    for(auto i = 0; i < producerCount; ++i)
        _createProducer(_messageSize);
}


void ChannelManager::handleMessage(const QByteArray message)
{
     _publisher->publish(message, _channelName);
     ++_messagesCounter;
}


void ChannelManager::_createProducer(int messageSize)
{
    ProducerPtr p;
    p = std::make_shared<DataProducer>(messageSize, 50,  this);

    connect(p.get(), &DataProducer::sendMessage, this, &ChannelManager::handleMessage);
    _producers.append(p);
}
