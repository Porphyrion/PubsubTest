#include "CommnadLineParser.h"
#include <tuple>

#include <QDebug>


CommnadLineParser::CommnadLineParser(QStringList args) : _parser(),
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


bool CommnadLineParser::parse()
{
    return (_parseChannelData() || _parseUnobligatoryOption());
}


CommnadLineParser::AppData CommnadLineParser::data()
{
    return _data;
}


QString CommnadLineParser::help()
{
    return _parser.helpText();
}


bool CommnadLineParser::_parseUnobligatoryOption()
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


void CommnadLineParser::_initParser()
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


bool CommnadLineParser::_parseChannelData()
{
    unsigned int size = _channelSizeOption.defaultValues().at(0).toInt();

    if(_parser.isSet(_channelSizeOption))
    {
        _severalChannels = true;
        size = _parser.value(_channelSizeOption).toInt();
    }


    QStringList v, m, n;

    if(_parser.isSet(_velocityOption))
    {
        v = _parser.values(_velocityOption);
        if(v.size() != size)
            return false;
    }
    else
        v = _velocityOption.defaultValues();



    if(_parser.isSet(_mesageSizeOption))
    {
        m = _parser.values(_mesageSizeOption);
        if(m.size() != size)
            return false;
    }
    else
        m = _mesageSizeOption.defaultValues();


    if(_parser.isSet(_channelNameOption))
    {
        n = _parser.values(_channelNameOption);
        if(n.size() != size)
            return false;
    }
    else
        n = _channelNameOption.defaultValues();


    for(auto i = 0; i < size; ++i)
    {
        auto velocity = v.at(i).toInt();
        auto messageSize = m.at(i).toInt();
        auto channelName = n.at(i);
        auto channel = std::make_tuple(channelName, messageSize, velocity);

        _data.channels.append(channel);
    }

    return true;
}

