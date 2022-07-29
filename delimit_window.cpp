#include "delimit_window.h"
#include "ui_delimit_window.h"

delimit_window::delimit_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::delimit_window)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Popup );
    tran.back_widget=this;
    tran.back_ui=ui;

    QThread *back_thread=new QThread();
    (&tran)->moveToThread(back_thread);
    connect(&tran,&Translator::back_tran,&tran,&Translator::Tranlate_back);
    back_thread->start();
    connect(&tran,&Translator::back_tran_finish,this,&delimit_window::tran_finish);
}

delimit_window::~delimit_window()
{
    delete ui;
}

void delimit_window::tran_finish(QString &string)
{
    ui->textBrowser->setText(string);
    this->move(MoveX,MoveY);
    this->show();
    qDebug()<<"1111";
}
