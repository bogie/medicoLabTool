#ifndef LABPROFILEDIALOG_H
#define LABPROFILEDIALOG_H

#include <QDialog>
#include <QMap>
#include <QDir>
#include <QSettings>
#include <QListWidget>
#include <QListWidgetItem>
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QKeyEvent>

#include "profiletreewidget.h"
struct Parameter {
    QString name;
    QStringList codes;
};

struct Table {
    QString name;
    QString orientation;
    QStringList order;
    QList<Parameter> parameters;
};

namespace Ui {
class LabProfileDialog;
}

class LabProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LabProfileDialog(QWidget *parent = nullptr);
    ~LabProfileDialog();

private slots:
    void on_profileListWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_profileListWidget_customContextMenuRequested(const QPoint &pos);
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    void on_predefinedTreeWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::LabProfileDialog *ui;
    QMap<QString, QSettings*> *profiles;
    QString selectedProfile;
    ProfileTreeWidget *profWidget;
    QSettings *settings;


    void loadProfiles();
    void saveProfile();
    void loadPreDefined();
    void selectLabProfile(QString profileName);
    bool createProfileFile(QString profileName);
};

#endif // LABPROFILEDIALOG_H
