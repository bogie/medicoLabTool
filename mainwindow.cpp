#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "Application file path " << QApplication::applicationDirPath();
    QDir appDir = QApplication::applicationDirPath();
    settings = new QSettings(appDir.absoluteFilePath("settings.ini"),QSettings::IniFormat);

    profiles = new QMap<QString,QSettings*>();
    loadProfiles();
    rView = new RegisterView(this);
    fView = new FollowupView(this);


    QDir dir(":/translations");
    QStringList fileNames = dir.entryList(QStringList("*.qm"), QDir::Files,
                                          QDir::Name);
    for (QString &fileName : fileNames)
        fileName = fileName.sliced(8,2);

    QString language = settings->value("language").toString();

    ui->languageCombo->addItem(tr("en"));
    ui->languageCombo->addItems(fileNames);
    ui->languageCombo->setCurrentText(language);
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
    lpd.exec();
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
    QDir profileDir(QApplication::applicationDirPath());
    profileDir.cd("profiles");
    profileDir.setNameFilters(QStringList("*.ini"));
    profileDir.setFilter(QDir::Files);
    QStringList profileFileNames = profileDir.entryList();

    qDebug() << "loadProfiles: absolute path is " << profileDir.absolutePath()
             << " and profileFileNames are: " << profileFileNames;

    ui->profilesComboBox->clear();
    if(profileFileNames.length() > 0) {
        for(const QString &fn : profileFileNames) {
            QSettings *s = new QSettings(profileDir.absoluteFilePath(fn),QSettings::IniFormat);
            QString profName = s->value("profilename",fn).toString();
            profiles->insert(profName,s);
            ui->profilesComboBox->addItem(profName);
        }
        QString defaultProfile = settings->value("selectedProfile").toString();
        if(defaultProfile != "" & ui->profilesComboBox->findText(defaultProfile)>=0) {
            qDebug() << "selecting default profile " << defaultProfile;
            ui->profilesComboBox->setCurrentText(defaultProfile);
        }
    }
}

void MainWindow::on_profilesComboBox_textActivated(const QString &arg1)
{
    settings->setValue("selectedProfile",arg1);
}

void MainWindow::on_languageCombo_textActivated(const QString &arg1)
{
    settings->setValue("language",arg1);
}
