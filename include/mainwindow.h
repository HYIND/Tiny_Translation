#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include "Translator.h"
#include <QThread>
#include <QApplication>
#include <QObject>
#include "MouseHook.h"
#include <QSystemTrayIcon>
#include <QAbstractNativeEventFilter>
#include <QMenu>
#include <QAction>

//划词开关热键
#define HOT_KEY_ALT_CTRL_T 300

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow,public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;
//    void changeEvent(QEvent * event);
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result);

private:
    bool isexit=false;
    QSystemTrayIcon* mSysTrayIcon;
    QAction* toggle_action;
    void Myclose();
    void Tray_Init();
    void closeEvent(QCloseEvent *event);

private slots:
    void on_Translation_clicked();
    void on_SwitchButton_clicked();
    void on_Switch_Delimit_clicked();
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
};

extern MainWindow* pmainwindow;

void mouseMoveEvent(QMouseEvent *event);
#endif // MAINWINDOW_H
