#include "delimit_window.h"
#include "ui_delimit_window.h"
#include <windows.h>
#include <wingdi.h>

double get_Scale()
{
    HWND hwd = ::GetDesktopWindow();
    HDC hdc = ::GetDC(hwd);
    int Scale_width = GetSystemMetrics(SM_CXSCREEN); //屏幕宽度
    int Dev_width = GetDeviceCaps (hdc, DESKTOPHORZRES); //实际宽度
    return (double)Dev_width/Scale_width;
}

double Scale=get_Scale();

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
    this->move(MoveX/Scale,MoveY/Scale);
    this->show();
}
