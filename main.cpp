#include "mainwindow.h"
#include "delimit_window.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    Translator tran;
    MainWindow mainwindow;
    mainwindow.show();
    delimit_window dw;
    SetMouseHook();
    a.installNativeEventFilter(&mainwindow);
    return a.exec();
}
