#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

#include "Analyzer.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QLineEdit* _id_field = nullptr;
    QLabel* _id_label = nullptr;
    QPushButton* _start_button = nullptr;
    QGridLayout* _layout = nullptr;

    Analyzer* _analyzer = nullptr;

private slots:
    void startAnalyze();

};
#endif // MAINWINDOW_H
