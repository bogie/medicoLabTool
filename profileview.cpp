#include "profileview.h"
#include "ui_profileview.h"

ProfileView::ProfileView(QWidget *parent, QSettings *settings) :
    QMainWindow(parent),
    ui(new Ui::ProfileView),
    settings(settings)
{
    ui->setupUi(this);
    mergingLabs = false;

    loadTables();
    //loadIndexes();
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

void ProfileView::loadTables()
{
    settings->beginGroup("tables");
    QStringList tables = settings->childGroups();
    for(QString table : tables) {
        QTableWidget *newTable = new QTableWidget(this);
        newTable->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(newTable,&QWidget::customContextMenuRequested,
                this,&ProfileView::on_customContextMenuRequested);
        settings->beginGroup(table);
        QString orientation = settings->value("orientation").toString();
        QStringList order = settings->value("order").toStringList();

        settings->beginGroup("parameters");
        for(int i = 0; i < order.length(); i++) {
            QString param = order.at(i);
            QStringList codes = settings->value(param).toStringList();
            for(QString code : codes) {
                LabParamPosition pos(newTable,i,orientation);
                positions.insert(code,pos);
            }
        }
        settings->endGroup();
        if(orientation=="horizontal") {
            newTable->setColumnCount(order.length());
            newTable->setHorizontalHeaderLabels(order);
            ui->bottomLayout->addWidget(newTable);
        }
        else if(orientation=="vertical") {
            newTable->setRowCount(order.length());
            newTable->setColumnCount(1);
            newTable->setVerticalHeaderLabels(order);
            newTable->setHorizontalHeaderLabels(QStringList("Value"));
            ui->topLayout->addWidget(newTable);
        }

        settings->endGroup();
        tableWidgets.insert(newTable,orientation);
    }
    settings->endGroup();
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

    QList<bool> addedList;
    QList<QTableWidget*> widgetList;
    for(int i = 0; i < ui->bottomLayout->count(); i++) {
        QTableWidget* widget = (QTableWidget*)ui->bottomLayout->itemAt(i)->widget();
        widget->insertRow(widget->rowCount());
        addedList.append(false);
        widgetList.append(widget);
    }

    foreach(QString line, lines) {
        LabValue lab = LabValue(line);
        if(lab.success == false)
            continue;

        if(positions.contains(lab.param)) {
            LabParamPosition pos = positions.value(lab.param);
            qDebug() << "Adding " << lab.param << " at index: " << pos.position;
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

            if(pos.orientation == "horizontal") {
                pos.widget->setItem(pos.widget->rowCount()-1,pos.position,item);
                int addedIdx = widgetList.indexOf(pos.widget);
                qDebug() << "addedIdx is " << addedIdx;
                addedList[addedIdx] = true;
            } else if(pos.orientation == "vertical") {
                pos.widget->setItem(pos.position,0,item);
            }
        }
    }

    for(int i = 0; i < widgetList.length(); i++) {
        if(addedList.at(i) == false) {
            widgetList.at(i)->removeRow(widgetList.at(i)->rowCount()-1);
        }
    }

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

void ProfileView::on_customContextMenuRequested(const QPoint &pos)
{
    qDebug() << "Got Custom context menu requested at pos: " << pos;
    if(QTableWidget* widget = qobject_cast<QTableWidget*>(this->sender())) {
        qDebug() << "showing custom context menu for widget";
        QMenu *menu = new QMenu(widget);
        QTableWidgetItem* item = widget->itemAt(pos);
        if(item == nullptr) {

        } else {

            QString orientation = tableWidgets.value(widget);

            QAction *copyRow = new QAction(tr("Copy row📋"),this);
            connect(copyRow, &QAction::triggered, this, [=]() {
                int row = widget->rowAt(pos.y());
                on_copyWidgetRow(widget,row);
            });
            if(item == nullptr){
                qDebug() << " no item at this position";
            }
            QAction *copyRF = new QAction(tr("Copy column📋"),this);
            connect(copyRF, &QAction::triggered, this, [=]() {
                on_copyWidgetColumn(widget);
            });

            QAction *deleteRow = new QAction(tr("Delete row"));
            connect(deleteRow, &QAction::triggered, this, [=]() {
                if(widget->selectedRanges().length()>0) {
                    QList<QTableWidgetSelectionRange> ranges = widget->selectedRanges();
                    qDebug() << "got ranges length: " << ranges.length();
                    mergingLabs = true;
                    for(int i = ranges.length(); i>0; i--) {
                        QTableWidgetSelectionRange range = ranges.at(i-1);
                        if((range.bottomRow()-range.topRow()) >= 1) {
                            for(int r = range.bottomRow(); r >= range.topRow(); r--) {
                                qDebug() << "removing multi row: " << r;
                                widget->removeRow(r);
                            }
                        } else {
                            qDebug() << "removing single row: " << range.bottomRow();
                            widget->removeRow(range.bottomRow());
                        }
                    }
                    mergingLabs = false;
                }
            });


                QAction *mergeRow = new QAction("Zeilen kombinieren");
                connect(mergeRow, &QAction::triggered, this, [=]() {
                    QList<QTableWidgetSelectionRange> ranges = widget->selectedRanges();
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
                            for(int c = 0; c < widget->columnCount(); c++) {
                                for(int r = range.topRow()+1; r <= range.bottomRow(); r++) {
                                    qDebug() << "merging at row: " << r;
                                    QTableWidgetItem* topItem = widget->item(range.topRow(),c);
                                    QTableWidgetItem* bottomItem = widget->item(r,c);

                                    if(topItem == nullptr || failureCodes.contains(topItem->data(256).toString())) {
                                        if(bottomItem != nullptr) {
                                            qDebug() << " bottomItem has text: " << bottomItem->text();
                                            bottomItem = widget->takeItem(r,c);
                                            widget->setItem(range.topRow(),c,bottomItem);
                                            qDebug() << "new position: " << range.topRow() << " column: " << c;
                                        }
                                    }
                                }
                            }
                            for(int i = range.bottomRow(); i >= range.topRow()+1; i--)
                                widget->removeRow(i);
                            mergingLabs = false;
                        }
                    }
                });


            if(orientation == "vertical") {
                menu->addAction(copyRF);
            } else if(orientation == "horizontal") {
                if(widget->selectedRanges().at(0).rowCount()>1) {
                    menu->addSeparator();
                    menu->addAction(mergeRow);
                }
                menu->addAction(copyRow);
                menu->addSeparator();
                menu->addAction(deleteRow);
            }
        }
        menu->popup(widget->viewport()->mapToGlobal(pos));
    }
}

