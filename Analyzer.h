#ifndef ANALYZER_H
#define ANALYZER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QEventLoop>
#include <QDebug>
#include <QTextCodec>

class Analyzer : public QObject
{
    Q_OBJECT
public:
    explicit Analyzer(QObject *parent = nullptr);
    Analyzer(const QString& token = QString(), const QStringList& keywords = QStringList(), QObject *parent = nullptr);

    bool checkId(const QString& id);
private:
    QString _api_base = "https://api.vk.com";
    QString _api_version = "5.130";

    QString _token;
    QStringList _keywords;

    QNetworkAccessManager* _manager = nullptr;

signals:

};

#endif // ANALYZER_H
