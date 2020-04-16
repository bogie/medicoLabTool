#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->addItem("Komma");
    ui->comboBox->addItem("Punkt");
    ui->comboBox->setCurrentIndex(0);
    ui->tableWidget->setColumnCount(55);

    QStringList headers;
    headers << "Leukozyten" << "Haemoglobin" << "Thrombozyten" << "Lymphozyten" << "Eosinophile" << "INR" << "aPTT" << "D-Dimere" << "TZ" << "Natrium" << "Kalium" << "Albumin" << "Bilirubin(ges)"
            << "Bilirubin(dir)" << "AST" << "ALT" << "gGT" << "AP" << "LDH" << "CK" << "CK-MB" << "TroponinT" << "NTproBNP" << "Harnstoff" << "Kreatinin" << "CRP" << "Procalcitonin"
            << "Ferritin" << "pH(venös)" << "pCO2(venös)" << "Standardbicarbonat" << "Standard-Base-Excess" << "pO2(venös)" << "Sauerstoffsättigung(venös)" << "Methaemoglobin" << "CO-Haemoglobin"
            << "Myoglobin" << "sIL-2" << "IL-6" << "TNFalpha" << "B Lymphoz CD19+ abs" << "B Lymphoz CD19+ rel" << "T Lymph CD3+ abs" << "T Lymph CD3+ rel" << "T Helfer CD3+CD4+abs"
            << "T Helfer CD3+CD4+rel" << "T Suppr CD3+CD8+ abs" << "T Suppr CD3+CD8+ rel" << "Quot CD3 CD4/CD3 CD8" << "NK Zel CD16+/56+ abs" << "NK Zel CD16+/56+rel" << "Albumin(Urin)" << "Kreatinin(Urin)" << "ACE" << "cLac";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    indexes = QMap<QString, int>();
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

    rfIdxs["9HBA1CM"] = 0;
    rfIdxs["LDLC"] = 1;
    rfIdxs["HDLC"] = 2;
    rfIdxs["TRIG"] = 3;
    rfIdxs["CHOL"] = 4;
    rfIdxs["TSHKK"] = 5;
    rfIdxs["9FT3KK"] = 6;
    rfIdxs["9FT4KK"] = 7;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadLabButton_clicked()
{
    QString data = ui->textEdit->toPlainText();
    if(data.length()==0)
        return;
    QStringList strList = data.split(QRegExp("[\n]"),QString::SkipEmptyParts);
    int lastRow = ui->tableWidget->rowCount();
    qDebug() << "rowCount before insert is: " << lastRow;
    ui->tableWidget->insertRow(lastRow);
    qDebug() << "rowCount after insert is: " << ui->tableWidget->rowCount();
    bool itemsAdded = false;

    foreach(QString str, strList) {
        QStringList split = str.split("\t");
        if(split.length()<2)
            continue;
        QString p = split[0];
        QString v = parseValue(p,split[2]);

        if(v.length()>0) {
            int idx = indexes.value(p, -1);
            if(idx>=0) {
                qDebug() << "found idx: " << idx;
                QTableWidgetItem *item = new QTableWidgetItem(v);
                item->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(lastRow,idx,item);
                itemsAdded = true;
            }

            if(rfIdxs.contains(p)){
                loadRFsIntoTable(p,v);
            }

        }
    }

    if(!itemsAdded)
        ui->tableWidget->removeRow(lastRow);

    ui->textEdit->clear();
}

