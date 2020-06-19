#include "registerview.h"
#include "ui_registerview.h"

RegisterView::RegisterView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterView)
{
    ui->setupUi(this);

    indexes["LEUK"] = 0;
    indexes["9HB"] = 1;
    indexes["THRO"] = 2;
    indexes["LYMR"] = 3;
    indexes["LYMM"] = 3;
    indexes["EOSR"] = 4;
    indexes["EOSM"] =4;
    indexes["INR"] = 5;
    indexes["PTT"] = 6;
    indexes["DD"] = 7;
    indexes["TZ"] = 8;
    indexes["NA"] = 9;
    indexes["K"] = 10;
    indexes["9ALB"] = 11;
    indexes["9ALBO"] = 11;
    indexes["TBIL"] = 12;
    indexes["DBIL"] = 13;
    indexes["AST37"] = 14;
    indexes["ALT37"] = 15;
    indexes["GGT37"] = 16;
    indexes["AP37"] = 17;
    indexes["LDH37"] = 18;
    indexes["CK37"] = 19;
    indexes["CKMB"] = 20;
    indexes["TNTHS"] = 21;
    indexes["NTpBNP2"] = 22;
    indexes["HST"] = 23;
    indexes["KREA"] = 24;
    indexes["CRP"] = 25;
    indexes["PCTKM"] = 26;
    indexes["FERR"] = 27;
    // VBG
    indexes["PHV"] = 28;
    indexes["PCO2V"] = 29;
    indexes["HCO3V"] = 30;
    indexes["SBEV"] = 31;
    indexes["PO2V"] = 32;
    indexes["SO2V"] = 33;
    indexes["METHn"] = 34;
    indexes["COHBn"] = 35;
    // ABG
    indexes["POC_CNA+"] = 9;
    indexes["POC_CK+"] = 10;
    indexes["POC_PH_T"] = 28;
    indexes["POC_PCO2_T"] = 29;
    indexes["POC_HCO3"] = 30;
    indexes["POC_ABE"] = 31;
    indexes["POC_PO2_T"] = 32;
    indexes["POC_SO2"] = 33;
    indexes["POC_FMETHB"] = 34;
    indexes["POC_FCOHGB"] = 35;
    // no _T
    indexes["POC_PH"] = 28;
    indexes["POC_PCO2"] = 29;
    indexes["POC_PO2"] = 32;
    //
    indexes["MYO"] = 36;
    indexes["IL2R"] = 37;
    indexes["IL6KE"] = 38;
    indexes["TNFA"] = 39;
    indexes["BC19A"] = 40;
    indexes["BC19"] = 41;
    indexes["TC3A"] = 42;
    indexes["TC3"] = 43;
    indexes["HC34A"] = 44;
    indexes["HC34"] = 45;
    indexes["SZC38A"] = 46;
    indexes["SZC38"] = 47;
    indexes["QC3438"] = 48;
    indexes["N1656A"] = 49;
    indexes["N1656"] = 50;
    indexes["ALSP"] = 51;
    indexes["KRSP"] = 52;
    indexes["9ACEYE"] = 53;
    indexes["POC_CLAC"] = 54;

    indexes["GLUC"] = 55;
    indexes["POC_CGLU"] = 55;
    indexes["POC_GLU_HEM"] = 55;

    rfIdxs["9HBA1CM"] = 0;
    rfIdxs["LDLC"] = 1;
    rfIdxs["HDLC"] = 2;
    rfIdxs["TRIG"] = 3;
    rfIdxs["CHOL"] = 4;
    rfIdxs["TSHKK"] = 5;
    rfIdxs["9FT3KK"] = 6;
    rfIdxs["9FT4KK"] = 7;
}

RegisterView::~RegisterView()
{
    delete ui;
}

