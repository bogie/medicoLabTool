#include "followupview.h"
#include "ui_followupview.h"

FollowupView::FollowupView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FollowupView)
{
    ui->setupUi(this);
    //this->showMaximized();

    uView = new UrinView();
    //rView = new RegisterView();

    QHeaderView* header = ui->tableWidget->verticalHeader();
    header->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(header, &QHeaderView::customContextMenuRequested, this, &FollowupView::on_verticalHeader_customContextMenuRequested);
    connect(header, &QHeaderView::sectionDoubleClicked,this, &FollowupView::on_verticalHeader_doubleClicked);
    ui->comboBox->addItem("Komma");
    ui->comboBox->addItem("Punkt");
    ui->comboBox->setCurrentIndex(0);


    QStringList headers;
    headers << "Leukozyten" << "Haemoglobin" << "Thrombozyten" << "Lymphozyten" << "Eosinophile"
            << "INR" << "aPTT" << "ATIII" << "Fibrinogen" << "D-Dimere" << "TZ"
            << "Natrium" << "Kalium" << "Phosphat"
            << "Albumin" << "Bilirubin(ges)" << "Bilirubin(dir)" << "AST" << "ALT" << "gGT" << "AP"
            << "LDH" << "CK" << "CK-MB" << "TroponinT" << "NTproBNP"
            << "Harnstoff" << "Kreatinin" << "CRP" << "Procalcitonin"
            << "Laktat" << "Eisen" << "Transferrinsättigung" << "Ferritin"
            << "Protein (qual)" << "DysErys" << "Kreatinin SpU" << "Harnstoff-Krea-Quotient SpU" << "Albumin SpU"
            << "pH" << "pCO2" << "Standardbicarbonat" << "Standard-Base-Excess" << "pO2" << "Sauerstoffsättigung" << "Methaemoglobin" << "CO-Haemoglobin"
            << "CystatinC" << "Myoglobin" << "Copeptin" << "Aldosteron" << "sIL-2" << "IL-6" << "TNFalpha"
            << "B Lymphoz CD19+ abs" << "B Lymphoz CD19+ rel" << "T Lymph CD3+ abs" << "T Lymph CD3+ rel" << "T Helfer CD3+CD4+abs" << "T Helfer CD3+CD4+rel"
            << "T Suppr CD3+CD8+ abs" << "T Suppr CD3+CD8+ rel" << "Quot CD3 CD4/CD3 CD8" << "NK Zel CD16+/56+ abs" << "NK Zel CD16+/56+rel"
            << "ACE" << "Glukose";

    ui->tableWidget->setColumnCount(headers.length());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    indexes = QMap<QString, int>();
    indexes["LEUK"] = 0;
    indexes["9HB"] = 1;
    indexes["THRO"] = 2;
    indexes["LYMR"] = 3;
    indexes["LYMM"] = 3;
    indexes["EOSR"] = 4;
    indexes["EOSM"] = 4;
    indexes["INR"] = 5;
    indexes["PTT"] = 6;
    indexes["AT3"] = 7;
    indexes["9FIB"] = 8;
    indexes["DD"] = 9;
    indexes["TZ"] = 10;
    indexes["NA"] = 11;
    indexes["K"] = 12;
    indexes["PHOS"] = 13;
    indexes["9ALB"] = 14;
    indexes["9ALBO"] = 14;
    indexes["TBIL"] = 15;
    indexes["DBIL"] = 16;
    indexes["AST37"] = 17;
    indexes["ALT37"] = 18;
    indexes["GGT37"] = 19;
    indexes["AP37"] = 20;
    indexes["LDH37"] = 21;
    indexes["CK37"] = 22;
    indexes["CKMB"] = 23;
    //indexes["CKMBMa"] = 23;
    indexes["TNTHS"] = 24;
    indexes["NTpBNP2"] = 25;
    indexes["HST"] = 26;
    indexes["KREA"] = 27;
    indexes["CRP"] = 28;
    indexes["PCTKM"] = 29;
    indexes["LACT"] = 30; // LDZ Laktat
    indexes["POC_CLAC"] = 30; // BGA Laktat
    indexes["9FE"] = 31;
    indexes["TRAFS"] = 32;
    indexes["FERR"] = 33;
    // Proteinurie
    indexes["9TPU"] = 34; // qualitativ
    indexes["TPSP"] = 34; // quantitativ
    indexes["DERY"] = 35;
    indexes["KRSP"] = 36;
    indexes["HSKRQ"] = 37;
    indexes["ALSP"] = 38;
    // VBG
    indexes["PHV"] = 39;
    indexes["PCO2V"] = 40;
    indexes["HCO3V"] = 41;
    indexes["SBEV"] = 42;
    indexes["PO2V"] = 43;
    indexes["SO2V"] = 44;
    indexes["METHn"] = 45;
    indexes["COHBn"] = 46;
    // ABG
    indexes["POC_CNA+"] = 11;
    indexes["POC_CK+"] = 12;
    indexes["POC_PH_T"] = 39;
    indexes["POC_PCO2_T"] = 40;
    indexes["POC_HCO3"] = 41;
    indexes["POC_ABE"] = 42;
    indexes["POC_PO2_T"] = 43;
    indexes["POC_SO2"] = 44;
    indexes["POC_FMETHB"] = 45;
    indexes["POC_FCOHGB"] = 46;
    // no _T
    indexes["POC_PH"] = 39;
    indexes["POC_PCO2"] = 40;
    indexes["POC_PO2"] = 43;
    //
    indexes["CYSCK"] = 47;
    indexes["MYO"] = 48;
    indexes["AVP"] = 49;
    indexes["9ADSTi"] = 50;
    indexes["IL2R"] = 51;
    indexes["IL6KE"] = 52;
    indexes["TNFA"] = 53;
    indexes["BC19A"] = 54;
    indexes["BC19"] = 55;
    indexes["TC3A"] = 56;
    indexes["TC3"] = 57;
    indexes["HC34A"] = 58;
    indexes["HC34"] = 59;
    indexes["SZC38A"] = 60;
    indexes["SZC38"] = 61;
    indexes["QC3438"] = 62;
    indexes["N1656A"] = 63;
    indexes["N1656"] = 64;
    indexes["9ACEYE"] = 65;


    // Glukose = LDZ, BGA, BZGerät
    indexes["GLUC"] = 66;
    indexes["POC_CGLU"] = 66;
    indexes["POC_GLU_HEM"] = 66;

    QStringList rfRowHeaders;
    rfRowHeaders << "HbA1C" << "LDL" << "HDL" << "Triglyceride" << "Cholesterin" << "Lp(a)" << "TSH"
                 << "fT3" << "fT4" << "25-OH-VitD3" << "1,25-OH-VitD3";
    ui->rfWidget->setRowCount(rfRowHeaders.length());
    ui->rfWidget->setVerticalHeaderLabels(rfRowHeaders);
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
}

