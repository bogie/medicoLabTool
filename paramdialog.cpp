#include "paramdialog.h"
#include "ui_paramdialog.h"

ParamDialog::ParamDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParamDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Add/modify laboratory parameter"));
}

ParamDialog::~ParamDialog()
{
    delete ui;
}

ParamDialogResult ParamDialog::getResult()
{
    return result;
}

void ParamDialog::setData(QString name, QStringList codes)
{
    ui->paramNameEdit->setText(name);
    ui->codesListWidget->addItems(codes);
}

void ParamDialog::on_codeEdit_returnPressed()
{

}

void ParamDialog::on_okButton_clicked()
{
    qDebug() << "ParamDialog: paramname is " << ui->paramNameEdit->text()
             << " codes list length is " << ui->codesListWidget->count();

    QStringList codesList;
    for(int i = 0; i < ui->codesListWidget->count(); i++) {
        QListWidgetItem* item = ui->codesListWidget->item(i);
        codesList.append(item->text());
    }
    result.name = ui->paramNameEdit->text();
    result.codes = codesList;
    accept();
}

void ParamDialog::on_cancelButton_clicked()
{
    reject();
}

void ParamDialog::on_addCodeButton_clicked()
{
    if(ui->codeEdit->text().isEmpty())
        return;
    ui->codesListWidget->addItem(ui->codeEdit->text());
    ui->codeEdit->clear();
}

void ParamDialog::on_codesListWidget_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem* item = ui->codesListWidget->itemAt(pos);
    QMenu *menu = new QMenu(this);
    QAction *removeItem = new QAction(tr("Delete laboratory code"),this);
    removeItem->setStatusTip(tr("Deletes the entry"));
    connect(removeItem, &QAction::triggered, this, [=]() {
        delete item;
    });
    menu->addAction(removeItem);

    menu->popup(ui->codesListWidget->viewport()->mapToGlobal(pos));
}
