#include "labprofiledialog.h"
#include "ui_labprofiledialog.h"

LabProfileDialog::LabProfileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LabProfileDialog)
{
    ui->setupUi(this);

    QDir appDir = QApplication::applicationDirPath();
    settings = new QSettings(appDir.absoluteFilePath("settings.ini"),QSettings::IniFormat);

    profWidget = new ProfileTreeWidget(this);
    ui->parameterLayout->addWidget(profWidget);
    profiles = new QMap<QString,QSettings*>();
    loadProfiles();
    loadPreDefined();
}

LabProfileDialog::~LabProfileDialog()
{
    delete ui;
}

void LabProfileDialog::loadProfiles()
{
    QDir profileDir(QApplication::applicationDirPath());
    profileDir.cd("profiles");
    profileDir.setNameFilters(QStringList("*.ini"));
    profileDir.setFilter(QDir::Files);
    QStringList profileFileNames = profileDir.entryList();

    if(profileFileNames.length() > 0) {
        for(QString fn : profileFileNames) {
            QSettings *s = new QSettings(profileDir.absoluteFilePath(fn),QSettings::IniFormat);
            QString profName = s->value("profilename",fn).toString();
            ui->profileListWidget->addItem(profName);
            profiles->insert(profName,s);
        }
        QString defaultProfile = settings->value("selectedProfile").toString();
        if(defaultProfile != "") {
            selectLabProfile(defaultProfile);
        } else {
            selectLabProfile(ui->profileListWidget->item(0)->text());
        }
    }
}

void LabProfileDialog::on_profileListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    selectLabProfile(item->text());
}

void LabProfileDialog::selectLabProfile(QString profileName) {
    if(selectedProfile == profileName)
        return;

    qDebug() << "switching to profile: " << profileName;
    // save old profile
    if(selectedProfile != "" & selectedProfile != profileName)
        saveProfile();

    QList<QListWidgetItem*> oldItem = ui->profileListWidget->findItems(selectedProfile,Qt::MatchExactly);
    if(oldItem.length()>0) {
        QFont f = oldItem.at(0)->font();
        f.setBold(false);
        oldItem.at(0)->setFont(f);
    }

    QList<QListWidgetItem*> pList = ui->profileListWidget->findItems(profileName,Qt::MatchExactly);
    if(pList.length()>0) {
        QFont ff = pList.at(0)->font();
        ff.setBold(true);
        pList.at(0)->setFont(ff);
    }


    // Logic to load data into tree widget
    QSettings* s = profiles->value(profileName);
    if(s == nullptr) {
        qDebug() << "selectProfile: failed to get settings for profileName " << profileName;
        return;
    }

    selectedProfile = profileName;
    settings->setValue("selectedProfile",selectedProfile);
    ui->profileLabel->setText("Profile: "+profileName);

    profWidget->clear();

    if(!s->childGroups().contains("tables")) {
        qDebug() << "profile does not contain tables";
        return;
    }

    s->beginGroup("tables");
    QStringList tables = s->childGroups();
    for(QString table : tables) {
        qDebug() << "loading table: " << table;
        QTreeWidgetItem *tItem = new QTreeWidgetItem(profWidget);
        tItem->setText(0,table);
        tItem->setData(0,Qt::UserRole,"table");
        s->beginGroup(table);
        tItem->setData(0,0x0101,s->value("orientation").toString());
        QStringList order = s->value("order").toStringList();
        s->beginGroup("parameters");
        for(QString param : order) {
            QTreeWidgetItem *pItem = new QTreeWidgetItem(tItem);
            pItem->setText(0,param);
            pItem->setData(0,Qt::UserRole,"param");
            QStringList codes = s->value(param).toStringList();

            for(QString code : codes){
                QTreeWidgetItem *cItem = new QTreeWidgetItem(pItem);
                cItem->setText(0,code);
                cItem->setData(0,Qt::UserRole,"code");
                QIcon icon(":/icons/codes.png");
                cItem->setIcon(0,icon);
                pItem->addChild(cItem);
            }
            QIcon vial(":/icons/vial.png");
            pItem->setIcon(0,vial);
            tItem->addChild(pItem);
        }
        s->endGroup();
        QIcon tIcon(":/icons/table.png");
        tItem->setIcon(0,tIcon);
        profWidget->addTopLevelItem(tItem);
        s->endGroup();
    }

    s->endGroup();
}

bool LabProfileDialog::createProfileFile(QString profileName)
{
    QDir profileDir(QApplication::applicationDirPath());
    profileDir.cd("profiles");
    profileDir.setNameFilters(QStringList("*.ini"));
    profileDir.setFilter(QDir::Files);
    QStringList profileFileNames = profileDir.entryList();

    if(profileFileNames.contains(profileName+".ini")) {
        qDebug() << "Profile already exists";
        return false;
    }
    qDebug() << "createProfileFile: adding new file for profilename: " << profileName << " filedir: "
             << profileDir.absoluteFilePath(profileName);

    QSettings *s = new QSettings(profileDir.absoluteFilePath(profileName+".ini"),QSettings::IniFormat);
    s->setValue("version","2.0");
    s->setValue("profilename",profileName);
    s->sync();
    profiles->insert(profileName,s);

    return true;
}

