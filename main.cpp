#include "mainwindow.h"
#include "delimit_window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Translator tran;
    MainWindow mainwindow;
    mainwindow.show();
    delimit_window dw;
    //    ::setMouseTracking(true);
        SetMouseHook();
    return a.exec();
}