void MainWindow::on_loadMultiButton_clicked()
{
    QString data = ui->textEdit->toPlainText();
    if(data.length()==0)
        return;
    QStringList strList = data.split(QRegExp("[\n]"),QString::SkipEmptyParts);
    int labCount = strList.at(0).split("\t").length();
    qDebug() << "multi has: " << labCount-3 << " labs";
    int curRow = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+labCount);

    foreach(QString str, strList) {
        qDebug() << str;
        QStringList split = str.split("\t");
        QString p = split[0];
        for(int i=4;i<split.length();i++) {
            QString v = parseValue(p,split[i]);
            if(v.length()>0) {
                if(rfIdxs.contains(p)){
                    loadRFsIntoTable(p,v);
                }

                int idx = indexes.value(p, -1);
                if(idx>=0) {
                    qDebug() << "found idx: " << idx;
                    QTableWidgetItem *item = new QTableWidgetItem(v);
                    item->setTextAlignment(Qt::AlignCenter);
                    ui->tableWidget->setItem(curRow,idx,item);
                }
            }
            curRow++;
        }
        curRow = ui->tableWidget->rowCount()-labCount;
    }
    for(int r = ui->tableWidget->rowCount()-1; r>=0; r-- ) {
        bool hasValue = false;
        for(int c = 0; c< ui->tableWidget->columnCount(); c++) {
            QTableWidgetItem* item = ui->tableWidget->item(r,c);
            if(item != nullptr){
                hasValue = true;
            }
            /*if(item->text().length()>0){
                qDebug() << "row: " << r << " has non-empty value: " << item->text() << " at column: " << c;
                hasValue = true;
            }*/
        }
        if(!hasValue){
            qDebug() << "removing row: " << r;
            ui->tableWidget->removeRow(r);
        }
    }
    ui->textEdit->clear();
}

void MainWindow::on_copyLabButton_clicked()
{
    QByteArray items;
    qDebug() << "onCopy: roWcount is: " << ui->tableWidget->rowCount();
    for(int i = 0; i< ui->tableWidget->columnCount(); i++) {

        QTableWidgetItem* t = ui->tableWidget->item(0,i);
        if(t == nullptr) {
            //qDebug() << "t is null!";
            items.append("");
        } else {
            items.append(t->text());
        }
        if(i< ui->tableWidget->columnCount()-1)
            items.append("\t");
    }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData("text/plain",items);
    QApplication::clipboard()->setMimeData(mimeData);
}

void MainWindow::on_clearButton_clicked()
{
    for(int i = ui->tableWidget->rowCount(); i >=0; i--){
        ui->tableWidget->removeRow(i);
    }

    ui->rfWidget->clearContents();
    /*for(int i = ui->rfWidget->rowCount(); i >= 0; i--) {
        ui->rfWidget->removeRow(i);
    }*/

    ui->textEdit->clear();
}

// Context Menus

// tableWidget

void MainWindow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem* item = ui->tableWidget->itemAt(pos);
    if(ui->tableWidget->rowCount()==0) {
        return;
    }


    QMenu *menu = new QMenu(this);

    QAction *setCellValue = new QAction("Wert übernehmen ✓",this);
    setCellValue->setStatusTip("Übernimmt den Wert in die erste Zeile");
    connect(setCellValue, SIGNAL(triggered()), this, SLOT(on_setCellValue()));
    QAction *editCell = new QAction("Wert editieren ✍", this);
    connect(editCell, &QAction::triggered,this,
            [=]() {
        if(!item){
            QTableWidgetItem* it = new QTableWidgetItem("");
            it->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(ui->tableWidget->rowAt(pos.y()),ui->tableWidget->columnAt(pos.x()), it);
            ui->tableWidget->editItem(it);
        } else
            ui->tableWidget->editItem(item);
    });

    QAction *copyRow = new QAction("Ganze Zeile kopieren📋",this);
    connect(copyRow, &QAction::triggered, this,
            [=]() { int row = ui->tableWidget->rowAt(pos.y()); on_copyRow(row); });
    if(item == nullptr){
        qDebug() << " no item at this position";
    } else {
        menu->addAction(setCellValue);

    }

    menu->addAction(editCell);
    menu->addSeparator();
    menu->addAction(copyRow);

    if(ui->tableWidget->selectedItems().length()>1) {
        QAction *copySelection = new QAction("Auswahl kopieren");
        connect(copySelection, &QAction::triggered, this, [=]() {
            on_copySelection();
        });
        menu->addSeparator();
        menu->addAction(copySelection);
    }

    menu->popup(ui->tableWidget->viewport()->mapToGlobal(pos));
}

