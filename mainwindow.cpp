#include "mainwindow.h"
#include "ui_mainwindow.h"

Translator tran;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui_init(ui);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Translation_clicked()
{
    tran.TranslateByGoogle();
}

