#pragma once

#include <QObject>
#include <string>

class DataProducer : public QObject
{
    Q_OBJECT
public:
    explicit DataProducer(std::string message, int speed,  QObject *parent = nullptr);

    std::string produce();

signals:
    void sendMessage(QString message);

private:
    std::string _message;
    unsigned int _count;

    std::string _createMessage();

protected:
    void timerEvent(QTimerEvent *event) override;
};

