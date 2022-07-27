#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

class Translator:public QObject
{
public:
    Translator(){}
    bool TranslateByGoogle();
};

void ui_init(Ui::MainWindow *ui_ptr);

#endif // TRANSLATOR_H