FollowupView::~FollowupView()
{
    delete ui;
}

void FollowupView::on_loadLabButton_clicked()
{
    QString data = ui->textEdit->toPlainText();
    if(data.length()==0)
        return;
    QStringList strList = data.split(QRegularExpression("[\n]"),Qt::SkipEmptyParts);
    int lastRow = ui->tableWidget->rowCount();
    qDebug() << "rowCount before insert is: " << lastRow;
    ui->tableWidget->insertRow(lastRow);
    qDebug() << "rowCount after insert is: " << ui->tableWidget->rowCount();
    bool itemsAdded = false;

    foreach(QString str, strList) {
        LabValue lab = LabValue(str);
        if(lab.success == false)
            continue;

        int idx = indexes.value(lab.param,-1);
        if(idx>=0) {
            QString value = lab.getValue(ui->comboBox->currentIndex());
            QString prettyValue = value;
            if(value != "entfällt" & value != "s.Bem" & value != "folgt")
                prettyValue += lab.unit;
            QTableWidgetItem* item = new QTableWidgetItem(prettyValue);
            item->setData(256,value);
            item->setTextAlignment(Qt::AlignCenter);
            item->setToolTip(lab.comment);

            if(lab.flag=="+")
                item->setBackground(QColor(Qt::GlobalColor::red));
            else if(lab.flag=="-")
                item->setBackground(QColor(Qt::GlobalColor::blue));
            ui->tableWidget->setItem(lastRow,idx,item);
            itemsAdded = true;
        } else if(rfIdxs.contains(lab.param)) {
            loadRFsIntoTable(lab);
        }
    }

    if(!itemsAdded)
        ui->tableWidget->removeRow(lastRow);

    ui->textEdit->clear();
}

