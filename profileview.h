#ifndef PROFILEVIEW_H
#define PROFILEVIEW_H

#include <QMainWindow>
#include <QSettings>
#include "labvalue.h"
#include <QMimeData>
#include <QClipboard>
#include <QTableWidget>
#include <QMessageBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHeaderView>

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
    void on_customContextMenuRequested(const QPoint &pos);
    void on_customHeaderContextMenuRequested(const QPoint &pos);
    void on_clearAll_clicked();
    void on_copyWidgetRow(QTableWidget* widget, int row);
    void on_copyWidgetColumn(QTableWidget* widget);
    void on_parseCumulativeButton_clicked();

private:
    Ui::ProfileView *ui;
    QSettings *settings;
    bool mergingLabs;

    QMap<QString, LabParamPosition> positions;
    QMap<QTableWidget*,QString> tableWidgets;

    void loadTables();
    QByteArray localizeString(QString input);
};

#endif // PROFILEVIEW_H
