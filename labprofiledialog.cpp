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

    if(profileFileNames.length() > 0) {
        for(QString fn : profileFileNames) {
            QSettings *s = new QSettings(profileDir.absoluteFilePath(fn),QSettings::IniFormat);
            QString profName = s->value("profilename",fn).toString();
            ui->profileListWidget->addItem(profName);
            profiles->insert(profName,s);
        }
        QFont f = ui->profileListWidget->item(0)->font();
        f.setBold(true);
        ui->profileListWidget->item(0)->setFont(f);
        selectLabProfile(ui->profileListWidget->item(0)->text());
    }
}

void LabProfileDialog::on_profileListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if(selectedProfile == item->text())
        return;

    qDebug() << "switching to profile: " << item->text();
    // save old profile
    saveProfile();

    QFont f = item->font();
    f.setBold(true);
    item->setFont(f);

    QList<QListWidgetItem*> pList = ui->profileListWidget->findItems(selectedProfile,Qt::MatchExactly);
    QFont ff = pList.at(0)->font();
    ff.setBold(false);
    pList.at(0)->setFont(ff);

    selectLabProfile(item->text());
}

void LabProfileDialog::selectLabProfile(QString profileName) {
    selectedProfile = profileName;
    ui->profileLabel->setText("Profile: "+profileName);

    ui->labTreeWidget->clear();
    // Logic to load data into tree widget
    QSettings* s = profiles->value(profileName);
    if(!s->contains("laborder"))
        return;
    QStringList labOrder = s->value("laborder").toStringList();
    if(labOrder.length()>0) {
        s->beginGroup("params");
        for(QString param : labOrder) {
            QStringList codes = s->value(param).toStringList();
            qDebug() << "key: " << param << "loaded codes: " << codes;
            QTreeWidgetItem* topItem = new QTreeWidgetItem();
            topItem->setText(0,param);
            for(QString code : codes) {
                QTreeWidgetItem* child = new QTreeWidgetItem(topItem);
                child->setText(0,code);
                topItem->addChild(child);
            }
            ui->labTreeWidget->addTopLevelItem(topItem);
        }
        s->endGroup();
    }
}

bool LabProfileDialog::createProfileFile(QString profileName)
{
    QDir profileDir("profiles");
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
    s->setValue("profilename",profileName);
    s->sync();
    profiles->insert(profileName,s);

    return true;
}

void LabProfileDialog::on_addLabparamButton_clicked()
{
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->labTreeWidget);
    item->setText(0,ui->labparamInput->text());
    ui->labTreeWidget->addTopLevelItem(item);
    ui->labparamInput->clear();

    bool ok;
    QString codes = QInputDialog::getText(this,
                                          tr("Medico Kennungen des Laborparameters angeben(optional mehrere durch Komma getrennt"),
                                          "Kennung",QLineEdit::Normal,
                                          "",
                                          &ok);
    if(ok && !codes.isEmpty()) {
        qDebug() << "adding new lab code(s): " << codes;

        QStringList codeList = codes.split(",",Qt::SkipEmptyParts);
        for(QString code : codeList) {
            QTreeWidgetItem* child = new QTreeWidgetItem(item);
            child->setText(0,code);
            item->addChild(child);
        }
    }
}

void LabProfileDialog::on_labTreeWidget_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem* item = ui->labTreeWidget->itemAt(pos);

    if(item==nullptr)
        return;
    for(int i = 0; i < item->childCount(); i++) {
        qDebug() << item->child(i)->text(0);
    }

    QMenu *menu = new QMenu(this);

    QAction *addChild = new QAction("Hinzufügen", this);
    connect(addChild, &QAction::triggered, this, [=]() {
        bool ok;
        QString param = QInputDialog::getText(this, tr("Medico Kennung des Laborparameters angeben"),"Kennung:",QLineEdit::Normal,"",&ok);
        if(ok && !param.isEmpty()) {
            qDebug() << "adding new lab code:" << param;

            QTreeWidgetItem* child = new QTreeWidgetItem(item);
            child->setText(0,param);
            item->addChild(child);
        }

    });
    QAction *removeItem = new QAction("Löschen",this);
    removeItem->setStatusTip("Löscht den Eintrag");
    connect(removeItem, &QAction::triggered, this, [=]() {
        delete item;
    });

    if(item->parent() == nullptr)
        menu->addAction(addChild);
    menu->addAction(removeItem);

    menu->popup(ui->labTreeWidget->viewport()->mapToGlobal(pos));
}

void LabProfileDialog::saveProfile() {
    QSettings* s = profiles->value(selectedProfile);
    QStringList labOrder;
    s->setValue("profilename",selectedProfile);
    s->remove("params");
    s->beginGroup("params");
    for(int t = 0; t < ui->labTreeWidget->topLevelItemCount(); t++) {
        QTreeWidgetItem* topItem = ui->labTreeWidget->topLevelItem(t);
        QStringList codes;
        for(int c = 0; c < topItem->childCount(); c++) {
            codes.append(topItem->child(c)->text(0));
        }
        qDebug() << "got topItem: " << topItem->text(0) << " with codes: " << codes;
        s->setValue(topItem->text(0),codes);
        labOrder.append(topItem->text(0));
    }
    s->endGroup();
    s->setValue("labOrder",labOrder);
}

void LabProfileDialog::on_profileListWidget_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem* item = ui->profileListWidget->itemAt(pos);
    qDebug() << "Profile List: Custom Context Menu requested at position: " << pos;
    QMenu *menu = new QMenu(this);

    if(item==nullptr) {
        qDebug() << "item is nullptr";
        QAction *addChild = new QAction("Profil hinzufügen", this);
        connect(addChild, &QAction::triggered, this, [=]() {
            bool ok;
            QString param = QInputDialog::getText(this, tr("Profilname eingeben"),"Name:",QLineEdit::Normal,"",&ok);
            if(ok && !param.isEmpty()) {
                qDebug() << "adding new profile: " << param;
                if(createProfileFile(param)) {
                    QListWidgetItem* item = new QListWidgetItem(ui->profileListWidget);
                    item->setText(param);
                    ui->profileListWidget->addItem(item);
                } else {
                    qDebug() << "failed to add new profile: reason exists";
                }
            }

        });
        menu->addAction(addChild);
    } else {
        QAction *removeItem = new QAction("Löschen",this);
        removeItem->setStatusTip("Profil löschen");
        connect(removeItem, &QAction::triggered, this, [=]() {
            delete item;
        });
        menu->addAction(removeItem);
    }
    menu->popup(ui->profileListWidget->viewport()->mapToGlobal(pos));
}

void LabProfileDialog::on_labparamInput_returnPressed()
{

}

void LabProfileDialog::on_okButton_clicked()
{
    saveProfile();
    this->accept();
}

void LabProfileDialog::on_cancelButton_clicked()
{
    this->reject();
}
