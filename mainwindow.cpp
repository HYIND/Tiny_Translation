#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tran.mainwindow=this;
    tran.main_ui=ui;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Translation_clicked()
{
    tran.Tranlate_front();
}

void MainWindow::on_SwitchButton_clicked()
{
    QString left=ui->label->text();
    QString right=ui->label_2->text();
    ui->label->setText(right);
    ui->label_2->setText(left);
}


//MyApplication::MyApplication(int &argc, char *argv[]):QApplication(argc,argv)
//{
//}

//MyApplication::~MyApplication()
//{
//}

//void MyApplication::MouseMoveEvent(QMouseEvent *p)
//{
//    QPoint p_ab = p->globalPos();//整个桌面位置
//    QString str;
//    str = QString("%1 , %2").arg(p_ab.x()).arg(p_ab.y());
//    qDebug()<<str;
//}

//bool MyApplication::notify(QObject *obj, QEvent *event)
//{
//    if(event->type()==QMouseEvent::MouseMove)
//    {
//        QMouseEvent *mouseevent=(QMouseEvent *)event;
//        QPoint p_ab = mouseevent->globalPos();//整个桌面位置
//        QString str;
//        str = QString("%1 , %2").arg(p_ab.x()).arg(p_ab.y());
//        qDebug()<<str;
//    }
//    return QApplication::notify(obj,event);
//}

//void mouseMoveEvent(QMouseEvent *event)
//{
//    QPoint p_ab = event->globalPos();//整个桌面位置
//    QString str;
//    str = QString("%1 , %2").arg(p_ab.x()).arg(p_ab.y());
//    qDebug()<<str;
//}

void MainWindow::on_Switch_Delimit_clicked()
{
    Delimit_toggle=!Delimit_toggle;
    if(Delimit_toggle)
        ui->Switch_Delimit->setText("关闭划词");
    else ui->Switch_Delimit->setText("开启划词");
}

