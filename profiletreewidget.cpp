#include "profiletreewidget.h"

ProfileTreeWidget::ProfileTreeWidget(QWidget *parent)
{
    this->setParent(parent);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setHeaderHidden(true);
    this->setEditTriggers(QTreeWidget::NoEditTriggers);
    connect(this,&QWidget::customContextMenuRequested,
            this,&ProfileTreeWidget::on_customContextMenuRequested);

    this->setDragDropMode(DragDropMode::DragDrop);
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
                QStringList codes;
                for(int i = 0; i < item->childCount(); i++) {
                    codes.append(item->child(i)->text(0));
                }

                pd.setData(item->text(0),codes);
                pd.setModal(true);

                if(pd.exec() == QDialog::Accepted) {
                    ParamDialogResult result = pd.getResult();
                    qDebug() << "adding new parameter: " << result.name
                             << " with codes: " << result.codes;
                    item->setText(0,result.name);
                    item->setData(0,Qt::UserRole,"param");
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
                QStringList codes;
                for(int i = 0; i < item->childCount(); i++) {
                    codes.append(item->child(i)->text(0));
                }
                codes.removeAll(item->text(0));
                parent->setData(0,0x0101,codes);
            }

            delete item;
        });
        menu->addAction(removeItem);
    }
    menu->popup(this->viewport()->mapToGlobal(pos));
}

void ProfileTreeWidget::dropEvent(QDropEvent *event)
{
    const QMimeData* qMimeData = event->mimeData();
    QByteArray encoded = qMimeData->data("application/x-qabstractitemmodeldatalist");

    QTreeWidget* widget = qobject_cast<QTreeWidget*>(event->source());
    if(widget != nullptr) {
        QTreeWidgetItem* item = widget->currentItem();
        QString type = item->data(0,Qt::UserRole).toString();

        QTreeWidgetItem* target = this->itemAt(event->position().toPoint());
        QString targetType = target->data(0,Qt::UserRole).toString();

        if(item == target) {
            event->ignore();
            return;
        }

        if(type == "table") {
            event->ignore();
            return;
        }

        if(target == nullptr) {
            event->ignore();
            return;
        }

        if(type == "code" && targetType == "table") {
            return;
        }

        QModelIndex index = target->treeWidget()->indexAt(event->position().toPoint());

        qDebug() << "index row " << index.row() << " column " << index.column();

        if(event->source() == this) {
            qDebug() << "dragging to self";
            if(type == "param") {
                if(targetType == "param") {
                    qDebug() << "param to param";
                    item->parent()->removeChild(item);
                    QModelIndex newindex = target->treeWidget()->indexAt(event->position().toPoint());
                    int targetindex = target->parent()->indexOfChild(target);
                    int row = newindex.row();
                    if(targetindex == row) {
                        row++;
                    }
                    target->parent()->insertChild(row,item);
                    event->accept();
                } else if(targetType == "code") {
                    event->ignore();
                } else if(targetType == "table") {
                    item->parent()->removeChild(item);
                    target->insertChild(index.row(),item);
                    event->accept();
                }
            } else if(type == "code") {
                if(targetType == "param") {
                    item->parent()->removeChild(item);
                    target->insertChild(index.row(),item);
                    event->accept();
                } else if(targetType == "code") {
                    item->parent()->removeChild(item);
                    target->parent()->insertChild(index.row(),item);
                    event->accept();
                }
            }

        } else {
            qDebug() << "not dragging to self";

            if(type == "param") {
                if(targetType == "param") {
                    QModelIndex newindex = target->treeWidget()->indexAt(event->position().toPoint());
                    int targetindex = target->parent()->indexOfChild(target);
                    int row = newindex.row();
                    if(targetindex == row) {
                        row++;
                    }
                    target->parent()->insertChild(row,item->clone());
                    event->accept();
                } else if(targetType == "table") {
                    target->insertChild(index.row(),item->clone());
                    event->accept();
                }
            } else if(type == "code") {
                if(targetType == "param") {
                    if(target->childCount()== 0) {
                        target->addChild(item->clone());
                    } else {
                    target->insertChild(index.row(),item->clone());
                    }
                    event->accept();
                } else if(targetType == "code") {
                    target->parent()->insertChild(index.row(),item->clone());
                    event->accept();
                }
            }

        }
    }


}

