#include "DataProducer.h"

DataProducer::DataProducer(int messageSize, int speed,  QObject *parent)
    : QObject{parent}, _message(messageSize, 'f'), _speed(speed), _size(messageSize), _timerId(0)
{

}


void DataProducer::produce()
{
     _timerId = startTimer(_speed);
}


void DataProducer::stopProduce()
{
    killTimer(_timerId);
}


void DataProducer::timerEvent(QTimerEvent *event)
{
    emit sendMessage(_message);
}

