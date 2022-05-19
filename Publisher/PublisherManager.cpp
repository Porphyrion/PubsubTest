#include "PublisherManager.h"

PublisherManager::PublisherManager(int port, QObject *parent)
    : QObject{parent}
    , _publisher(std::make_shared<Publisher>(port))
{

}


void PublisherManager::appendChannel(ChannelData data)
{
   ChannelManager channel{data, _publisher, this};
   _channels.append(channel);
}

