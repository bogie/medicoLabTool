#ifndef PROFILEVIEW_H
#define PROFILEVIEW_H

#include <QMainWindow>
#include <QSettings>
#include "labvalue.h"
#include <QMimeData>
#include <QClipboard>
#include <QTableWidget>
#include <QMessageBox>

class LabParamPosition {
    public:
        LabParamPosition() {
            this->widget = nullptr;
            this->position = -1;
            this->orientation = "";
        }
        LabParamPosition(QTableWidget* widget, int position, QString orientation) {
            this->widget = widget;
            this->position = position;
            this->orientation = orientation;
        }

    public:
        QTableWidget* widget;
        int position;
        QString orientation;
};

namespace Ui {
class ProfileView;
}

class ProfileView : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProfileView(QWidget *parent = nullptr, QSettings* settings = nullptr);
    ~ProfileView();

private slots:
    void on_copyAllLabs_clicked();

    void on_loadRawLab_clicked();

    void on_labTable_customContextMenuRequested(const QPoint &pos);

    void on_customContextMenuRequested(const QPoint &pos);

    void on_clearAll_clicked();
    void on_copyRow(int row);
    void on_copyWidgetRow(QTableWidget* widget, int row);
    void on_copyWidgetColumn(QTableWidget* widget);

private:
    Ui::ProfileView *ui;
    QSettings *settings;
    bool mergingLabs;

    QMap<QString, int> indexes;
    QMap<QString, LabParamPosition> positions;
    QMap<QTableWidget*,QString> tableWidgets;

    void loadIndexes();
    void loadTables();
    QByteArray localizeString(QString input);
};

#endif // PROFILEVIEW_H