void FollowupView::on_loadMultiButton_clicked()
{
    QString data = ui->textEdit->toPlainText();
    if(data.length()==0)
        return;
    QStringList strList = data.split(QRegularExpression("[\n]"),Qt::SkipEmptyParts);
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
                    loadRFsIntoTable(p,v,"");
                }

                int idx = indexes.value(p, -1);
                if(idx>=0) {
                    qDebug() << "found idx: " << idx;
                    QTableWidgetItem *item = new QTableWidgetItem(v);
                    item->setData(256,v);
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
        }
        if(!hasValue){
            qDebug() << "removing row: " << r;
            ui->tableWidget->removeRow(r);
        }
    }
    ui->textEdit->clear();
}

void FollowupView::on_copyLabButton_clicked()
{
    QByteArray items;
    qDebug() << "onCopy: roWcount is: " << ui->tableWidget->rowCount();
    for(int r = 0; r < ui->tableWidget->rowCount(); r++) {
        items.append(ui->tableWidget->verticalHeaderItem(r)->text().toUtf8());
        items.append("\t\t");

        for(int c = 0; c < ui->tableWidget->columnCount(); c++) {
            QTableWidgetItem* t = ui->tableWidget->item(r,c);
            if(t == nullptr) {
                //qDebug() << "t is null!";
                items.append("");
            } else {
                items.append(t->data(256).toString().toUtf8());
            }
            if(c < ui->tableWidget->columnCount()-1)
                items.append("\t");
        }
        if(r< ui->tableWidget->rowCount()-1)
            items.append("\n");
    }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData("text/plain",items);
    QApplication::clipboard()->setMimeData(mimeData);
}

void FollowupView::on_clearButton_clicked()
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

void FollowupView::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem* item = ui->tableWidget->itemAt(pos);
    if(ui->tableWidget->rowCount()==0) {
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
            [=]() { int row = ui->tableWidget->rowAt(pos.y()); on_copyRow(row); });
    if(item == nullptr){
        qDebug() << " no item at this position";
    } else {
        menu->addAction(setCellValue);
    }

    //menu->addAction(editCell);
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

    QAction *deleteRow = new QAction("Ganze Zeile löschen");
    connect(deleteRow, &QAction::triggered, this, [=]() {
        if(ui->tableWidget->selectedRanges().length()>0) {
            QList<QTableWidgetSelectionRange> ranges = ui->tableWidget->selectedRanges();
            qDebug() << "got ranges length: " << ranges.length();
            for(int i = ranges.length(); i>0; i--) {
                QTableWidgetSelectionRange range = ranges.at(i-1);
                if((range.bottomRow()-range.topRow()) >= 1) {
                    for(int r = range.bottomRow(); r >= range.topRow(); r--) {
                        qDebug() << "removing multi row: " << r;
                        ui->tableWidget->removeRow(r);
                    }
                } else {
                    qDebug() << "removing single row: " << range.bottomRow();
                    ui->tableWidget->removeRow(range.bottomRow());
                }
            }
        }
    });
    menu->addSeparator();
    menu->addAction(deleteRow);

    menu->popup(ui->tableWidget->viewport()->mapToGlobal(pos));
}

void FollowupView::on_verticalHeader_customContextMenuRequested(const QPoint &pos)
{
    int row = ui->tableWidget->verticalHeader()->logicalIndexAt(pos);

    qDebug() << "Clicked on row: " << row;
    QMenu *menu=new QMenu(this);
    QAction* renameRow = new QAction("Datum eintragen");
    connect(renameRow, &QAction::triggered, this, [=]() {
        bool ok;
        QString rName = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                              tr("User name:"), QLineEdit::Normal,
                                              "", &ok);
        if(ok && !rName.isEmpty()) {
            ui->tableWidget->setVerticalHeaderItem(row,new QTableWidgetItem(rName));
        }
    });
    menu->addAction(renameRow);
    menu->popup(ui->tableWidget->verticalHeader()->viewport()->mapToGlobal(pos));
}

void FollowupView::on_verticalHeader_doubleClicked(int row)
{
    qDebug() << "double clicked on header row: " << row;
    bool ok;
    QString rName = QInputDialog::getText(this, tr("Datum eintragen"),
                                          tr("Datum:"), QLineEdit::Normal,
                                          "", &ok);
    if(ok && !rName.isEmpty()) {
        ui->tableWidget->setVerticalHeaderItem(row,new QTableWidgetItem(rName));
    }
}

