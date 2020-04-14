#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMimeData>
#include <QClipboard>
#include <QTableWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_clearButton_clicked();

    void on_loadLabButton_clicked();

    void on_copyLabButton_clicked();

    void on_loadMultiButton_clicked();

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void on_setCellValue();

    void on_copyRF();

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_rfWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    QMap<QString, int> indexes;
    QMap<QString, int> rfIdxs;

    void on_copyRow(int row);
    void loadRFsIntoTable(QString p, QString v);
};

#endif // MAINWINDOW_H
