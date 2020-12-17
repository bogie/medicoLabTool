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
    selectLabProfile(ui->profileListWidget->item(0)->text());
}

void LabProfileDialog::on_buttonBox_accepted()
{
    saveProfile();
}

void LabProfileDialog::on_profileListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if(selectedProfile == item->text())
        return;

    qDebug() << "switching to profile: " << item->text();
    // save old profile
    saveProfile();
    selectLabProfile(item->text());
}

void LabProfileDialog::selectLabProfile(QString profileName) {
    selectedProfile = profileName;
    ui->profileLabel->setText("Profile: "+profileName);

    ui->labTreeWidget->clear();
    // Logic to load data into tree widget
    QSettings* s = profiles->value(profileName);
    QStringList labOrder = s->value("labOrder").toStringList();
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

void LabProfileDialog::on_addLabparamButton_clicked()
{
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->labTreeWidget);
    item->setText(0,ui->labparamInput->text());
    ui->labTreeWidget->addTopLevelItem(item);
    ui->labparamInput->clear();
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
            qDebug() << "adding new child";
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
