#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <QCoreApplication>


#include <QCommandLineParser>
#include <QCommandLineOption>

#include "publisherapp.h"


int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;

    parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination directory."));

    parser.setOptionsAfterPositionalArgumentsMode(QCommandLineParser::ParseAsPositionalArguments);
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);

    QCommandLineOption fastPublisher(QStringList()<<"f"<<"fast",
                                     QCoreApplication::translate("main", "Amount of fast prodecures"),
                                     QCoreApplication::translate("main", "Fast producers"));
    parser.addOption(fastPublisher);

    QCommandLineOption bigMessagePublisher("b",
                                           QCoreApplication::translate("main", "Amount of fast prodecures witch makes big messages"),
                                           QCoreApplication::translate("main", "Big"));
    parser.addOption(bigMessagePublisher);

    QCommandLineOption bigFast("fb",
                               QCoreApplication::translate("main", "Amount of fast prodecures witch makes big messages"),
                               QCoreApplication::translate("main", "Fast and big "));
    parser.addOption(bigFast);

    QCommandLineOption casual("c",
                              QCoreApplication::translate("main", "Amount of casual prodecers"),
                              QCoreApplication::translate("main", "Casual"));
    auto x = parser.addOption(casual);

    parser.process(a);


    if(parser.isSet(fastPublisher))
        f = parser.value(fastPublisher).toInt();

    if(parser.isSet(bigMessagePublisher))
        b = parser.value(bigMessagePublisher).toInt();

    if(parser.isSet(bigFast))
        bf = parser.value(bigFast).toInt();

    if(parser.isSet(casual))
        f = parser.value(casual).toInt();

    qDebug()<<f<<b<<bf<<c;

    const QStringList args = parser.positionalArguments();

    PublisherApp p(c, f, b, bf);

    return a.exec();
}
