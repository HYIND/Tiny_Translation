#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "mainwindow.h"
#include "delimit_window.h"
#include "ui_mainwindow.h"
#include "ui_delimit_window.h"
#include <QClipboard>
#include <QMimeData>

class Translator:public QObject
{
    Q_OBJECT
public:
    Ui::MainWindow *main_ui = nullptr;
    Ui::delimit_window *back_ui = nullptr;

    QWidget *back_widget = nullptr;
//    QMainWindow* pmainwindow;

    Translator(){}
    void Tranlate_front();
    bool TranslateByGoogle(QString& in,QString& out,QString intype="en",QString outtype="zh_CN");
    void Tranlate_back_emit();
signals:
    void back_tran();
    void back_tran_finish(QString &string,QMimeData* oldmimedata);
public slots:
    void Tranlate_back();
};


extern Translator tran;

#endif // TRANSLATOR_H
