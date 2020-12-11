#include "registerview.h"
#include "ui_registerview.h"

RegisterView::RegisterView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterView)
{
    ui->setupUi(this);

    mergingLabs = false;
    indexes["LEUK"] = 0;
    indexes["9HB"] = 1;
    indexes["THRO"] = 2;
    indexes["LYMR"] = 3;
    indexes["LYMM"] = 3;
    indexes["EOSR"] = 4;
    indexes["EOSM"] =4;
    indexes["INR"] = 5;
    indexes["PTT"] = 6;
    indexes["AT3"] = 7;
    indexes["9FIB"] = 8;
    indexes["DD"] = 9;
    indexes["TZ"] = 10;
    indexes["NA"] = indexes["POC_CNA+"] = 11;
    indexes["K"] = indexes["POC_CK+"] = 12;
    indexes["PHOS"] = 13;
    indexes["9ALB"] = indexes["9ALBO"] = 14;
    indexes["TBIL"] = 15;
    indexes["DBIL"] = 16;
    indexes["AST37"] = 17;
    indexes["ALT37"] = 18;
    indexes["GGT37"] = 19;
    indexes["AP37"] = 20;
    indexes["LDH37"] = 21;
    indexes["CK37"] = 22;
    indexes["CKMB"] = 23;
    indexes["TNTHS"] = 24;
    indexes["NTpBNP2"] = 25;
    indexes["HST"] = 26;
    indexes["KREA"] = 27;
    indexes["CRP"] = 28;
    indexes["PCTKM"] = 29;
    indexes["FERR"] = 30;
    // VBG
    indexes["PHV"] = indexes["POC_PH_T"] = indexes["POC_PH"] = 31;
    indexes["PCO2V"] = indexes["POC_PCO2_T"] = indexes["POC_PCO2"] = 32;
    indexes["HCO3V"] = indexes["POC_HCO3"] = 33;
    indexes["SBEV"] = indexes["POC_ABE"] = 34;
    indexes["PO2V"] = indexes["POC_PO2_T"] = indexes["POC_PO2"] = 35;
    indexes["SO2V"] = indexes["POC_SO2"] = 36;
    indexes["METHn"] = indexes["POC_FMETHB"] = 37;
    indexes["COHBn"] = indexes["POC_FCOHGB"] = 38;
    indexes["POC_CLAC"] = indexes["LACT"] = 39;

    indexes["CYSCK"] = 40;
    indexes["MYO"] = 41;
    indexes["AVP"] = 42;
    indexes["9ADSTi"] = 43;
    indexes["IL2R"] = 44;
    indexes["IL6KE"] = 45;
    indexes["TNFA"] = 46;
    indexes["BC19A"] = 47;
    indexes["BC19"] = 48;
    indexes["TC3A"] = 49;
    indexes["TC3"] = 50;
    indexes["HC34A"] = 51;
    indexes["HC34"] = 52;
    indexes["SZC38A"] = 53;
    indexes["SZC38"] = 54;
    indexes["QC3438"] = 55;
    indexes["N1656A"] = 56;
    indexes["N1656"] = 57;
    indexes["ALSP"] = 58;
    indexes["KRSP"] = 59;
    indexes["9ACEYE"] = 60;

    indexes["GLUC"] = indexes["POC_CGLU"] = indexes["POC_GLU_HEM"] = 61;

    rfIdxs["9HBA1CM"] = 0;
    rfIdxs["LDLC"] = 1;
    rfIdxs["HDLC"] = 2;
    rfIdxs["TRIG"] = 3;
    rfIdxs["CHOL"] = 4;
    rfIdxs["99LPAK"] = 5;
    rfIdxs["TSHKK"] = 6;
    rfIdxs["9FT3KK"] = 7;
    rfIdxs["9FT4KK"] = 8;
    rfIdxs["VD25T"] = 9;
    rfIdxs["VD125"] = 10;
    rfIdxs["9FE"] = 11;
    rfIdxs["TRAF"] = 12;
    rfIdxs["TRAFS"] = 13;

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
    urineIndexes["URUN"] = 13;
    urineIndexes["UZYLA"] = 14;
    urineIndexes["ZYLUF"] = 15;
    urineIndexes["9COND"] = 16;
    urineIndexes["9BAKTUF"] = 17;
    urineIndexes["KRSTUF"] = 18;
    urineIndexes["UHEFE"] = 19;
}

