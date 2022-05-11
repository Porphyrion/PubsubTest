#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>

struct PublisherPrivate;

class Publisher
    : public QObject
{
    Q_OBJECT
public:

    Publisher(uint port = 0, QObject* parent = nullptr);
    ~Publisher() override;

    uint    port() const;

public slots:

    void publish(QByteArray const& msg, QString channel = QString{});

private:


    PublisherPrivate*   _d = nullptr;
};