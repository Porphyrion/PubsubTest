#include "CommandLineParserSub.h"

CommandLineParserSub::CommandLineParserSub(QStringList args) :
    _parser(),
    _portOption(QStringList()<<"p"<<"port", "Port", "Port"),
    _hostOption(QStringList()<<"host", "Host", "Host"),
    _chanelOption(QStringList()<<"c"<<"chanel", "Chanel", "Chanel")
{
    _initParser();
    _parser.process(args);
}


bool CommandLineParserSub::parse()
{
    if(_parser.isSet(_hostOption))
        _data.host = _parser.value(_hostOption);
    else
        _data.host = _hostOption.defaultValues().first();

    if(_parser.isSet(_portOption))
        _data.port = _parser.value(_portOption).toInt();
    else
        _data.port = _portOption.defaultValues().first().toInt();

    if(_parser.isSet(_chanelOption))
        _data.chanel = _parser.value(_chanelOption);
    else
        _data.chanel = _chanelOption.defaultValues().first();

    return true;
}


void CommandLineParserSub::_initParser()
{
    _parser.addHelpOption();

    _portOption.setDefaultValue("1488");
    _parser.addOption(_portOption);

    _hostOption.setDefaultValue("127.0.0.1");
    _parser.addOption(_hostOption);

    _chanelOption.setDefaultValue("test1");
    _parser.addOption(_chanelOption);
}
