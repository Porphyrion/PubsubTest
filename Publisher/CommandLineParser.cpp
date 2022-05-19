#include "CommandLineParser.h"
#include <tuple>

#include <QDebug>


CommandLineParser::CommandLineParser(QStringList args) : _parser(),
    _channelSizeOption(QStringList()<<"n"<<"number", "Number of publushers", "Number"),
    _velocityOption(QStringList()<<"v"<<"velocitys","Velocity in messages per second. Can accept several parameters", "Velocity"),
    _mesageSizeOption(QStringList()<<"s"<<"sizes", "Message size. Can accept several parameters", "Size"),
    _channelNameOption(QStringList()<<"x"<<"names", "Channels name. Can accept several parameters", "Names"),
    _portOption(QStringList()<<"p"<<"port", "Port binding", "Port"),
    _severalPublishersOption("e", "Single publisher for Each channel")
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


    if(_parser.isSet(_severalPublishersOption))
        _data.severalPublishers = true;
    else
        _data.severalPublishers = false;

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

    _channelSizeOption.setDefaultValues({"1"});
    _parser.addOption(_channelSizeOption);

    _channelNameOption.setDefaultValues({"test1"});
    _parser.addOption(_channelNameOption);


    _portOption.setDefaultValues({"1488"});
    _parser.addOption(_portOption);

    _parser.addOption(_severalPublishersOption);
}


bool CommandLineParser::_parseChannelData()
{
    //! If there is numberoption each option have to be set and have number arguments
    if(_parser.isSet(_channelSizeOption))
       return _parseChannelOption(_parser.value(_channelSizeOption).toInt());
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

    if(_parser.isSet(_channelNameOption))
    {
        n = _parser.values(_channelNameOption).first().split(",");
        if(n.size() != size)
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


    if(_parser.isSet(_channelNameOption))
        n = _parser.values(_channelNameOption);
    else
        n = _channelNameOption.defaultValues();

    _fillChannelsData(std::move(v), std::move(m), std::move(n));

    return true;
}


void CommandLineParser::_fillChannelsData(QStringList&& velocity, QStringList&& messageSize, QStringList&& names)
{
    for(auto i = 0; i < velocity.size(); ++i)
    {
        auto vel = velocity.at(i).toInt();
        auto mesSize = messageSize.at(i).toInt();
        auto channelName = names.at(i);
        auto channel = std::make_tuple(channelName, mesSize, vel);

        _data.channels.append(channel);
    }
}
