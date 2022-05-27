#pragma once

#include <QCommandLineParser>
#include <QCommandLineOption>

class CommandLineParserSub
{
public:
    struct SubscriberData
    {
        QStringList chanels;
        QString host;
        int port;
        int timeout;
    };

    CommandLineParserSub(QStringList args);

    bool parse();

    SubscriberData data() {return _data;};

    QString help() {return _parser.helpText(); }

private:
    QCommandLineParser _parser;

    QCommandLineOption _portOption;
    QCommandLineOption _hostOption;
    QCommandLineOption _chanelOption;
    QCommandLineOption _timeOutOption;

    SubscriberData _data;

    void _initParser();
};

