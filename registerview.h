#ifndef REGISTERVIEW_H
#define REGISTERVIEW_H

#include <QMainWindow>
#include <QMap>
#include <QTableWidgetItem>
#include <QMenu>
#include <QMimeData>
#include <QClipboard>
#include <QDebug>
#include <QMessageBox>

#include "labvalue.h"

namespace Ui {
class RegisterView;
}

class RegisterView : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterView(QWidget *parent = nullptr);
    ~RegisterView();

private slots:
    void on_registerTable_customContextMenuRequested(const QPoint &pos);

    void on_loadRegister_clicked();

    void on_rfWidget_customContextMenuRequested(const QPoint &pos);

    void on_setCellValue();
    void on_copyRF();
    void on_copyRow(int row);
    void on_copyUrineRow(int row);
    void on_copySelection();
    void on_registerTable_cellDoubleClicked(int row, int column);

    void on_clearPatient_clicked();

    void on_urineTable_customContextMenuRequested(const QPoint &pos);

    void on_loadCumulative_clicked();

    void on_copyAllLabs_clicked();

    void on_registerTable_itemSelectionChanged();

private:
    Ui::RegisterView *ui;

    QMap<QString, int> indexes;
    QMap<QString, int> rfIdxs;
    QMap<QString, int> urineIndexes;
    bool merginLabs;
};

#endif // REGISTERVIEW_H
