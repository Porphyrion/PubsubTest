#include "ChannelManager.h"
#include <QTimer>
#include <QDateTime>
#include <QCoreApplication>

#include <QDebug>

ChannelManager::ChannelManager(QString name , QList<ProducrData> data, std::shared_ptr<Publisher> publisher, QObject *parent) :
    QObject{parent},
    _publisher(publisher),
    _channelName(name),
    _messagesCounter(0)
{
    for(auto prodData : data)
        _createProducer(prodData.first, prodData.second);

    qInfo()<< "Created channel" << _channelName << "messages types amount: " << _producers.size();
}


void ChannelManager::handleMessage(const QByteArray message)
{
    if(_messagesCounter.load() == 0)
        qInfo()<<_channelName<<"started publishing at "<<QDateTime::currentDateTime().toMSecsSinceEpoch();


    auto fullMessage = (message + ":" + QString::number(_messagesCounter)).toUtf8();
    _publisher->publish(fullMessage, _channelName);
    ++_messagesCounter;

}


void ChannelManager::stop()
{
    qInfo()<<"Chanel"<<_channelName<<"sent" <<_messagesCounter;

    _messagesCounter = 0;

    // QTimer::singleShot(2000, [=](){QCoreApplication::exit(0);});

    emit stopProduce();

}


void ChannelManager::_createProducer(int messageSize, int speed)
{
    ProducerPtr p;
    p = std::make_shared<DataProducer>(messageSize, speed,  this);

    connect(p.get(), &DataProducer::sendMessage, this, &ChannelManager::handleMessage);
    connect(this, &ChannelManager::startProduce, p.get(), &DataProducer::produce);
    connect(this, &ChannelManager::stopProduce, p.get(), &DataProducer::stopProduce);

    _producers.append(p);
}
