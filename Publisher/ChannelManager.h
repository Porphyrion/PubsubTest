#pragma once

#include <QObject>

#include <memory>
#include <atomic>
#include <tuple>

#include "Publisher.h"
#include "DataProducer.h"

class ChannelManager : public QObject
{
    Q_OBJECT

    using ProducerPtr = std::shared_ptr<DataProducer>;
    //! Channel name, message size, speed
    using ProducrData = std::pair<int, int>;
    using PublisherPtr = std::shared_ptr<Publisher>;

public:
    explicit ChannelManager(QString name , QList<ProducrData> data, std::shared_ptr<Publisher> publisher,  QObject *parent = nullptr);
    ~ChannelManager() override = default;

    QString chanelName() const;

public slots:
    void handleMessage(const QByteArray message);
    void stop();

signals:
    void startProduce();
    void stopProduce();

private:
    QList<ProducerPtr> _producers;
    PublisherPtr       _publisher;

    int _messageSize;
    int _speed;

    QString _channelName;

    std::atomic<unsigned int> _messagesCounter;

    void _createProducer(int messageSize, int speed);
};

