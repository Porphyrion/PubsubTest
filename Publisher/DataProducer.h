#pragma once

#include <QObject>
#include <QByteArray>
#include <QList>

class DataProducer : public QObject
{
    Q_OBJECT

     const int FREQUENCY = 10;

public:
    explicit DataProducer(int messageSize, int speed,  QObject *parent = nullptr);


    int size() { return _size;}
    int speed() {return _speed;}


public slots:
     void produce();
     void stopProduce();

signals:
    void sendMessage(const QByteArray message);

private:
    QByteArray _message;

    int _timersAmount;
    int _speed;
    int _size;

     QList<int> _timersId;

protected:
    void timerEvent(QTimerEvent *event) override;
};