RegisterView::~RegisterView()
{
    this->parentWidget()->show();
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
            mergingLabs = true;
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
            mergingLabs = false;
        }
    });
    menu->addSeparator();
    menu->addAction(deleteRow);

    menu->popup(ui->registerTable->viewport()->mapToGlobal(pos));
}

void RegisterView::on_urineTable_customContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem* item = ui->urineTable->itemAt(pos);
    if(ui->urineTable->rowCount()==0) {
        return;
    }

    QMenu *menu = new QMenu(this);

    QAction *copyRow = new QAction("Ganze Zeile kopieren📋",this);
    connect(copyRow, &QAction::triggered, this,
            [=]() { int row = ui->urineTable->rowAt(pos.y()); on_copyUrineRow(row); });
    if(item == nullptr){
        qDebug() << " no item at this position";
    }

    //menu->addAction(editCell);
    menu->addSeparator();
    menu->addAction(copyRow);


    QAction *deleteRow = new QAction("Ganze Zeile löschen");
    connect(deleteRow, &QAction::triggered, this, [=]() {
        if(ui->urineTable->selectedRanges().length()>0) {
            QList<QTableWidgetSelectionRange> ranges = ui->urineTable->selectedRanges();
            qDebug() << "got ranges length: " << ranges.length();
            for(int i = ranges.length(); i>0; i--) {
                QTableWidgetSelectionRange range = ranges.at(i-1);
                if((range.bottomRow()-range.topRow()) >= 1) {
                    for(int r = range.bottomRow(); r >= range.topRow(); r--) {
                        qDebug() << "removing multi row: " << r;
                        ui->urineTable->removeRow(r);
                    }
                } else {
                    qDebug() << "removing single row: " << range.bottomRow();
                    ui->urineTable->removeRow(range.bottomRow());
                }
            }
        }
    });
    menu->addSeparator();
    menu->addAction(deleteRow);

    menu->popup(ui->urineTable->viewport()->mapToGlobal(pos));
}

