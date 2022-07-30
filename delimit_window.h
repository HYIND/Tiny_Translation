#ifndef DELIMIT_WINDOW_H
#define DELIMIT_WINDOW_H

#include <QWidget>
#include "Translator.h"

QT_BEGIN_NAMESPACE
namespace Ui {class delimit_window;}
QT_END_NAMESPACE

class delimit_window : public QWidget
{
    Q_OBJECT

public:
    explicit delimit_window(QWidget *parent = nullptr);
    ~delimit_window();

private:
    Ui::delimit_window *ui;

public slots:
    void tran_finish(QString &string,QMimeData* oldmimedata);
};


extern delimit_window* pdw;

#endif // DELIMIT_WINDOW_H
