#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

class Translator:public QObject
{
public:
    Ui::MainWindow *ui = nullptr;
    QMainWindow* mainwindow;

    Translator(){}
    void Tranlate_front();
    bool TranslateByGoogle(QString& in,QString& out,QString intype,QString outtype);

};

void ui_init(Ui::MainWindow *ui_ptr);

#endif // TRANSLATOR_H
