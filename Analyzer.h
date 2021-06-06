#ifndef ANALYZER_H
#define ANALYZER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QEventLoop>
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

//#define DEBUG

#ifdef DEBUG
    #include <QDebug>
#endif

class Analyzer : public QObject
{
    Q_OBJECT
public:
    explicit Analyzer(QObject *parent = nullptr);
    Analyzer(const QString& token = QString(), const QStringList& keywords = QStringList(), QObject *parent = nullptr);

    bool checkId(const QString& id);
    QJsonArray loadPosts(const QString& id);
    int getTotalCountWords(const QJsonArray& posts);
    int getCountKeywords(const QJsonArray& posts);
    QStringList getUrlPostsByKeywords(const QString& id, const QJsonArray& posts);
    QStringList getKeywords();
private:
    QString _api_base = "https://api.vk.com";
    QString _api_version = "5.130";

    QString _token;
    QStringList _keywords;

    QNetworkAccessManager* _manager = nullptr;
    int _num_post_in_request = 100;

    QJsonArray _getPosts(const QString& id, int& total_count, int offset = 0, int num = 100);
    QJsonArray _getAllPosts(const QString& id);
    QStringList _getTextFromPosts(const QJsonArray& posts);
    int _totalCountWords(const QStringList& posts_text);

signals:

};

#endif // ANALYZER_H
