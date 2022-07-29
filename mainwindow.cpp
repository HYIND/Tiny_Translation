#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tran.mainwindow=this;
    tran.main_ui=ui;
    Tray_Init();
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


void MainWindow::changeEvent(QEvent * event)
{
    if (this->windowState() == Qt::WindowMinimized)
    {
        this->hide();
        mSysTrayIcon->show();
    }
}

void MainWindow::Tray_Init()
{
       mSysTrayIcon = new QSystemTrayIcon(this);
       QIcon icon = QIcon(":/switch.png");    //托盘icon
       mSysTrayIcon->setIcon(icon);
       mSysTrayIcon->setToolTip(QObject::tr("Tiny Translation"));       //当鼠标移动到托盘上的图标时，显示此处设置的内容
       connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
               this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::Trigger:
        //单击托盘图标
        break;
    case QSystemTrayIcon::DoubleClick:
        //双击托盘图标
        //双击后显示主程序窗口
        this->show();
        mSysTrayIcon->hide();
        break;
    default:
        break;
    }
}

void MainWindow::on_Switch_Delimit_clicked()
{
    Delimit_toggle=!Delimit_toggle;
    if(Delimit_toggle)
        ui->Switch_Delimit->setText("关闭划词");
    else ui->Switch_Delimit->setText("开启划词");
}