void LabProfileDialog::saveProfile() {
    if(selectedProfile.isEmpty()) {
        qDebug() << "no profiles to save!";
        return;
    }
    QSettings* s = profiles->value(selectedProfile);

    if(s == nullptr) {
        qDebug() << "could not find settings for profile: " << selectedProfile;
        return;
    }

    s->beginGroup("tables");
    for(int t = 0; t < profWidget->topLevelItemCount(); t++) {
        QTreeWidgetItem *topItem = profWidget->topLevelItem(t);
        QString orientation = topItem->data(0,0x0101).toString();
        qDebug() << "saveProfile: saving table " << topItem->text(0)
                 << " with orientation " << orientation
                 << " childCount is " << topItem->childCount();
        QStringList order;
        s->beginGroup(topItem->text(0));
        s->beginGroup("parameters");
        for(int c = 0; c < topItem->childCount(); c++) {
            QTreeWidgetItem *param = topItem->child(c);
            order.append(param->text(0));
            QStringList codes;
            for(int i = 0; i < param->childCount(); i++) {
                codes.append(param->child(i)->text(0));
            }
            s->setValue(param->text(0),codes);
        }
        s->endGroup();
        s->setValue("order",order);
        s->setValue("orientation",orientation);
        s->endGroup();
    }
    s->endGroup();
}

void LabProfileDialog::saveParameters() {

}

void LabProfileDialog::loadPreDefined()
{
    QDir profileDir(QApplication::applicationDirPath());
    profileDir.cd("profiles");
    profileDir.setNameFilters(QStringList("*.txt"));
    profileDir.setFilter(QDir::Files);
    QStringList profileFileNames = profileDir.entryList();

    QList<QString> addedCodes;

    if(profileFileNames.length() > 0) {
        for(QString fn : profileFileNames) {
            QFile f = QFile(profileDir.absoluteFilePath(fn));
            f.open(QFile::ReadOnly);
            while(!f.atEnd()) {
                QString line = QString(f.readLine());
                line = line.replace("\r","").replace("\n","");
                QStringList split = line.split("\t");
                //qDebug() << "split is: " << split;
                if(split.length()<2)
                    continue;

                QString code = split.at(0);
                QString name = split.at(1);

                if(addedCodes.contains(code)) {
                    qDebug() << "code already added: " << code;
                    continue;
                }
                addedCodes.append(code);

                QTreeWidgetItem *pItem = new QTreeWidgetItem(ui->preDefinedTreeWidget);
                pItem->setText(0,name);
                pItem->setData(0,Qt::UserRole,"param");
                QStringList codes(code);

                for(QString code : codes){
                    QTreeWidgetItem *cItem = new QTreeWidgetItem(pItem);
                    cItem->setText(0,code);
                    cItem->setData(0,Qt::UserRole,"code");
                    QIcon icon(":/icons/codes.png");
                    cItem->setIcon(0,icon);
                    pItem->addChild(cItem);
                }
                QIcon vial(":/icons/vial.png");
                pItem->setIcon(0,vial);
                ui->preDefinedTreeWidget->addTopLevelItem(pItem);
            }
        }
    }
}

void LabProfileDialog::on_profileListWidget_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem* item = ui->profileListWidget->itemAt(pos);
    qDebug() << "Profile List: Custom Context Menu requested at position: " << pos;
    QMenu *menu = new QMenu(this);

    if(item==nullptr) {
        qDebug() << "item is nullptr";
        QAction *addChild = new QAction(tr("Add profile"), this);
        connect(addChild, &QAction::triggered, this, [=]() {
            bool ok;
            QString param = QInputDialog::getText(this, tr("Enter profile name"),tr("Name:"),QLineEdit::Normal,"",&ok);
            if(ok && !param.isEmpty()) {
                qDebug() << "adding new profile: " << param;
                if(createProfileFile(param)) {
                    QListWidgetItem* item = new QListWidgetItem(ui->profileListWidget);
                    item->setText(param);
                    ui->profileListWidget->addItem(item);
                    if(selectedProfile.isEmpty()) {
                        qDebug() << "no previous profile selected, selecting newly created profile: " << param;
                        selectLabProfile(param);
                    }
                } else {
                    qDebug() << "failed to add new profile: reason exists";
                }
            }

        });
        menu->addAction(addChild);
    } else {
        QAction *removeItem = new QAction(tr("Delete profile"),this);
        removeItem->setStatusTip(tr("Removes the selected profile"));
        connect(removeItem, &QAction::triggered, this, [=]() {
            delete item;
        });
        menu->addAction(removeItem);
    }
    menu->popup(ui->profileListWidget->viewport()->mapToGlobal(pos));
}

void LabProfileDialog::on_okButton_clicked()
{
    saveProfile();
    saveParameters();
    this->accept();
}

void LabProfileDialog::on_cancelButton_clicked()
{
    this->reject();
}

void LabProfileDialog::on_predefinedTreeWidget_customContextMenuRequested(const QPoint &pos)
{

}
