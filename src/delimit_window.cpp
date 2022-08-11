#include "delimit_window.h"
#include "ui_delimit_window.h"
#include <windows.h>
#include <wingdi.h>
#include <QFontMetrics>

delimit_window* pdw;

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
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Popup/*|Qt::Tool| Qt::WindowStaysOnTopHint*/);
    this->setAttribute(Qt::WA_ShowWithoutActivating,true);
    this->setAttribute(Qt::WA_NoMouseReplay);
    tran.back_widget=this;
    tran.back_ui=ui;

    QThread *back_thread=new QThread();
    (&tran)->moveToThread(back_thread);
    connect(&tran,&Translator::back_tran,&tran,&Translator::Tranlate_back);
    back_thread->start();
    connect(&tran,&Translator::back_tran_finish,this,&delimit_window::tran_finish);

    pdw=this;
}

delimit_window::~delimit_window()
{
    delete ui;
}

void delimit_window::tran_finish(QString &string,QMimeData* oldmimedata)
{
    if(!string.isEmpty())
    {
        ui->textBrowser->setText(string);
        //    QFontMetrics fm(ui->textBrowser->font());
        //    QRect rec = fm.boundingRect("字");
        //    //字符串所占的像素宽度,高度
        //    int textWidth = rec.width();
        //    int textHeight = rec.height();
        //    qDebug()<<textWidth<<textHeight;

        //   int textperline=300/textWidth;
        //   int width=(ui->textBrowser->toPlainText().length()) * textWidth<300?
        //               (ui->textBrowser->toPlainText().length()) * textWidth:300;
        //   int height=textHeight*(ui->textBrowser->toPlainText().length()/textperline+1);
        //   qDebug()<<width<<height;
        //   ui->textBrowser->resize(width,height);
        this->move(MoveX/Scale+10,MoveY/Scale+10);
        this->show();
    }

    QClipboard* clipboard=QApplication::clipboard();
    clipboard->setMimeData(oldmimedata);       //恢复剪切板
    delete oldmimedata;
}

