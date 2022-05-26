#pragma once

#include <QObject>
#include <QByteArray>

class DataProducer : public QObject
{
    Q_OBJECT
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

    int _speed;
    int _size;

    int _timerId;

protected:
    void timerEvent(QTimerEvent *event) override;
};