void FollowupView::on_setCellValue()
{
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();

    foreach(QTableWidgetItem* item, items) {
        qDebug() << "received SIGNAL for item: " << item->text() << " at row: " << item->row() << " and column: " << item->column();
        int column = item->column();
        QTableWidgetItem* target = ui->tableWidget->takeItem(0,item->column());
        delete target;
        /*if(target == nullptr) {
            target = new QTableWidgetItem(item->text());
            target->setData(256,item->data(256));
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(0,item->column(),target);
        } else {
            target->setText(item->text());

        }
        target->setTextAlignment(Qt::AlignCenter);*/
        ui->tableWidget->takeItem(item->row(),item->column());
        ui->tableWidget->setItem(0,column,item);

    }
}

void FollowupView::on_tableWidget_cellDoubleClicked(int row, int column)
{
    QTableWidgetItem* item = ui->tableWidget->takeItem(row,column);
    if(item == nullptr) {
        qDebug() << "cellDoubleClicked: item is null at row: " << row << " and column: " << column;
        return;
    }
    ui->tableWidget->takeItem(0,column);
    ui->tableWidget->setItem(0,column,item);
    //QTableWidgetItem* target = ui->tableWidget->item(0,item->column());
    /*if(target == nullptr) {
        target = new QTableWidgetItem(item->text());
        target->setData(256,item->text());
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(0,item->column(),target);
    } else
        target->setText(item->text());
    target->setTextAlignment(Qt::AlignCenter);*/

}

void FollowupView::on_copyRow(int row)
{
    QByteArray items;
    //QTableWidgetItem* item = ui->tableWidget->selectedItems().at(0);
    for(int i = 0; i< ui->tableWidget->columnCount(); i++) {

        QTableWidgetItem* t = ui->tableWidget->item(row,i);
        if(t == nullptr) {
            //qDebug() << "t is null!";
            items.append("");
        } else {
            items.append(t->data(256).toString().toUtf8());
        }
        if(i< ui->tableWidget->columnCount()-1)
            items.append("\t");
    }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData("text/plain",items);
    QApplication::clipboard()->setMimeData(mimeData);
}

void FollowupView::on_copySelection()
{
    QByteArray items;
    QList<QTableWidgetItem*> selected = ui->tableWidget->selectedItems();
    QList<QTableWidgetSelectionRange> ranges = ui->tableWidget->selectedRanges();
    foreach(QTableWidgetItem* item, selected) {
        if(item == nullptr) {
            items.append("");
            return;
        } else {
            items.append(item->data(256).toString().toUtf8());
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
void FollowupView::on_rfWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);

    QAction *copyRF = new QAction("Risikofaktoren kopieren📋",this);
    connect(copyRF, &QAction::triggered, this, &FollowupView::on_copyRF);

    menu->addAction(copyRF);
    menu->popup(ui->rfWidget->viewport()->mapToGlobal(pos));
}

void FollowupView::on_copyRF()
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

void FollowupView::loadRFsIntoTable(QString p, QString v, QString c)
{
    qDebug() << "loadRFsIntoTable: " << p << v << c;
    int row = rfIdxs.value(p,-1);
    qDebug() << "loadRFsIntoTable: row is " << row;
    if(row>=0) {
        if(ui->rfWidget->item(row,0) == nullptr){
            qDebug() << "adding new rfWidget item for row: " << row << " parameter: " << p << " and value: " << v;
            QTableWidgetItem *item = new QTableWidgetItem(v);
            item->setToolTip(c);
            item->setData(256,v);
            if(c.length()>0)
                item->setBackground(QColor(Qt::GlobalColor::lightGray));
            item->setTextAlignment(Qt::AlignCenter);
            ui->rfWidget->setItem(row,0,item);
        }
    }
}

void FollowupView::loadRFsIntoTable(LabValue lab)
{
    int row = rfIdxs.value(lab.param,-1);
    if(row>=0) {
        if(ui->rfWidget->item(row,0) == nullptr){
            QString value = lab.getValue(ui->comboBox->currentIndex());
            QString prettyValue = value;
            if(value != "entfällt" & value != "s.Bem")
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
}

QString FollowupView::parseValue(QString p, QString v)
{
    QString output;
    if(v.contains(QRegularExpression("[+-]?([0-9]*[.])?[0-9]+")) ||
            v.contains("folgt") || v.contains("positiv") || v.contains("negativ") ||
            v.contains("s. Bem") || v.contains("entfällt")) {
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

void FollowupView::on_actionUrindiagnostik_triggered()
{
    uView->showMaximized();
}

void FollowupView::on_actionRegister_triggered()
{
    //rView->showMaximized();
}