void ProfileView::on_clearAll_clicked()
{
    for(int i = ui->labTable->rowCount(); i >=0; i--){
        ui->labTable->removeRow(i);
    }

    for(int i = 0; i < ui->bottomLayout->count(); i++) {
        QTableWidget* widget = (QTableWidget*)ui->bottomLayout->itemAt(i)->widget();
        for(int i = widget->rowCount(); i>=0; i--) {
            widget->removeRow(i);
        }
    }

    for(int i = 0; i < ui->topLayout->count(); i++) {
        if(QTableWidget* widget = qobject_cast<QTableWidget*>(ui->topLayout->itemAt(i)->widget())){
            widget->clearContents();
        }

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

void ProfileView::on_copyWidgetRow(QTableWidget *widget, int row)
{
    QByteArray items;
    for(int i = 0; i< widget->columnCount(); i++) {

        QTableWidgetItem* item = widget->item(row,i);
        if(item == nullptr) {
            items.append("");
        } else {
            items.append(localizeString(item->data(256).toString()));
        }
        if(i< widget->columnCount()-1)
            items.append("\t");
    }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData("text/plain",items);
    QApplication::clipboard()->setMimeData(mimeData);
}

void ProfileView::on_copyWidgetColumn(QTableWidget *widget)
{
    QByteArray items;
    for(int i=0; i< widget->rowCount(); i++) {
        QTableWidgetItem* item = widget->item(i,0);
        if(item == nullptr) {
            // skip
            items.append("");
        } else {
            items.append(localizeString(item->data(256).toString()));
        }
        if(i< widget->rowCount()-1)
            items.append("\n");
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
