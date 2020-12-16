#ifndef LABPROFILEDIALOG_H
#define LABPROFILEDIALOG_H

#include <QDialog>
#include <QMap>
#include <QDir>
#include <QSettings>

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

private:
    Ui::LabProfileDialog *ui;
    QMap<QString, QSettings*> *profiles;

    void loadProfiles();
};

#endif // LABPROFILEDIALOG_H
