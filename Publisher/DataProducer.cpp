#include <QDebug>

#include "DataProducer.h"

DataProducer::DataProducer(std::string message, int speed, QObject *parent)
    : QObject{parent}, _message(message), _count(0)
{
    startTimer(speed);
}


std::string DataProducer::produce()
{
    return _createMessage();
}


std::string DataProducer::_createMessage()
{
    auto result = _message + " " + std::to_string(_count++);
    return result;
}


void DataProducer::timerEvent(QTimerEvent *event)
{
    emit sendMessage(QString::fromStdString(_createMessage()));
}

