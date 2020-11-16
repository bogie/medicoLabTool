#include "viewselectordialog.h"
#include "ui_viewselectordialog.h"

ViewSelectorDialog::ViewSelectorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewSelectorDialog)
{
    ui->setupUi(this);
    w = new MainWindow(this);

    //w->hide();
    r = new RegisterView(this);
    //r->hide();
}

ViewSelectorDialog::~ViewSelectorDialog()
{
    delete ui;
}

void ViewSelectorDialog::on_registerViewButton_clicked()
{
    r->show();
}

void ViewSelectorDialog::on_followupViewButton_clicked()
{
    w->show();
}
