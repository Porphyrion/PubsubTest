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
    using ChannelData = std::tuple<QString, int, int>;

public:
    explicit ChannelManager(ChannelData data, std::shared_ptr<Publisher> publisher,  QObject *parent = nullptr);
    ~ChannelManager() override = default;

    QString chanelName() const;

public slots:
    void handleMessage(const QByteArray message);

signals:

private:
    QList<ProducerPtr>            _producers;
    std::shared_ptr<Publisher>    _publisher;

    QString _channelName;
    int _messageSize;
    int _speed;

    std::atomic<unsigned int> _messagesCounter;

    void _createProducer(int messageSize);
};

