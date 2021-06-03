#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "registerview.h"
#include "followupview.h"
#include "profileview.h"
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

    void on_openProfileButton_clicked();

    void on_languageCombo_currentIndexChanged(const QString &arg1);

    void on_languageCombo_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    RegisterView *rView;
    FollowupView *fView;

    void loadProfiles();
    QMap<QString, QSettings*> *profiles;
    QSettings *settings;
};

#endif // MAINWINDOW_H
