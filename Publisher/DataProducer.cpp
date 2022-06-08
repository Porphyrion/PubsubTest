#include "DataProducer.h"

DataProducer::DataProducer(int messageSize, int speed,  QObject *parent)
    : QObject{parent}, _message(messageSize, 'f'), _speed(speed), _size(messageSize)
{
    _timersAmount = _speed/100;


}


void DataProducer::produce()
{
      for(auto i = 0; i < _timersAmount; ++i)
        startTimer(FREQUENCY);
}


void DataProducer::stopProduce()
{
    for(auto id : _timersId)
        killTimer(id);
}


void DataProducer::timerEvent(QTimerEvent *event)
{
    emit sendMessage(_message);
}