void RegisterView::on_loadCumulative_clicked()
{
    QString data = ui->rawLab->toPlainText();
    if(data.length()==0)
        return;
    QStringList strList = data.split(QRegularExpression("[\n]"),Qt::SkipEmptyParts);
    int labCount = strList.at(0).split("\t").length();
    qDebug() << "multi has: " << labCount-3 << " labs";
    int curRow = ui->registerTable->rowCount();
    ui->registerTable->setRowCount(ui->registerTable->rowCount()+labCount);
    ui->urineTable->setRowCount(ui->urineTable->rowCount()+labCount);

    foreach(QString str, strList) {
        QStringList split = str.split("\t");
        QString p = split[0];
        QString paramName = split[1];
        QString unit = split[2];
        QString refRange = split[3];
        int lIdx = indexes.value(p, -1);
        int rfIdx = rfIdxs.value(p, -1);
        int uIdx = urineIndexes.value(p, -1);
        if(lIdx == -1 && rfIdx == -1 && uIdx == -1) {
            qDebug() << "paramter is not tracked: " << p;
            continue;
        }
        for(int i=4;i<split.length();i++) {
            LabValue lab = LabValue(p, paramName, unit, refRange, split[i]);
            if(lab.success) {
                qDebug() << "found lab value: " << lab.paramName << " and value: " << lab.value;

                if(rfIdx>=0) {
                    // add to rfTable
                    int row = rfIdxs.value(lab.param,-1);
                    if(ui->rfWidget->item(row,0) == nullptr){
                        QString value = lab.getValue(ui->delimiterCombo->currentIndex());
                        QString prettyValue = value;
                        if(!lab.isPassthrough)
                            prettyValue += lab.unit;
                        qDebug() << "adding new rfWidget item for row: " << row << " parameter: " << lab.param << " and value: " << value;
                        QTableWidgetItem *item = new QTableWidgetItem(prettyValue);
                        item->setToolTip(lab.comment);
                        if(lab.comment.length()>0)
                            item->setBackground(QColor(Qt::GlobalColor::lightGray));
                        if(lab.flag=="+")
                            item->setBackground(QColor(Qt::GlobalColor::red));
                        else if(lab.flag=="-")
                            item->setBackground(QColor(Qt::GlobalColor::blue));
                        item->setTextAlignment(Qt::AlignCenter);
                        item->setData(256,value);
                        ui->rfWidget->setItem(row,0,item);
                    }
                }
                if(lIdx>=0) {
                    qDebug() << "adding parameter " << p << " to laboratory table at idx: " << lIdx;
                    QString value = lab.getValue(ui->delimiterCombo->currentIndex());
                    QString prettyValue = value;
                    if(!lab.isPassthrough)
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
                        item->setBackground(QColor(Qt::GlobalColor::red));
                    else if(lab.flag=="-")
                        item->setBackground(QColor(Qt::GlobalColor::blue));
                    ui->registerTable->setItem(curRow,lIdx,item);
                }

                if(uIdx>=0) {
                    qDebug() << "adding parameter " << p << " to urine table at idx: " << lIdx;
                    QString value = lab.getValue(ui->delimiterCombo->currentIndex());
                    QString prettyValue = value;
                    if(!lab.isPassthrough)
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
                        item->setBackground(QColor(Qt::GlobalColor::red));
                    else if(lab.flag=="-")
                        item->setBackground(QColor(Qt::GlobalColor::blue));
                    if(uIdx == 13) {
                        QTableWidgetItem* oldItem = ui->urineTable->item(curRow,uIdx);
                        if(oldItem) {
                            qDebug() << "old item has data: " << oldItem->data(256).toString();
                            if(oldItem->data(256).toString() == "s.u.")
                                ui->urineTable->setItem(curRow, uIdx,item);
                        } else {
                            ui->urineTable->setItem(curRow, uIdx,item);
                        }
                    } else
                        ui->urineTable->setItem(curRow,uIdx,item);
                }
            }
            curRow++;
        }
        curRow = ui->registerTable->rowCount()-labCount;
    }
    for(int r = ui->registerTable->rowCount()-1; r>=0; r-- ) {
        bool hasValue = false;
        for(int c = 0; c< ui->registerTable->columnCount(); c++) {
            QTableWidgetItem* item = ui->registerTable->item(r,c);
            if(item != nullptr){
                hasValue = true;
            }
        }
        if(!hasValue){
            qDebug() << "removing row: " << r;
            ui->registerTable->removeRow(r);
        }
    }

    for(int r = ui->urineTable->rowCount()-1; r>=0; r--) {
        bool hasValue = false;
        for(int c = 0; c< ui->urineTable->columnCount(); c++) {
            QTableWidgetItem* item = ui->urineTable->item(r,c);
            if(item != nullptr){
                hasValue = true;
            }
        }
        if(!hasValue){
            qDebug() << "removing row: " << r;
            ui->urineTable->removeRow(r);
        }
    }
    ui->rawLab->clear();
}

