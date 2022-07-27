#include "mainwindow.h"
#include "ui_mainwindow.h"

Translator tran;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tran.mainwindow=this;
    tran.ui=ui;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Translation_clicked()
{
    tran.Tranlate_front();
}


void MainWindow::on_pushButton_clicked()
{
    QString left=ui->label->text();
    QString right=ui->label_2->text();
    ui->label->setText(right);
    ui->label_2->setText(left);
}

