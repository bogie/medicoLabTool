#ifndef PROFILETREEWIDGET_H
#define PROFILETREEWIDGET_H

#include <QTreeWidget>
#include <QObject>
#include <QWidget>
#include <QMenu>
#include <QInputDialog>
#include <QDropEvent>
#include <QMimeData>

#include "tabledialog.h"
#include "paramdialog.h"

class ProfileTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProfileTreeWidget(QWidget *parent);

private:
    void on_customContextMenuRequested(const QPoint &pos);
    void dropEvent(QDropEvent *event) override;
};

#endif // PROFILETREEWIDGET_H
