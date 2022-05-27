#include "CommandLineParserSub.h"

CommandLineParserSub::CommandLineParserSub(QStringList args) :
    _parser(),
    _portOption(QStringList()<<"p"<<"port", "Port", "Port"),
    _hostOption(QStringList()<<"host", "Host", "Host"),
    _chanelOption(QStringList()<<"c"<<"chanel", "Chanel", "Chanel"),
    _timeOutOption(QStringList()<<"t"<<"timeout","Timeout", "Subscribtion duration" )
{
    _initParser();
    _parser.process(args);
}


bool CommandLineParserSub::parse()
{
    _data.host = _parser.isSet(_hostOption) ? _parser.value(_hostOption) :  _hostOption.defaultValues().first();

    _data.port = _parser.isSet(_portOption) ? _parser.value(_portOption).toInt() : _portOption.defaultValues().first().toInt();

    _data.chanels = _parser.isSet(_chanelOption) ? _parser.value(_chanelOption).split(",") : _chanelOption.defaultValues();

    _data.timeout = _parser.isSet(_timeOutOption) ? _parser.value(_timeOutOption).toInt() : _timeOutOption.defaultValues().first().toInt();

    return true;
}


void CommandLineParserSub::_initParser()
{
    _parser.addHelpOption();

    _portOption.setDefaultValue("1488");
    _parser.addOption(_portOption);

    _hostOption.setDefaultValue("127.0.0.1");
    _parser.addOption(_hostOption);

    _chanelOption.setDefaultValues({"test1"});
    _parser.addOption(_chanelOption);

    _timeOutOption.setDefaultValue("1000");
    _parser.addOption(_timeOutOption);
}