void MainWindow::on_setCellValue()
{
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();

    foreach(QTableWidgetItem* item, items) {
        qDebug() << "received SIGNAL for item: " << item->text() << " at row: " << item->row() << " and column: " << item->column();

        QTableWidgetItem* target = ui->tableWidget->item(0,item->column());
        if(target == nullptr) {
            target = new QTableWidgetItem(item->text());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(0,item->column(),target);
        } else
            target->setText(item->text());
        target->setTextAlignment(Qt::AlignCenter);
    }
}

void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
    QTableWidgetItem* item = ui->tableWidget->item(row,column);
    if(item == nullptr)
        return;

    QTableWidgetItem* target = ui->tableWidget->item(0,item->column());
    if(target == nullptr) {
        target = new QTableWidgetItem(item->text());
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(0,item->column(),target);
    } else
        target->setText(item->text());
    target->setTextAlignment(Qt::AlignCenter);
}

void MainWindow::on_copyRow(int row)
{
    QByteArray items;
    //QTableWidgetItem* item = ui->tableWidget->selectedItems().at(0);
    for(int i = 0; i< ui->tableWidget->columnCount(); i++) {

        QTableWidgetItem* t = ui->tableWidget->item(row,i);
        if(t == nullptr) {
            //qDebug() << "t is null!";
            items.append("");
        } else {
            items.append(t->text());
        }
        if(i< ui->tableWidget->columnCount()-1)
            items.append("\t");
    }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData("text/plain",items);
    QApplication::clipboard()->setMimeData(mimeData);
}

void MainWindow::on_copySelection()
{
    QByteArray items;
    QList<QTableWidgetItem*> selected = ui->tableWidget->selectedItems();
    QList<QTableWidgetSelectionRange> ranges = ui->tableWidget->selectedRanges();
    foreach(QTableWidgetItem* item, selected) {
        if(item == nullptr) {
            items.append("");
            return;
        } else {
            items.append(item->text());
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

// rfWidget
void MainWindow::on_rfWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);

    QAction *copyRF = new QAction("Risikofaktoren kopieren📋",this);
    connect(copyRF, &QAction::triggered, this, &MainWindow::on_copyRF);

    menu->addAction(copyRF);
    menu->popup(ui->rfWidget->viewport()->mapToGlobal(pos));
}

void MainWindow::on_copyRF()
{
    QByteArray items;
    for(int i=0; i<ui->rfWidget->rowCount(); i++) {
        QTableWidgetItem* item = ui->rfWidget->item(i,0);
        if(item == nullptr) {
            // skip
            items.append("");
        } else {
            items.append(item->text());
        }
        if(i< ui->rfWidget->rowCount()-1)
            items.append("\n");
    }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData("text/plain",items);
    QApplication::clipboard()->setMimeData(mimeData);
}

void MainWindow::loadRFsIntoTable(QString p, QString v)
{
    qDebug() << "loadRFsIntoTable: " << p << v;
    int row = rfIdxs.value(p,-1);
    if(row>=0) {
        if(ui->rfWidget->item(row,0) == nullptr){
            QTableWidgetItem *item = new QTableWidgetItem(v);
            item->setTextAlignment(Qt::AlignCenter);
            ui->rfWidget->setItem(row,0,item);
        }
    }
}

QString MainWindow::parseValue(QString p, QString v)
{
    QString output;
    if(v.contains(QRegExp("[+-]?([0-9]*[.])?[0-9]+")) || v == "folgt") {
        output = v.simplified().remove(" ");
        output = output.remove("<").remove(">").remove("+").remove("N").remove("?");
        if(output.endsWith("-"))
            output = v.remove("-");
        qDebug() << "p: " << p << "output: " << output;
        if(p == "9ALBO") {
            // mg/dl statt g/dl! FAKTOR 1000!
            double albo = output.toDouble();
            albo  /= 1000;
            qDebug() << "output is now: " << output;
            qDebug() << "albo is now: " << albo;
            output = QString::number(albo);
            qDebug() << "output is now: " << output;
        }
        if(ui->comboBox->currentIndex()== 0){
            output = output.replace(".",",");
        } else {
            output = output.replace(",",".");
        }
    } else
        output = "";
    return output;
}
