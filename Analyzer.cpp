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
    QString url = QString("%1/method/users.get?user_ids=%2&access_token=%3&v=%4").arg(_api_base).arg(id).arg(_token).arg(_api_version);

    QNetworkRequest request;
    request.setUrl(url);
    QNetworkReply* reply = _manager->get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool is_open_profile = false;
    if (reply->error() == QNetworkReply::NoError){
        QByteArray content = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(content);
#ifdef DEBUG
        qDebug() << json;
#endif
        if(json.isObject()){
            QJsonObject object = json.object();
            QJsonValue resp = object["response"];
            if(resp.isArray()){
                QJsonValue user = resp.toArray().first();
                if(user.isObject()){
                    QJsonValue is_close = user["is_closed"];
                    if(is_close.isBool())
                        is_open_profile = !is_close.toBool();
                }
            }
        }
    } else {
#ifdef DEBUG
        qDebug() << reply->errorString();
#endif
    }

    reply->deleteLater();
    return is_open_profile;
}

QJsonArray Analyzer::loadPosts(const QString& id){
    return _getAllPosts(id);
}
int Analyzer::getTotalCountWords(const QJsonArray& posts){
    QStringList texts = _getTextFromPosts(posts);
    return _totalCountWords(texts);
}
int Analyzer::getCountKeywords(const QJsonArray& posts){
    int count_keywords = 0;
    QStringList texts = _getTextFromPosts(posts);
    for(const QString& text : texts){
        QStringList words = text.split(" ");
        for(const QString& word : words)
            if(_keywords.contains(word, Qt::CaseInsensitive))
                count_keywords++;
    }
    return count_keywords;
}
QStringList Analyzer::getUrlPostsByKeywords(const QString& user_id, const QJsonArray& posts){
    QStringList urls;
    for(const auto& post : posts){
        QJsonValue _text = post["text"];
        if(_text.isString()){
            QString text = _text.toString();
            for(const QString& keyword : _keywords){
                if(text.contains(keyword, Qt::CaseInsensitive)){
                    QJsonValue _id = post["id"];
                    QJsonValue _from_id = post["from_id"];
                    int id = static_cast<int>(_id.toDouble());
                    int from_id = static_cast<int>(_from_id.toDouble());
                    QString url = QString("https://vk.com/%1?w=wall%2_%3").arg(user_id).arg(from_id).arg(id);
                    urls.append(url);
                }

            }
        }
    }
    return urls;
}
QStringList Analyzer::getKeywords(){
    return _keywords;
}
QJsonArray Analyzer::_getPosts(const QString& id, int& total_count, int offset, int num){
    QString url = QString("%1/method/wall.get?domain=%2&offset=%3&count=%4&access_token=%5&v=%6").arg(_api_base).arg(id).arg(offset).arg(num).arg(_token).arg(_api_version);
    QNetworkRequest request;
    request.setUrl(url);
    QNetworkReply* reply = _manager->get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QJsonArray posts;
    if (reply->error() == QNetworkReply::NoError){
        QByteArray content = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(content);
#ifdef DEBUG
        qDebug() << json;
#endif
        if(json.isObject()){
            QJsonObject object = json.object();
            QJsonValue resp = object["response"];
            if(resp.isObject()){
                QJsonValue count = resp["count"];
                QJsonValue items = resp["items"];

                if(count.isDouble())
                    total_count = static_cast<int>(count.toDouble());
                if(items.isArray()){
                    posts = items.toArray();
                }
            }
        }
    } else {
#ifdef DEBUG
        qDebug() << reply->errorString();
#endif
    }
    return posts;
}
QJsonArray Analyzer::_getAllPosts(const QString& id){
    int total_count = 0;
    QJsonArray all_posts = _getPosts(id, total_count, 0, _num_post_in_request);
    for(int offset = _num_post_in_request; offset < total_count; offset += _num_post_in_request){
#ifdef DEBUG
        qDebug() << offset << " / " << total_count;
#endif
        QJsonArray posts = _getPosts(id, total_count, offset, _num_post_in_request);
        for(const auto& post : posts)
            all_posts.append(post);
    }
#ifdef DEBUG
    qDebug() << offset << " / " << total_count;
#endif
    return all_posts;
}
QStringList Analyzer::_getTextFromPosts(const QJsonArray& posts){
    QStringList texts;
    for(const auto& post : posts){
        QJsonValue _text = post["text"];
        if(_text.isString()){
            QString text = _text.toString();
            texts.append(text);
        }
    }
    return texts;
}
int Analyzer::_totalCountWords(const QStringList& posts_text){
    int total_count_words = 0;
    for(const QString& text : posts_text){
        QStringList words = text.split(" ");
        total_count_words += words.size();
    }
    return total_count_words;
}

