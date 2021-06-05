#include "Analyzer.h"

Analyzer::Analyzer(QObject *parent) : QObject(parent)
{
    _token = QString();
    _keywords = QStringList();
    _manager = new QNetworkAccessManager(this);
}
Analyzer::Analyzer(const QString& token, const QStringList& keywords, QObject *parent)
    : QObject(parent)
{
    _token = token;
    _keywords = keywords;
    _manager = new QNetworkAccessManager(this);
}
bool Analyzer::checkId(const QString& id){
    QString url = QString("%1/method/wall.get?domain=%2&access_token=%3&v=%4").arg(_api_base).arg(id).arg(_token).arg(_api_version);

    QNetworkRequest request;
    request.setUrl(url);
    QNetworkReply* reply = _manager->get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError){
        QByteArray content = reply->readAll();
        QTextCodec *codec = QTextCodec::codecForName("utf8"); // cp-1251
        qDebug() << codec->toUnicode(content.data());
        return true;
    } else {
        qDebug() << reply->errorString();
    }

    reply->deleteLater();
    return false;
}