void RegisterView::on_registerTable_customContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem* item = ui->registerTable->itemAt(pos);
    if(ui->registerTable->rowCount()==0) {
        return;
    }

    QMenu *menu = new QMenu(this);

    QAction *setCellValue = new QAction("Wert übernehmen ✓",this);
    setCellValue->setStatusTip("Übernimmt den Wert in die erste Zeile");
    connect(setCellValue, SIGNAL(triggered()), this, SLOT(on_setCellValue()));
    /*QAction *editCell = new QAction("Wert editieren ✍", this);
    connect(editCell, &QAction::triggered,this,
            [=]() {
        if(!item){
            QTableWidgetItem* it = new QTableWidgetItem("");
            it->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(ui->tableWidget->rowAt(pos.y()),ui->tableWidget->columnAt(pos.x()), it);
            ui->tableWidget->editItem(it);
        } else
            ui->tableWidget->editItem(item);
    });*/

    QAction *copyRow = new QAction("Ganze Zeile kopieren📋",this);
    connect(copyRow, &QAction::triggered, this,
            [=]() { int row = ui->registerTable->rowAt(pos.y()); on_copyRow(row); });
    if(item == nullptr){
        qDebug() << " no item at this position";
    } else {
        menu->addAction(setCellValue);

    }

    //menu->addAction(editCell);
    menu->addSeparator();
    menu->addAction(copyRow);

    if(ui->registerTable->selectedItems().length()>1) {
        QAction *copySelection = new QAction("Auswahl kopieren");
        connect(copySelection, &QAction::triggered, this, [=]() {
            on_copySelection();
        });
        menu->addSeparator();
        menu->addAction(copySelection);
    }

    QAction *deleteRow = new QAction("Ganze Zeile löschen");
    connect(deleteRow, &QAction::triggered, this, [=]() {
        if(ui->registerTable->selectedRanges().length()>0) {
            QList<QTableWidgetSelectionRange> ranges = ui->registerTable->selectedRanges();
            qDebug() << "got ranges length: " << ranges.length();
            for(int i = ranges.length(); i>0; i--) {
                QTableWidgetSelectionRange range = ranges.at(i-1);
                if((range.bottomRow()-range.topRow()) >= 1) {
                    for(int r = range.bottomRow(); r >= range.topRow(); r--) {
                        qDebug() << "removing multi row: " << r;
                        ui->registerTable->removeRow(r);
                    }
                } else {
                    qDebug() << "removing single row: " << range.bottomRow();
                    ui->registerTable->removeRow(range.bottomRow());
                }
            }
        }
    });
    menu->addSeparator();
    menu->addAction(deleteRow);

    menu->popup(ui->registerTable->viewport()->mapToGlobal(pos));
}

void RegisterView::on_loadRegister_clicked()
{
    QString data = ui->rawLab->toPlainText();
    if(data.length()==0)
        return;
    QStringList lines = data.split(QRegExp("[\n]"),QString::SkipEmptyParts);
    int lastRow = ui->registerTable->rowCount();
    qDebug() << "rowCount before insert is: " << lastRow;
    ui->registerTable->insertRow(lastRow);
    qDebug() << "rowCount after insert is: " << ui->registerTable->rowCount();
    bool itemsAdded = false;

    foreach(QString line, lines) {
        LabValue lab = LabValue(line,false);
        if(lab.success == false)
            continue;

        int idx = indexes.value(lab.param,-1);
        if(idx>=0) {
            qDebug() << "Adding " << lab.param << " at index: " << idx;
            QString value = lab.getValue(ui->delimiterCombo->currentIndex());
            QString prettyValue = value;
            if(value != "entfällt" & value != "s.Bem")
                prettyValue += lab.unit;
            QTableWidgetItem* item = new QTableWidgetItem(prettyValue);
            item->setData(256,value);
            item->setTextAlignment(Qt::AlignCenter);
            item->setToolTip(lab.comment);
            /*if(lab.comment.length()>0) {
                QIcon icon = QIcon(":/icons/warning.jpg");
                item->setIcon(icon);
            }*/
            if(lab.flag=="+")
                item->setBackgroundColor(QColor(Qt::GlobalColor::red));
            else if(lab.flag=="-")
                item->setBackgroundColor(QColor(Qt::GlobalColor::blue));
            ui->registerTable->setItem(lastRow,idx,item);
            itemsAdded = true;
        } else if(rfIdxs.contains(lab.param)) {
            int row = rfIdxs.value(lab.param,-1);
            if(ui->rfWidget->item(row,0) == nullptr){
                QString value = lab.getValue(ui->delimiterCombo->currentIndex());
                QString prettyValue = value;
                if(value != "entfällt" & value != "s.Bem")
                    prettyValue += lab.unit;
                qDebug() << "adding new rfWidget item for row: " << row << " parameter: " << lab.param << " and value: " << value;
                QTableWidgetItem *item = new QTableWidgetItem(prettyValue);
                item->setToolTip(lab.comment);
                if(lab.comment.length()>0)
                    item->setBackgroundColor(QColor(Qt::GlobalColor::lightGray));
                if(lab.flag=="+")
                    item->setBackgroundColor(QColor(Qt::GlobalColor::red));
                else if(lab.flag=="-")
                    item->setBackgroundColor(QColor(Qt::GlobalColor::blue));
                item->setTextAlignment(Qt::AlignCenter);
                item->setData(256,value);
                ui->rfWidget->setItem(row,0,item);
            }
        }
    }

    if(!itemsAdded)
        ui->registerTable->removeRow(lastRow);

    ui->rawLab->clear();
}

