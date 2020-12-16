#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "registerview.h"
#include "followupview.h"
#include "labprofiledialog.h"

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
    void on_registerViewButton_clicked();

    void on_followupViewButton_clicked();

    void on_actionProfile_triggered();

private:
    Ui::MainWindow *ui;
    RegisterView *rView;
    FollowupView *fView;
};

#endif // MAINWINDOW_H
