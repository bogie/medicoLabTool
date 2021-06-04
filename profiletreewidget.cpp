#include "profiletreewidget.h"

ProfileTreeWidget::ProfileTreeWidget(QWidget *parent)
{
    this->setParent(parent);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setHeaderHidden(true);
    connect(this,&QWidget::customContextMenuRequested,
            this,&ProfileTreeWidget::on_customContextMenuRequested);

    this->setDragDropMode(DragDropMode::InternalMove);
}

void ProfileTreeWidget::on_customContextMenuRequested(const QPoint &pos)
{
    qDebug() << "ProfileTreeWidget: on_customContextMenuRequested at position: " << pos;
    QTreeWidgetItem* item = this->itemAt(pos);

    QMenu *menu = new QMenu(this);

    /*
     Logic:
     - item == null -> Add new Table
     - item.data(0,Qt::UserRole) == Table -> Modify Table, Delete Table, Add Param
     - item.data(0,Qt::UserRole) == Param -> Modify Param, Delete param, Add Codes
     - item.data(0,Qt::UserRole) == Code -> Modify Code, Delete Code
    */

    if(item == nullptr) {
        // Add new Table
        QAction *addTable = new QAction(tr("Add table"), this);
        connect(addTable, &QAction::triggered, this, [=]() {
            TableDialog td;
            td.setModal(true);

            if(td.exec() == QDialog::Accepted) {
                TableDialogResult res = td.getResult();
                qDebug() << "received result with name: "
                         << res.name << " and orientation: " << res.orientation;
                QTreeWidgetItem* child = new QTreeWidgetItem(this);
                child->setText(0,res.name);
                child->setData(0,Qt::UserRole,"table");
                child->setData(0,0x0101,res.orientation);
                QIcon icon(":/icons/table.png");
                child->setIcon(0,icon);
                this->addTopLevelItem(child);
            }
        });
        menu->addAction(addTable);
    } else {
        QString type = item->data(0,Qt::UserRole).toString();
        qDebug() << "Type is: " << type;
        if(type == "table") {
            QAction *modifyTable = new QAction(tr("Modify table"), this);
            connect(modifyTable, &QAction::triggered, this, [=]() {
                TableDialog td;
                td.setModal(true);
                td.setData(item->text(0),item->data(0,0x0101).toString());
                if(td.exec() == QDialog::Accepted) {
                    TableDialogResult res = td.getResult();
                    qDebug() << "Modify Table: received result with name: "
                             << res.name << " and orientation: " << res.orientation;
                    item->setText(0,res.name);
                    item->setData(0,Qt::UserRole,"table");
                    item->setData(0,0x0101,res.orientation);
                }
            });
            menu->addAction(modifyTable);

            QAction *addParam = new QAction(tr("Add parameter"),this);
            connect(addParam, &QAction::triggered, this, [=]() {
                ParamDialog pd;
                pd.setModal(true);

                if(pd.exec() == QDialog::Accepted) {
                    ParamDialogResult result = pd.getResult();
                    qDebug() << "AddParam: adding new parameter: " << result.name
                             << " with codes: " << result.codes;
                    QTreeWidgetItem* child = new QTreeWidgetItem(item);
                    child->setText(0,result.name);
                    child->setData(0,Qt::UserRole,"param");
                    child->setData(0,0x0101,result.codes);
                    for(QString code : result.codes) {
                        if(code.isEmpty())
                            continue;
                        QTreeWidgetItem* codeItem = new QTreeWidgetItem(child);
                        codeItem->setText(0,code);
                        codeItem->setData(0,Qt::UserRole,"code");
                        QIcon icon(":/icons/codes.png");
                        codeItem->setIcon(0,icon);
                        child->addChild(codeItem);
                    }
                    QIcon vial(":/icons/vial.png");
                    child->setIcon(0,vial);
                    item->addChild(child);
                }
            });
            menu->addAction(addParam);
        } else if(type == "param") {
            QAction *modifyParam = new QAction(tr("Modify parameter"),this);
            connect(modifyParam, &QAction::triggered, this, [=]() {
                ParamDialog pd;
                pd.setData(item->text(0),item->data(0,0x0101).toStringList());
                pd.setModal(true);

                if(pd.exec() == QDialog::Accepted) {
                    ParamDialogResult result = pd.getResult();
                    qDebug() << "adding new parameter: " << result.name
                             << " with codes: " << result.codes;
                    item->setText(0,result.name);
                    item->setData(0,Qt::UserRole,"param");
                    item->setData(0,0x0101,result.codes);
                    item->takeChildren();
                    for(QString code : result.codes) {
                        if(code.isEmpty())
                            continue;
                        QTreeWidgetItem* codeItem = new QTreeWidgetItem(item);
                        codeItem->setText(0,code);
                        codeItem->setData(0,Qt::UserRole,"code");
                        QIcon icon(":/icons/codes.png");
                        codeItem->setIcon(0,icon);
                        item->addChild(codeItem);
                    }
                }
            });
            menu->addAction(modifyParam);
        } else if(type == "code") {

        }
        QAction *removeItem = new QAction(tr("Delete item"),this);
        removeItem->setStatusTip(tr("Removes the selected profile"));
        connect(removeItem, &QAction::triggered, this, [=]() {
            QString type = item->data(0,Qt::UserRole).toString();
            if(type=="code") {
                QTreeWidgetItem *parent = item->parent();
                qDebug() << "Removing code " << item->text(0)
                         << "from parent: " << parent->text(0);
                QStringList codes = item->data(0,0x0101).toStringList();
                codes.removeAll(item->text(0));
                parent->setData(0,0x0101,codes);
            }

            delete item;
        });
        menu->addAction(removeItem);
    }
    menu->popup(this->viewport()->mapToGlobal(pos));
}
