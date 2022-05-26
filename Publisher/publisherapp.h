#pragma once

#include <QList>

#include <memory>

#include "CommandLineParser.h"
#include "PublisherManager.h"


class PublisherApp : public QObject
{
    Q_OBJECT

    using ProducerPtr = std::shared_ptr<DataProducer>;
    using PublisherManagerPtr = std::shared_ptr<PublisherManager>;

public:
    explicit PublisherApp(QStringList args, QObject *parent = nullptr);

    void init();

signals:
    void exit();

private:
    CommandLineParser _cmdParser;
    PublisherManagerPtr  _manager;

    void _processData(CommandLineParser::AppData data);
    void _createManager();
};