void RegisterView::on_loadRegister_clicked()
{
    QString data = ui->rawLab->toPlainText();
    if(data.length()==0)
        return;
    QStringList lines = data.split(QRegularExpression("[\n]"),Qt::SkipEmptyParts);
    int lastRow = ui->registerTable->rowCount();
    int urineRow = ui->urineTable->rowCount();
    qDebug() << "rowCount before insert is: " << lastRow;
    ui->registerTable->insertRow(lastRow);
    ui->urineTable->insertRow(urineRow);
    qDebug() << "rowCount after insert is: " << ui->registerTable->rowCount();
    bool itemsAdded = false;
    bool urineItemAdded = false;

    foreach(QString line, lines) {
        LabValue lab = LabValue(line);
        if(lab.success == false)
            continue;

        int idx = indexes.value(lab.param,-1);
        if(idx>=0) {
            qDebug() << "Adding " << lab.param << " at index: " << idx;
            QString value = lab.getValue(ui->delimiterCombo->currentIndex());
            QString prettyValue = value;
            if(!lab.isPassthrough)
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
                item->setBackground(QColor(Qt::GlobalColor::red));
            else if(lab.flag=="-")
                item->setBackground(QColor(Qt::GlobalColor::blue));
            ui->registerTable->setItem(lastRow,idx,item);
            itemsAdded = true;
        } else if(rfIdxs.contains(lab.param)) {
            int row = rfIdxs.value(lab.param,-1);
            if(ui->rfWidget->item(row,0) == nullptr){
                QString value = lab.getValue(ui->delimiterCombo->currentIndex());
                QString prettyValue = value;
                if(!lab.isPassthrough)
                    prettyValue += lab.unit;
                qDebug() << "adding new rfWidget item for row: " << row << " parameter: " << lab.param << " and value: " << value;
                QTableWidgetItem *item = new QTableWidgetItem(prettyValue);
                item->setToolTip(lab.comment);
                if(lab.comment.length()>0)
                    item->setBackground(QColor(Qt::GlobalColor::lightGray));
                if(lab.flag=="+")
                    item->setBackground(QColor(Qt::GlobalColor::red));
                else if(lab.flag=="-")
                    item->setBackground(QColor(Qt::GlobalColor::blue));
                item->setTextAlignment(Qt::AlignCenter);
                item->setData(256,value);
                ui->rfWidget->setItem(row,0,item);
            }
        }
        if(urineIndexes.contains(lab.param)) {
            int uIdx = urineIndexes.value(lab.param,-1);
            if(uIdx>=0) {
                qDebug() << "Adding " << lab.param << " at index: " << uIdx;
                QString value = lab.getValue(ui->delimiterCombo->currentIndex());
                QString prettyValue = value;
                if(!lab.isPassthrough)
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
                    item->setBackground(QColor(Qt::GlobalColor::red));
                else if(lab.flag=="-")
                    item->setBackground(QColor(Qt::GlobalColor::blue));
                if(uIdx == 13) {
                    QTableWidgetItem* oldItem = ui->urineTable->item(urineRow,uIdx);
                    if(oldItem) {
                        qDebug() << "old item has data: " << oldItem->data(256).toString();
                        if(oldItem->data(256).toString() == "s.u.")
                            ui->urineTable->setItem(urineRow, uIdx,item);
                    } else {
                        ui->urineTable->setItem(urineRow, uIdx,item);
                    }
                } else
                    ui->urineTable->setItem(urineRow,uIdx,item);
                urineItemAdded = true;
            }
        }
    }

    if(!itemsAdded)
        ui->registerTable->removeRow(lastRow);
    if(!urineItemAdded)
        ui->urineTable->removeRow(urineRow);

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
            items.append(item->data(256).toString().toUtf8());
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
        if(item->row() == 0)
            continue;
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
            items.append(t->data(256).toString().toUtf8());
        }
        if(i< ui->registerTable->columnCount()-1)
            items.append("\t");
    }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData("text/plain",items);
    QApplication::clipboard()->setMimeData(mimeData);
}

