#pragma once

#include <QObject>
#include <QMap>

#include <QCommandLineParser>
#include <QCommandLineOption>


class CommandLineParser : public QObject
{
    Q_OBJECT;
public:

    //! Channel name, message size, speed
    using ChannelData = std::tuple<QString, int, int>;

    struct AppData
    {
        QList<ChannelData> channels;
        bool severalPublishers = false;
        int port = 1488;
    };

    CommandLineParser(QStringList args);

     bool parse();
     AppData data();

     QString help();

signals:
    void error(QString text);

private:
    QCommandLineParser _parser;

    QCommandLineOption _channelSizeOption;
    QCommandLineOption _velocityOption;
    QCommandLineOption _mesageSizeOption;
    QCommandLineOption _channelNameOption;
    QCommandLineOption _portOption;
    QCommandLineOption _severalPublishersOption;

    AppData _data;
    bool _severalChannels = false;

    CommandLineParser::AppData _parse();

    QString _lastError;

    void _initParser();

    bool _parseChannelData();
    bool _parseChannelOption(int size);
    bool _parseChannelOption();

    bool _parseUnobligatoryOption();

    void _fillChannelsData(QStringList&& velocity, QStringList&& messageSize, QStringList&& names);
};

