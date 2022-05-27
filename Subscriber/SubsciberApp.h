#pragma once

#include <QObject>

#include <memory>

#include "Subscriber.h"
#include "CommandLineParserSub.h"



class SubsciberApp : public QObject
{
    Q_OBJECT
public:
    SubsciberApp(QStringList args, QObject* parent = nullptr);

    bool init();

signals:
    void stop();

private:
    std::shared_ptr<Subscriber> _subscriber;
    CommandLineParserSub _parser;
};

