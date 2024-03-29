#ifndef VIEWSELECTORDIALOG_H
#define VIEWSELECTORDIALOG_H

#include <QDialog>
#include "followupview.h"
#include "registerview.h"

namespace Ui {
class ViewSelectorDialog;
}

class ViewSelectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewSelectorDialog(QWidget *parent = nullptr);
    FollowupView* w;
    RegisterView* r;
    ~ViewSelectorDialog();

private slots:
    void on_registerViewButton_clicked();

    void on_followupViewButton_clicked();

private:
    Ui::ViewSelectorDialog *ui;
};

#endif // VIEWSELECTORDIALOG_H
