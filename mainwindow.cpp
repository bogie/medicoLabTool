#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    rView = new RegisterView(this);
    fView = new FollowupView(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_registerViewButton_clicked()
{
    rView->show();
}

void MainWindow::on_followupViewButton_clicked()
{
    fView->show();
}

void MainWindow::on_actionProfile_triggered()
{
    LabProfileDialog lpd(this);
    lpd.exec();
}
