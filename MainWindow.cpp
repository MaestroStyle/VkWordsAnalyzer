#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    _id_label = new QLabel("ID пользователя:",this);
    _id_field = new QLineEdit(this);
    _start_button = new QPushButton("Анализировать", this);
    _text_edit = new QTextEdit(this);
    _text_edit->setReadOnly(true);

    _layout = new QGridLayout(this);
    _layout->addWidget(_id_label, 0, 0);
    _layout->addWidget(_id_field, 0, 1);
    _layout->addWidget(_start_button, 1, 1);
    _layout->addWidget(_text_edit, 2, 0, 1, 2);

    QObject::connect(_start_button, &QPushButton::clicked, this, &MainWindow::startAnalyze);

    QFile f("keywords.txt");
    f.open(QIODevice::ReadOnly);
    QTextStream fkeywords(&f);
    QStringList keywords;
    while(!fkeywords.atEnd()){
        QString keyword;
        fkeywords >> keyword;
        keywords.append(keyword);
    }
    f.close();

    _analyzer = new Analyzer("31a83b8f31a83b8f31a83b8fdc31deddad331a831a83b8f51e7a2cc1502785511f775e0", keywords, this);
}

void MainWindow::startAnalyze(){
    _start_button->setEnabled(false);
    QString user_id = _id_field->text();
    if(user_id.isEmpty()){
        _id_field->setText("Введите ID пользователя!");
        return;
    }
    if(!_analyzer->checkId(user_id)){
        _id_field->setText("Профиль не доступен!");
        return;
    }
    QJsonArray posts = _analyzer->loadPosts(user_id);
    int num_words = _analyzer->getTotalCountWords(posts);
    int num_keywords = _analyzer->getCountKeywords(posts);
    double keyword_share = static_cast<double>(num_keywords) / static_cast<double>(num_words) * 100;

    QStringList urls = _analyzer->getUrlPostsByKeywords(user_id, posts);
    QString urls_list;
    for(const QString& url : urls)
        urls_list += QString("<a href=%1>%1</a><br>").arg(url);

    QStringList keywords = _analyzer->getKeywords();
    QString keywords_list;
    for(const QString& keyword : keywords)
        keywords_list += keyword + "<br>";

    QString conclusion = QString("Общее кол-во слов в постах: %1<br> \
                                 Кол-во встерчающихся ключевых слов: %2<br> \
                                 Доля ключевых слов: %3%<br> \
                                 <br>Посты с ключевыми словами: <br>%4<br> \
                                 <br>Ключевые слова: <br>%5<br>").arg(num_words).arg(num_keywords).arg(QString::number(keyword_share, 'f', 2)).arg(urls_list).arg(keywords_list);

    _text_edit->setText(conclusion);

    QDir dir;
    dir.mkdir("conclusions");
    QString fname = QString("conclusions/conclusion_%1_%2.html").arg(user_id).arg(QDateTime::currentDateTime().toString(Qt::ISODate));
    fname.replace(":","_");
    QFile f(fname);
    if(f.open(QIODevice::WriteOnly)){
        QTextStream output(&f);
        output << conclusion;
        f.close();
    }
    _start_button->setEnabled(true);
}
MainWindow::~MainWindow()
{
}

