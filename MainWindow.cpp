#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    _id_label = new QLabel("ID пользователя:",this);
    _id_field = new QLineEdit(this);
    _start_button = new QPushButton("Анализировать", this);

    _layout = new QGridLayout(this);
    _layout->addWidget(_id_label, 0, 0);
    _layout->addWidget(_id_field, 0, 1);
    _layout->addWidget(_start_button, 1, 1);

    QObject::connect(_start_button, &QPushButton::clicked, this, &MainWindow::startAnalyze);

    _analyzer = new Analyzer("31a83b8f31a83b8f31a83b8fdc31deddad331a831a83b8f51e7a2cc1502785511f775e0", QStringList(), this);
}

void MainWindow::startAnalyze(){
    QString user_id = _id_field->text();
    if(user_id.isEmpty()){
        _id_field->setText("Введите ID пользователя!");
        return;
    }
    qDebug() << _analyzer->checkId(user_id);
}
MainWindow::~MainWindow()
{
}

