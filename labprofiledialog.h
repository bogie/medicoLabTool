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
    void on_buttonBox_accepted();

    void on_profileListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_addLabparamButton_clicked();

    void on_labTreeWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::LabProfileDialog *ui;
    QMap<QString, QSettings*> *profiles;
    QString selectedProfile;

    void loadProfiles();
    void saveProfile();
    void selectLabProfile(QString profileName);
};

#endif // LABPROFILEDIALOG_H
