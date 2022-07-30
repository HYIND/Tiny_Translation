#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>
#include <windowsx.h>
#include "delimit_window.h"
#include <QMessageBox>

MainWindow* pmainwindow=nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tran.main_ui=ui;
    Tray_Init();

    // ALT + CTRL + T
    if (RegisterHotKey((HWND)this->winId(), HOT_KEY_ALT_CTRL_T, MOD_CONTROL|MOD_ALT, 0x54) == false)
    {
        QMessageBox::critical(this,"热键注册失败","热键注册失败，请检查是否有热键冲突：ALT + CTRL + T", QMessageBox::Ok, QMessageBox::Ok);
    }

    pmainwindow=this;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Translation_clicked()
{
    tran.Tranlate_front();
}

void MainWindow::Tray_Init()
{
    mSysTrayIcon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/switch.png");    //托盘icon
    mSysTrayIcon->setIcon(icon);
    mSysTrayIcon->setToolTip(QObject::tr("Tiny Translation\n划词翻译已关闭\n快捷键：CTRL+ALT+T"));       //当鼠标移动到托盘上的图标时，显示此处设置的内容

    QMenu* menu = new QMenu(this);
    toggle_action = new QAction(menu);
    QAction* exit_action = new QAction(menu);

    toggle_action->setText("开启划词");
    exit_action->setText("退出");

    menu->addAction(toggle_action);
    menu->addAction(exit_action);

    connect(toggle_action, &QAction::triggered, this, on_Switch_Delimit_clicked);
    connect(exit_action, &QAction::triggered, this, Myclose);

    mSysTrayIcon->setContextMenu(menu);
    mSysTrayIcon->show();
    connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
}
bool MainWindow::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
{
    if (eventType == "windows_generic_MSG")
    {
        MSG  *msg = (MSG*)message;
        if (msg->message == WM_HOTKEY)
        {
            switch (msg->wParam) {
            case HOT_KEY_ALT_CTRL_T:
                on_Switch_Delimit_clicked();
                break;
            default:
                qDebug() << "被注入了其他热键.";
                break;
            }
            return true;
        }
    }
    return false;
}

void MainWindow::Myclose()
{
    isexit=true;
    this->close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(isexit)
    {
        pdw->close();
        event->accept();
        UnregisterHotKey((HWND)this->winId(),HOT_KEY_ALT_CTRL_T);
    }
    else
    {
        event->ignore();
        this->hide();
        mSysTrayIcon->showMessage("TinyTranslation","程序已最小化至托盘",QSystemTrayIcon::Information,3000);
    }
}

void MainWindow::on_SwitchButton_clicked()
{
    QString left=ui->label->text();
    QString right=ui->label_2->text();
    ui->label->setText(right);
    ui->label_2->setText(left);
}

void MainWindow::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::Trigger:
        //单击托盘图标，显示主程序窗口
        this->show();
        break;
        //    case QSystemTrayIcon::DoubleClick:
        //        //双击托盘图标
        //        break;
    case QSystemTrayIcon::Context:
        mSysTrayIcon->contextMenu()->show();
        break;
    default:
        break;
    }
}

void MainWindow::on_Switch_Delimit_clicked()
{
    Delimit_toggle=!Delimit_toggle;
    if(Delimit_toggle)
    {
        ui->Switch_Delimit->setText("关闭划词");
        toggle_action->setText("关闭划词");
        mSysTrayIcon->setToolTip(QObject::tr("Tiny Translation\n划词翻译已关闭\n开启快捷键：CTRL+ALT+T"));
    }
    else
    {
        ui->Switch_Delimit->setText("开启划词");
        toggle_action->setText("开启划词");
        mSysTrayIcon->setToolTip(QObject::tr("Tiny Translation\n划词翻译已开启\n关闭快捷键：CTRL+ALT+T"));
    }
}


