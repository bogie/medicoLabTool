#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(55);
    ui->tableWidget->setRowCount(1);
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
    indexes["EOSR"] = 4;
    indexes["INR"] = 5;
    indexes["PTT"] = 6;
    indexes["DD"] = 7;
    indexes["TZ"] = 8;
    indexes["NA"] = 9;
    indexes["K"] = 10;
    indexes["9ALB"] = 11;
    indexes["TBIL"] = 12;
    indexes["DBIL"] = 13;
    indexes["AST37"] = 14;
    indexes["ALT37"] = 15;
    indexes["GGT37"] = 16;
    indexes["AP37"] = 17;
    indexes["LDH37"] = 18;
    indexes["CK37"] = 19;
    indexes["CKMBMa"] = 20;
    indexes["TNTHS"] = 21;
    indexes["NTpBNP2"] = 22;
    indexes["HST"] = 23;
    indexes["KREA"] = 24;
    indexes["CRP"] = 25;
    indexes["PCTKM"] = 26;
    indexes["FERR"] = 27;
    indexes["PHV"] = 28;
    indexes["PCO2V"] = 29;
    indexes["HCO3V"] = 30;
    indexes["SBEV"] = 31;
    indexes["PO2V"] = 32;
    indexes["SO2V"] = 33;
    indexes["METHn"] = 34;
    indexes["COHBn"] = 35;
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
    indexes["9ACEYE"] = 53;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString data = ui->textEdit->toPlainText();
    QStringList strList = data.split(QRegExp("[\n]"),QString::SkipEmptyParts);

    foreach(QString str, strList) {
        QStringList split = str.split("	");
        if(split.length()<2)
            continue;
        QString p = split[0];
        QString v= split[2];

        if(v.contains(QRegExp("[+-]?([0-9]*[.])?[0-9]+")) || v == "folgt") {
            qDebug() << "p: " << p << "v: " << v;
            int idx = indexes.value(p, -1);
            if(idx>=0) {
                qDebug() << "found idx: " << idx;
                QTableWidgetItem *item = new QTableWidgetItem(v);
                ui->tableWidget->setItem(0,idx,item);
            }
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QList<QTableWidgetItem*> selected = QList<QTableWidgetItem*>();
    QByteArray items;
    for(int i = 0; i< ui->tableWidget->columnCount(); i++) {
        QTableWidgetItem* t = ui->tableWidget->item(0,i);
        if(t == nullptr) {
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
