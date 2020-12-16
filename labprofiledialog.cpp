#include "labprofiledialog.h"
#include "ui_labprofiledialog.h"

LabProfileDialog::LabProfileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LabProfileDialog)
{
    ui->setupUi(this);

    profiles = new QMap<QString,QSettings*>();
    loadProfiles();
}

LabProfileDialog::~LabProfileDialog()
{
    delete ui;
}

void LabProfileDialog::loadProfiles()
{
    QDir profileDir("profiles");
    profileDir.setNameFilters(QStringList("*.ini"));
    profileDir.setFilter(QDir::Files);
    QStringList profileFileNames = profileDir.entryList();

    for(QString fn : profileFileNames) {
        QSettings *s = new QSettings(profileDir.absoluteFilePath(fn),QSettings::IniFormat);
        QString profName = s->value("profilename",fn).toString();
        ui->profileListWidget->addItem(profName);
        profiles->insert(profName,s);
    }
}

void LabProfileDialog::on_buttonBox_accepted()
{

}
