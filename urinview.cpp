#include "urinview.h"
#include "ui_urinview.h"

UrinView::UrinView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UrinView)
{
    ui->setupUi(this);

    urineIndexes = QMap<QString, int>();

    urineIndexes["SPGU"] = 0;
    urineIndexes["PHU"] = 1;
    urineIndexes["9LEUU"] = 2;
    urineIndexes["9HBU"] = 3;
    urineIndexes["9BILU"] = 4;
    urineIndexes["9UROU"] = 5;
    urineIndexes["9TPU"] = 6;
    urineIndexes["9KETU"] = 7;
    urineIndexes["9NITU"] = 8;
    urineIndexes["9GLUU"] = 9;
    urineIndexes["SERYUF"] = 10;
    urineIndexes["SLEUUF"] = 11;
    urineIndexes["PLEPUF"] = 12;
    urineIndexes["RUEPUF"] = 13;
    urineIndexes["UZYLA"] = 14;
    urineIndexes["ZYLUF"] = 15;
    urineIndexes["9COND"] = 16;
    urineIndexes["9BAKTUF"] = 17;
    urineIndexes["KRSTUF"] = 18;
    urineIndexes["UHEFE"] = 19;
}

UrinView::~UrinView()
{
    delete ui;
}

void UrinView::on_clearPatient_clicked()
{
    for(int i = ui->urineTable->rowCount(); i >= 0; i--) {
        ui->urineTable->removeRow(i);
    }

    ui->rawLab->clear();
}

void UrinView::on_loadUrine_clicked()
{
    QString data = ui ->rawLab->toPlainText();
    if(data.length() == 0)
        return;
    QStringList lines = data.split(QRegularExpression("[\n]"),Qt::SkipEmptyParts);

    int lastRow = ui->urineTable->rowCount();
    ui->urineTable->insertRow(lastRow);
    bool itemsAdded = false;

    foreach(QString line, lines) {
        LabValue lab = LabValue(line);
        qDebug() << "parsed lab: " << lab.paramName << " success: " << lab.success;
        if(lab.success == false)
            continue;

        int idx = urineIndexes.value(lab.param,-1);
        qDebug() << "loadUrine: index is: " << idx;
        if(idx>=0) {
            QString value = lab.getValue(ui->delimiterCombo->currentIndex());
            QString prettyValue = value;
            if(value != "entfällt" & value != "s.Bem")
                prettyValue += lab.unit;
            QTableWidgetItem* item = new QTableWidgetItem(prettyValue);
            item->setData(256,value);
            item->setTextAlignment(Qt::AlignCenter);
            item->setToolTip(lab.comment);
            if(lab.flag=="+")
                item->setBackground(QColor(Qt::GlobalColor::red));
            else if(lab.flag=="-")
                item->setBackground(QColor(Qt::GlobalColor::blue));
            ui->urineTable->setItem(lastRow,idx,item);
            itemsAdded = true;
        }
    }
    if(!itemsAdded)
        ui->urineTable->removeRow(lastRow);
    ui->rawLab->clear();
}

void UrinView::on_urineTable_customContextMenuRequested(const QPoint &pos)
{
    //QTableWidgetItem* item = ui->urineTable->itemAt(pos);
    if(ui->urineTable->rowCount()==0) {
        return;
    }

    QMenu * menu = new QMenu(this);

    QAction *copyRow = new QAction("Ganze Zeile kopieren📋",this);
    connect(copyRow, &QAction::triggered, this,
            [=]() { int row = ui->urineTable->rowAt(pos.y()); on_copy_row(row); });
    menu->addAction(copyRow);

    menu->popup(ui->urineTable->viewport()->mapToGlobal(pos));
}

void UrinView::on_copy_row(int row)
{
    QByteArray items;
    //QTableWidgetItem* item = ui->tableWidget->selectedItems().at(0);
    for(int i = 0; i< ui->urineTable->columnCount(); i++) {

        QTableWidgetItem* t = ui->urineTable->item(row,i);
        if(t == nullptr) {
            //qDebug() << "t is null!";
            items.append("");
        } else {
            items.append(t->data(256).toString().toUtf8());
        }
        if(i< ui->urineTable->columnCount()-1)
            items.append("\t");
    }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData("text/plain",items);
    QApplication::clipboard()->setMimeData(mimeData);

}
