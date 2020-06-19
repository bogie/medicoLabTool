#ifndef URINVIEW_H
#define URINVIEW_H

#include <QMainWindow>
#include <QMap>
#include <QTableWidgetItem>
#include <QMenu>
#include <QMimeData>
#include <QClipboard>
#include <QDebug>

#include "labvalue.h"

namespace Ui {
class UrinView;
}

class UrinView : public QMainWindow
{
    Q_OBJECT

public:
    explicit UrinView(QWidget *parent = nullptr);
    ~UrinView();

private slots:
    void on_clearPatient_clicked();

    void on_loadUrine_clicked();

    void on_urineTable_customContextMenuRequested(const QPoint &pos);

    void on_copy_row(int row);

private:
    Ui::UrinView *ui;
    QMap<QString, int> urineIndexes;
};

#endif // URINVIEW_H
