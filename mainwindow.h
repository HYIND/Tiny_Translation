#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include "Translator.h"
#include <QThread>
#include <QApplication>
#include <QObject>
#include "MouseHook.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//class MyApplication:public QApplication
//{
//    Q_OBJECT

//public:
//    MyApplication(int& argc,char *argv[]);
//    ~MyApplication();

//    void MouseMoveEvent(QMouseEvent *p);
//    bool notify(QObject* obj,QEvent* e);
//};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;

private slots:
    void on_Translation_clicked();
    void on_SwitchButton_clicked();
    void on_Switch_Delimit_clicked();
};

void mouseMoveEvent(QMouseEvent *event);

#endif // MAINWINDOW_H
