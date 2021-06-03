#ifndef PROFILETREEWIDGET_H
#define PROFILETREEWIDGET_H

#include <QTreeWidget>
#include <QObject>
#include <QWidget>
#include <QMenu>
#include <QInputDialog>

#include "tabledialog.h"
#include "paramdialog.h"

class ProfileTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProfileTreeWidget(QWidget *parent);

private:
    void on_customContextMenuRequested(const QPoint &pos);

};

#endif // PROFILETREEWIDGET_H
