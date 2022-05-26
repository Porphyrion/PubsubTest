#include "PublisherManager.h"

#include <QDebug>

PublisherManager::PublisherManager(int port, QObject *parent)
    : QObject{parent}
    , _publisher(std::make_shared<Publisher>(port))
{
    qInfo()<<"Publisher start on port: " << port;

    connect(_publisher.get(), &Publisher::startProduce, this, &PublisherManager::startProduce);
    connect(_publisher.get(), &Publisher::stopProduce, this, &PublisherManager::stopProduce);
}



void PublisherManager::appendChannel(QString name, QList<ProducerData> data)
{
   auto channel = std::make_shared<ChannelManager>(name, data, _publisher, this);
   connect(this, &PublisherManager::startProduce, channel.get(), &ChannelManager::startProduce);
   connect(this, &PublisherManager::stopProduce, channel.get(), &ChannelManager::stop);
   _channels.append(channel);
}

