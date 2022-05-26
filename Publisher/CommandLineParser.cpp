#include "CommandLineParser.h"
#include <tuple>

#include <QDebug>


CommandLineParser::CommandLineParser(QStringList args, QObject * parent) :
    QObject{parent},
    _parser(),
    _channelSizeOption(QStringList()<<"n"<<"number", "Number of chanels", "Number"),
    _producersSizeOption(QStringList()<<"p"<<"producers", "Number of producers", "Producers"),
    _velocityOption(QStringList()<<"v"<<"velocitys","Velocity in messages per second. Can accept several parameters", "Velocity"),
    _mesageSizeOption(QStringList()<<"s"<<"sizes", "Message size. Can accept several parameters", "Size"),
    _portOption(QStringList()<<"port", "Port binding", "Port")
{
    _initParser();
    _parser.process(args);
}


bool CommandLineParser::parse()
{
    return (_parseChannelData() && _parseUnobligatoryOption());
}


CommandLineParser::AppData CommandLineParser::data()
{
    return _data;
}


QString CommandLineParser::help()
{
    return _parser.helpText();
}


bool CommandLineParser::_parseUnobligatoryOption()
{

    if(_parser.isSet(_portOption))
        _data.port = _parser.value(_portOption).toInt();
    else
        _data.port = _portOption.defaultValues().at(0).toInt();

    return true;
}


void CommandLineParser::_initParser()
{
    _parser.setApplicationDescription("ZeroMQ publisher tester");

    _parser.addHelpOption();

    _velocityOption.setDefaultValues({"20"});
    _parser.addOption(_velocityOption);

    _mesageSizeOption.setDefaultValues({"32"});
    _parser.addOption(_mesageSizeOption);

    _parser.addOption(_producersSizeOption);

    _channelSizeOption.setDefaultValues({"1"});
    _parser.addOption(_channelSizeOption);

    _portOption.setDefaultValues({"1488"});
    _parser.addOption(_portOption);

}


bool CommandLineParser::_parseChannelData()
{
    //! If there is number option each option have to be set and have number arguments
    if(_parser.isSet(_producersSizeOption))
       return _parseChannelOption(_parser.value(_producersSizeOption).toInt());
    else
        return _parseChannelOption();
}


bool CommandLineParser::_parseChannelOption(int size)
{
    QStringList v, m, n;

    if(_parser.isSet(_velocityOption))
    {
        v = _parser.values(_velocityOption).first().split(",");
        if(v.size() != size)
            return false;
    }

    if(_parser.isSet(_mesageSizeOption))
    {
        m = _parser.values(_mesageSizeOption).first().split(",");
        if(m.size() != size)
            return false;
    }

    _fillChannelsData(std::move(v), std::move(m), std::move(n));

    return true;
}


bool CommandLineParser::_parseChannelOption()
{
    QStringList v, m, n;

    if(_parser.isSet(_velocityOption))
        v = _parser.values(_velocityOption);
    else
        v = _velocityOption.defaultValues();


    if(_parser.isSet(_mesageSizeOption))
        m = _parser.values(_mesageSizeOption);
    else
        m = _mesageSizeOption.defaultValues();

    _fillChannelsData(std::move(v), std::move(m), std::move(n));

    return true;
}


QStringList CommandLineParser::_createChannelNames()
{
    QStringList names;
    if(_parser.isSet(_channelSizeOption))
    {
        auto size = _parser.value(_channelSizeOption).toInt();
        for(auto i = 0; i < size; ++i)
            names.append(QString("test") + QString::number(i));
    }
    else
        names.append(QString("test") + QString::number(1));

    return names;
}


void CommandLineParser::_fillChannelsData(QStringList&& velocity, QStringList&& messageSize, QStringList&& names)
{

    _data._chanelNames = _createChannelNames();


    for(auto i = 0; i < velocity.size(); ++i)
    {
        auto vel = velocity.at(i).toInt();
        auto mesSize = messageSize.at(i).toInt();
        auto channel = std::make_pair(mesSize, vel);

        _data._producers.append(channel);
    }
}
