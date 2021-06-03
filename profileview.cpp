#include "profileview.h"
#include "ui_profileview.h"

ProfileView::ProfileView(QWidget *parent, QSettings *settings) :
    QMainWindow(parent),
    ui(new Ui::ProfileView),
    settings(settings)
{
    ui->setupUi(this);

    loadIndexes();
    qDebug() << "Created ProfileView for profilename: " << settings->value("profilename");
}

ProfileView::~ProfileView()
{
    delete ui;
}

void ProfileView::loadIndexes()
{
    QStringList order = settings->value("laborder").toStringList();
    if(order.length()>0) {
        settings->beginGroup("params");
        for(int i=0; i < order.length(); i++) {
            QString param = order.at(i);
            QStringList codes = settings->value(param).toStringList();
            qDebug() << "key: " << param << "loaded codes: " << codes;

            for(QString code : codes) {
                qDebug() << "added code: " << code << " at index: " << i;
                indexes.insert(code,i);
            }
        }
        settings->endGroup();

        ui->labTable->setColumnCount(order.length());
        ui->labTable->setHorizontalHeaderLabels(order);
    }
}

QByteArray ProfileView::localizeString(QString input) {
    QByteArray output;
    QRegularExpression re("^\\d");
    if(!re.match(input).hasMatch())
        output = input.toUtf8();
    else {
        if(ui->delimiterCombo->currentIndex()== 0){
            output = input.replace(".",",").toUtf8();
        } else {
            output = input.replace(",",".").toUtf8();
        }
    }
    return output;
}

void ProfileView::on_loadRawLab_clicked()
{
    QString data = ui->rawLab->toPlainText();
    if(data.length()==0)
        return;
    QStringList lines = data.split(QRegularExpression("[\n]"),Qt::SkipEmptyParts);
    int lastRow = ui->labTable->rowCount();
    qDebug() << "rowCount before insert is: " << lastRow;
    ui->labTable->insertRow(lastRow);
    qDebug() << "rowCount after insert is: " << ui->labTable->rowCount();
    bool itemsAdded = false;

    foreach(QString line, lines) {
        LabValue lab = LabValue(line);
        if(lab.success == false)
            continue;

        int idx = indexes.value(lab.param,-1);
        if(idx>=0) {
            qDebug() << "Adding " << lab.param << " at index: " << idx;
            QString value = lab.getValue();
            QString prettyValue = value;
            if(!lab.isPassthrough)
                prettyValue += lab.unit;
            QTableWidgetItem* item = new QTableWidgetItem(prettyValue);
            item->setData(256,value);
            item->setTextAlignment(Qt::AlignCenter);
            item->setToolTip(lab.comment);
            if(lab.flag=="+")
                item->setBackground(QColor(Qt::GlobalColor::red));
            else if(lab.flag=="-")
                item->setBackground(QColor(Qt::GlobalColor::blue));
            ui->labTable->setItem(lastRow,idx,item);
            itemsAdded = true;
        }
    }

    if(!itemsAdded)
        ui->labTable->removeRow(lastRow);

    ui->rawLab->clear();
}

void ProfileView::on_labTable_customContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem* item = ui->labTable->itemAt(pos);
    if(ui->labTable->rowCount()==0) {
        return;
    }

    QMenu *menu = new QMenu(this);

    QAction *copyRow = new QAction(tr("Copy line📋"),this);
    connect(copyRow, &QAction::triggered, this,
            [=]() { int row = ui->labTable->rowAt(pos.y()); on_copyRow(row); });
    if(item == nullptr){
        qDebug() << " no item at this position";
    }

    //menu->addAction(editCell);
    menu->addSeparator();
    menu->addAction(copyRow);


    QAction *deleteRow = new QAction(tr("Delete line"));
    connect(deleteRow, &QAction::triggered, this, [=]() {
        if(ui->labTable->selectedRanges().length()>0) {
            QList<QTableWidgetSelectionRange> ranges = ui->labTable->selectedRanges();
            qDebug() << "got ranges length: " << ranges.length();
            for(int i = ranges.length(); i>0; i--) {
                QTableWidgetSelectionRange range = ranges.at(i-1);
                if((range.bottomRow()-range.topRow()) >= 1) {
                    for(int r = range.bottomRow(); r >= range.topRow(); r--) {
                        qDebug() << "removing multi row: " << r;
                        ui->labTable->removeRow(r);
                    }
                } else {
                    qDebug() << "removing single row: " << range.bottomRow();
                    ui->labTable->removeRow(range.bottomRow());
                }
            }
        }
    });
    menu->addSeparator();
    menu->addAction(deleteRow);

    menu->popup(ui->labTable->viewport()->mapToGlobal(pos));
}

void ProfileView::on_clearAll_clicked()
{
    for(int i = ui->labTable->rowCount(); i >=0; i--){
        ui->labTable->removeRow(i);
    }

    ui->rawLab->clear();
}

void ProfileView::on_copyRow(int row)
{
    QByteArray items;
     for(int i = 0; i< ui->labTable->columnCount(); i++) {

         QTableWidgetItem* item = ui->labTable->item(row,i);
         if(item == nullptr) {
             items.append("");
         } else {
             items.append(localizeString(item->data(256).toString()));
         }
         if(i< ui->labTable->columnCount()-1)
             items.append("\t");
     }
     QMimeData *mimeData = new QMimeData();
     mimeData->setData("text/plain",items);
     QApplication::clipboard()->setMimeData(mimeData);

}

void ProfileView::on_copyAllLabs_clicked()
{
    QByteArray items;
    qDebug() << "onCopy: roWcount is: " << ui->labTable->rowCount();
    for(int r = 0; r < ui->labTable->rowCount(); r++) {
        if(ui->labTable->verticalHeaderItem(r) != nullptr){
            items.append(ui->labTable->verticalHeaderItem(r)->text().toUtf8());
            items.append("\t\t");
        }

        for(int c = 0; c < ui->labTable->columnCount(); c++) {
            QTableWidgetItem* item = ui->labTable->item(r,c);
            if(item == nullptr) {
                items.append("");
            } else {
                items.append(localizeString(item->data(256).toString()));
            }
            if(c < ui->labTable->columnCount()-1)
                items.append("\t");
        }
        if(r< ui->labTable->rowCount()-1)
            items.append("\n");
    }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData("text/plain",items);
    QApplication::clipboard()->setMimeData(mimeData);
}