void RegisterView::on_copyRF()
{
    QByteArray items;
    for(int i=0; i<ui->rfWidget->rowCount(); i++) {
        QTableWidgetItem* item = ui->rfWidget->item(i,0);
        if(item == nullptr) {
            // skip
            items.append("");
        } else {
            items.append(item->data(256).toString());
        }
        if(i< ui->rfWidget->rowCount()-1)
            items.append("\n");
    }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData("text/plain",items);
    QApplication::clipboard()->setMimeData(mimeData);
}

void RegisterView::on_rfWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);

    QAction *copyRF = new QAction("Risikofaktoren kopieren📋",this);
    connect(copyRF, &QAction::triggered, this, &RegisterView::on_copyRF);

    menu->addAction(copyRF);
    menu->popup(ui->rfWidget->viewport()->mapToGlobal(pos));

}

void RegisterView::on_setCellValue()
{
    QList<QTableWidgetItem*> items = ui->registerTable->selectedItems();

    foreach(QTableWidgetItem* item, items) {
        qDebug() << "received SIGNAL for item: " << item->text() << " at row: " << item->row() << " and column: " << item->column();
        int column = item->column();
        QTableWidgetItem* target = ui->registerTable->takeItem(0,item->column());
        delete target;
        ui->registerTable->takeItem(item->row(),item->column());
        ui->registerTable->setItem(0,column,item);
    }
}

void RegisterView::on_copyRow(int row)
{
    QByteArray items;
    //QTableWidgetItem* item = ui->tableWidget->selectedItems().at(0);
    for(int i = 0; i< ui->registerTable->columnCount(); i++) {

        QTableWidgetItem* t = ui->registerTable->item(row,i);
        if(t == nullptr) {
            //qDebug() << "t is null!";
            items.append("");
        } else {
            items.append(t->data(256).toString());
        }
        if(i< ui->registerTable->columnCount()-1)
            items.append("\t");
    }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData("text/plain",items);
    QApplication::clipboard()->setMimeData(mimeData);
}

void RegisterView::on_copySelection()
{
    QByteArray items;
    QList<QTableWidgetItem*> selected = ui->registerTable->selectedItems();
    QList<QTableWidgetSelectionRange> ranges = ui->registerTable->selectedRanges();
    foreach(QTableWidgetItem* item, selected) {
        if(item == nullptr) {
            items.append("");
            return;
        } else {
            items.append(item->data(256).toString());
        }
        items.append("\t");
    }
    if(items.endsWith("\t")){
        items.remove(items.lastIndexOf("\t"),1);
    }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData("text/plain",items);
    QApplication::clipboard()->setMimeData(mimeData);
}

void RegisterView::on_registerTable_cellDoubleClicked(int row, int column)
{
    QTableWidgetItem* item = ui->registerTable->takeItem(row,column);
    if(item == nullptr) {
        qDebug() << "cellDoubleClicked: item is null at row: " << row << " and column: " << column;
        return;
    }
    ui->registerTable->takeItem(0,column);
    ui->registerTable->setItem(0,column,item);
}

void RegisterView::on_clearPatient_clicked()
{
    for(int i = ui->registerTable->rowCount(); i >=0; i--){
        ui->registerTable->removeRow(i);
    }

    ui->rfWidget->clearContents();
    /*for(int i = ui->rfWidget->rowCount(); i >= 0; i--) {
        ui->rfWidget->removeRow(i);
    }*/

    ui->rawLab->clear();
}
