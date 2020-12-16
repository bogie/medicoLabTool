#ifndef FOLLOWUPVIEW_H
#define FOLLOWUPVIEW_H

#include <QMainWindow>
#include <QDebug>
#include <QMimeData>
#include <QClipboard>
#include <QTableWidgetItem>
#include <QTableWidgetSelectionRange>
#include <QHeaderView>
#include <QInputDialog>
#include <QLineEdit>

#include "labvalue.h"
#include "urinview.h"

namespace Ui {
class FollowupView;
}

class FollowupView : public QMainWindow
{
    Q_OBJECT

public:
    explicit FollowupView(QWidget *parent = nullptr);
    ~FollowupView();

private slots:
    void on_clearButton_clicked();

    void on_loadLabButton_clicked();

    void on_copyLabButton_clicked();

    void on_loadMultiButton_clicked();

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);
    void on_verticalHeader_customContextMenuRequested(const QPoint &pos);
    void on_verticalHeader_doubleClicked(int row);

    void on_setCellValue();
    void on_copyRF();
    void on_copyRow(int row);
    void on_copySelection();

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_rfWidget_customContextMenuRequested(const QPoint &pos);

    void on_actionUrindiagnostik_triggered();

    void on_actionRegister_triggered();

private:
    Ui::FollowupView *ui;
    QMap<QString, int> indexes;
    QMap<QString, int> rfIdxs;
    UrinView *uView;

    QString parseValue(QString p, QString v);
    void loadRFsIntoTable(QString p, QString v, QString c);
    void loadRFsIntoTable(LabValue lab);
};

#endif // FOLLOWUPVIEW_H
