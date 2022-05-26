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
    using ProducerData = std::pair<int, int>;
    using ChannelManagerPtr = std::shared_ptr<ChannelManager>;

public:
    explicit PublisherManager(int port = 0, QObject *parent = nullptr);
    ~PublisherManager() override = default;

    void appendChannel(QString name, QList<ProducerData> data);

signals:
    void startProduce();
    void stopProduce();

private:
    std::shared_ptr<Publisher> _publisher;
    QList<ChannelManagerPtr>   _channels;
};

