#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QDialog>

class TableDialogResult {
    public:
    TableDialogResult() {}
    TableDialogResult(QString name, QString orientation) {
        this->name = name;
        this->orientation = orientation;
    }
    QString name;
    QString orientation;
};

namespace Ui {
class TableDialog;
}

class TableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TableDialog(QWidget *parent = nullptr);
    ~TableDialog();
    TableDialogResult getResult();
    void setData(QString name, QString orientation);

private slots:
    void on_buttonBox_accepted();

    void on_nameEdit_textChanged(const QString &arg1);

    void on_orientationBox_currentTextChanged(const QString &arg1);

private:
    Ui::TableDialog *ui;
    TableDialogResult result;
};

#endif // TABLEDIALOG_H
