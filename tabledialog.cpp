#include "tabledialog.h"
#include "ui_tabledialog.h"

TableDialog::TableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Add/modify table"));
}

TableDialog::~TableDialog()
{
    delete ui;
}

TableDialogResult TableDialog::getResult()
{
    return result;
}

void TableDialog::setData(QString name, QString orientation)
{
    ui->nameEdit->setText(name);
    ui->orientationBox->setCurrentText(orientation);
}

void TableDialog::on_buttonBox_accepted()
{
    result = TableDialogResult(ui->nameEdit->text(),ui->orientationBox->currentText());
}

void TableDialog::on_nameEdit_textChanged(const QString &arg1)
{
    //result.name = ui->nameEdit->text();
}

void TableDialog::on_orientationBox_currentTextChanged(const QString &arg1)
{
    //result.orientation = ui->orientationBox->currentText();
}
