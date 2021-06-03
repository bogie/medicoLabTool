#ifndef PARAMDIALOG_H
#define PARAMDIALOG_H

#include <QDialog>
#include <QAction>
#include <QMenu>

class ParamDialogResult {
public:
    ParamDialogResult() {}
    ParamDialogResult(QString name, QStringList codes) {
        this->name = name;
        this->codes = codes;
    }
    QString name;
    QStringList codes;
};


namespace Ui {
class ParamDialog;
}

class ParamDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParamDialog(QWidget *parent = nullptr);
    ~ParamDialog();
    ParamDialogResult getResult();
    void setData(QString name, QStringList codes);

private slots:
    void on_codeEdit_returnPressed();

    void on_okButton_clicked();

    void on_cancelButton_clicked();

    void on_addCodeButton_clicked();

    void on_codesListWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::ParamDialog *ui;
    ParamDialogResult result;
};

#endif // PARAMDIALOG_H
