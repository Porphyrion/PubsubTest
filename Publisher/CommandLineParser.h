#pragma once

#include <QObject>

#include <QCommandLineParser>
#include <QCommandLineOption>


class CommandLineParser : public QObject
{
    Q_OBJECT;
public:

    //! Channel  message size, velocity
    using ProducerData = std::pair<int, int>;

    struct AppData
    {
        QList<ProducerData> _producers;
        QList<QString> _chanelNames;
        int port;
    };

    CommandLineParser(QStringList args, QObject * parent = nullptr);

     bool parse();
     AppData data();

     QString help();

private:
    QCommandLineParser _parser;

    QCommandLineOption _channelSizeOption;
    QCommandLineOption _producersSizeOption;
    QCommandLineOption _velocityOption;
    QCommandLineOption _mesageSizeOption;
    QCommandLineOption _portOption;

    AppData _data;
    bool _severalChannels = false;

    CommandLineParser::AppData _parse();

    void _initParser();

    bool _parseChannelData();
    bool _parseChannelOption(int size);
    bool _parseChannelOption();

    bool _parseUnobligatoryOption();

    QStringList _createChannelNames();

    void _fillChannelsData(QStringList&& velocity, QStringList&& messageSize, QStringList&& names);
};