void RegisterView::on_copyUrineRow(int row)
{
    QByteArray items;
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

void RegisterView::on_copySelection()
{
    QByteArray items;
    QList<QTableWidgetItem*> selected = ui->registerTable->selectedItems();
    QList<QTableWidgetSelectionRange> ranges = ui->registerTable->selectedRanges();

    if(ranges.length()>1) {
        qDebug() << "only one range supported";
    }
    QTableWidgetSelectionRange range = ranges.at(0);
    qDebug() << "selection range has rows: " << range.rowCount() << " and columns: " << range.columnCount();
    for(int i=0; i<range.rowCount(); i++) {
        int row = i + range.topRow();
        for(int j =0; j<range.columnCount(); j++) {
            int column = j + range.leftColumn();
            QTableWidgetItem* item = ui->registerTable->item(row,column);
            if(!item) {
                items.append("");
            } else {
                items.append(item->data(256).toString().toUtf8());
            }
            if(j+1<range.columnCount())
                items.append("\t");
        }

        if(i+1<range.rowCount())
            items.append("\n");
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
    QTableWidgetItem* oldItem = ui->registerTable->takeItem(0,column);
    delete oldItem;
    ui->registerTable->setItem(0,column,item);
}

void RegisterView::on_clearPatient_clicked()
{
    for(int i = ui->registerTable->rowCount(); i >=0; i--){
        ui->registerTable->removeRow(i);
    }

    for(int i = ui->urineTable->rowCount(); i >=0; i--){
        ui->urineTable->removeRow(i);
    }

    ui->rfWidget->clearContents();
    ui->urineTable->clearContents();
    ui->rawLab->clear();
}

void RegisterView::on_copyAllLabs_clicked()
{
    QByteArray items;
    qDebug() << "onCopy: roWcount is: " << ui->registerTable->rowCount();
    for(int r = 0; r < ui->registerTable->rowCount(); r++) {
        if(ui->registerTable->verticalHeaderItem(r) != nullptr){
            items.append(ui->registerTable->verticalHeaderItem(r)->text().toUtf8());
            items.append("\t\t");
        }

        for(int c = 0; c < ui->registerTable->columnCount(); c++) {
            QTableWidgetItem* t = ui->registerTable->item(r,c);
            if(t == nullptr) {
                //qDebug() << "t is null!";
                items.append("");
            } else {
                items.append(t->data(256).toString().toUtf8());
            }
            if(c < ui->registerTable->columnCount()-1)
                items.append("\t");
        }
        if(r< ui->registerTable->rowCount()-1)
            items.append("\n");
    }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData("text/plain",items);
    QApplication::clipboard()->setMimeData(mimeData);
}

void RegisterView::on_registerTable_itemSelectionChanged()
{
    qDebug() << "Item selection has changed";
    QList<QTableWidgetSelectionRange> ranges = ui->registerTable->selectedRanges();
    if(ranges.length()==0)
        return;
    QTableWidgetSelectionRange range = ranges.at(0);
    if(range.rowCount()>1 && !mergingLabs) {
        qDebug() << "Allow merge!";
        QMessageBox box;
        box.setText("Sollen die ausgewählten Zeilen kombiniert werden?");
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        box.setDefaultButton(QMessageBox::No);
        int ret = box.exec();

        QList<QString> failureCodes;
        failureCodes << "s. Bem" << "entfällt" << "Mat. fehlt" << "folgt";

        if(ret == QMessageBox::Yes) {
            mergingLabs = true;
            for(int c = 0; c < ui->registerTable->columnCount(); c++) {
                for(int r = range.topRow()+1; r <= range.bottomRow(); r++) {
                    qDebug() << "merging at row: " << r;
                    QTableWidgetItem* topItem = ui->registerTable->item(range.topRow(),c);
                    QTableWidgetItem* bottomItem = ui->registerTable->item(r,c);

                    if(topItem == nullptr || failureCodes.contains(topItem->data(256).toString())) {
                        if(bottomItem != nullptr) {
                            qDebug() << " bottomItem has text: " << bottomItem->text();
                            bottomItem = ui->registerTable->takeItem(r,c);
                            ui->registerTable->setItem(range.topRow(),c,bottomItem);
                            qDebug() << "new position: " << range.topRow() << " column: " << c;
                        }
                    }
                }
            }
            for(int i = range.bottomRow(); i >= range.topRow()+1; i--)
                ui->registerTable->removeRow(i);
            mergingLabs = false;
        }
    }
}
