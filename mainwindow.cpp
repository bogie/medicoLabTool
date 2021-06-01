#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    profiles = new QMap<QString,QSettings*>();
    loadProfiles();
    rView = new RegisterView(this);
    fView = new FollowupView(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_registerViewButton_clicked()
{
    rView->show();
}

void MainWindow::on_followupViewButton_clicked()
{
    fView->show();
}

void MainWindow::on_actionProfile_triggered()
{
    LabProfileDialog lpd(this);
    int ret = lpd.exec();
    loadProfiles();
}

void MainWindow::on_openProfileButton_clicked()
{
    if(ui->profilesComboBox->children().length()==0)
        return;
    QSettings *s = profiles->value(ui->profilesComboBox->currentText());
    ProfileView *pView = new ProfileView(this,s);
    pView->setWindowTitle("Profile: "+s->value("profilename").toString());
    pView->show();
}

void MainWindow::loadProfiles()
{
    QDir profileDir("profiles");
    profileDir.setNameFilters(QStringList("*.ini"));
    profileDir.setFilter(QDir::Files);
    QStringList profileFileNames = profileDir.entryList();

    if(profileFileNames.length() > 0) {
        for(QString fn : profileFileNames) {
            QSettings *s = new QSettings(profileDir.absoluteFilePath(fn),QSettings::IniFormat);
            QString profName = s->value("profilename",fn).toString();
            profiles->insert(profName,s);
            ui->profilesComboBox->addItem(profName);
        }
    }
}
