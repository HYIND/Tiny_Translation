#ifndef TRAY_H
#define TRAY_H



class SystemTray : public QWidget
{
    Q_OBJECT

public:
    SystemTray(QWidget *parent = 0);
    ~SystemTray();

protected:
    void activeTray(QSystemTrayIcon::ActivationReason reason);
    void showWindow();//显示窗体
    void showMessage();//消息框
    void showMenu();//显示菜单

private:
    QSystemTrayIcon *m_systemTray;

    QMenu *m_menu;
    QAction *m_action1;
    QAction *m_action2;
};

#endif // TRAY_H
