#pragma once

#include <QObject>
#include <QVariant>

#include "Publisher.h"
#include "DataProducer.h"

#include <memory>

#include <QList>

class PublisherApp : public QObject
{
    Q_OBJECT

    using ProducerPtr = std::shared_ptr<DataProducer>;

    const char * BIG_MESSAGE =  "MessageMessageMessageMessageMessageMessageMessageMessageMesMessageMessage"
                                "MessageMessageMessageMessageMessageMessageMessageMessageMesMessageMessage"
                                "MessageMessageMessageMessageMessageMessageMessageMessageMesMessageMessage"
                                "MessageMessageMessageMessageMessageMessageMessageMessageMesMessageMessage"
                                "MessageMessageMessageMessageMessageMessageMessageMessageMesMessageMessage"
                                "MessageMessageMessageMessageMessageMessageMessageMessageMesMessageMessage"
                                "MessageMessageMessageMessageMessageMessageMessageMessageMesMessageMessage"
                                "MessageMessageMessageMessageMessageMessageMessageMessageMesMessageMessage"
                                "MessageMessageMessageMessageMessageMessageMessageMessageMesMessageMessage"
                                "MessageMessageMessageMessageMessageMessageMessageMessageMesMessageMessage"
                                "MessageMessageMessageMessageMessageMessageMessageMessageMesMessageMessage"
                                "MessageMessageMessageMessageMessageMessageMessageMessageMesMessageMessage"
                                "MessageMessageMessageMessageMessageMessageMessageMessageMesMessageMessage"
                                "MessageMessageMessageMessageMessageMessageMessageMessageMesMessageMessage"
                                "sageMessageMessageMessageMessageMessageMessageMessageMessageMessageMessage"
                                "sageMessageMessageMessageMessageMessageMessageMessageMessageMessageMessage"
                                "sageMessageMessageMessageMessageMessageMessageMessageMessageMessageMessage";

    const char * COMMOM_MESSAGE = "Message";
    const int    COMMON_SPEED = 500;
    const int    FAST_SPEED = 50;

    enum producer_type
    {
        casual = 0,
        fast,
        big,
        fast_and_big
    };



public:
    explicit PublisherApp(int producerAmount = 0, int producerAmountFast = 0, int producerAmounBig = 0, int producerAmountFB = 0,  QObject *parent = nullptr);

private:
    QList<ProducerPtr>            _producers;
    std::shared_ptr<Publisher>    _publisher;

    void _createProducer(producer_type type);
    void _appendProducer(ProducerPtr prod);


private slots:
    void messageHandle(QString message);
};


