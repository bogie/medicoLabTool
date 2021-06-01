#ifndef PROFILEVIEW_H
#define PROFILEVIEW_H

#include <QMainWindow>
#include <QSettings>
#include "labvalue.h"
#include <QMimeData>
#include <QClipboard>

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

    void on_clearAll_clicked();
    void on_copyRow(int row);

private:
    Ui::ProfileView *ui;
    QSettings *settings;

    QMap<QString, int> indexes;

    void loadIndexes();
    QByteArray localizeString(QString input);
};

#endif // PROFILEVIEW_H
