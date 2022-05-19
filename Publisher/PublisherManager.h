#pragma once

#include <QObject>
#include <QList>


#include <memory>

#include "ChannelManager.h"
#include "Publisher.h"

class PublisherManager : public QObject
{
    Q_OBJECT

    //! Channel name, message size, speed
    using ChannelData = std::tuple<QString, int, int>;

public:
    explicit PublisherManager(int port, QObject *parent = nullptr);
    ~PublisherManager() override = default;

    void appendChannel(ChannelData data);

private:
    std::shared_ptr<Publisher> _publisher;
    QList<ChannelManager> _channels;
};

