#include "viewselectordialog.h"
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    //ViewSelectorDialog d;
    //d.show();

    QSettings *settings = new QSettings(a.applicationDirPath()+"settings.ini",QSettings::IniFormat);

    QString language = settings->value("language").toString();


    if(language != "en") {
        QTranslator translator;
        // :/translations/labtool_de.ts

        QDir transDir(":/translations");

        QStringList fileNames = transDir.entryList(QStringList("*.qm"), QDir::Files,
                                                  QDir::Name);

        QMap<QString,QString> fileMap;
        for (QString fileName : fileNames) {
                QString path = transDir.filePath(fileName);
                QString name = fileName.sliced(8,2);
                fileMap.insert(name,path);
        }

        const QString &qmlFile = fileMap.value(language);
        qDebug() << "loading qml file: " << qmlFile;
        if(translator.load(qmlFile)) {
            QCoreApplication::installTranslator(&translator);
            qDebug() << "installed translator";
        }
    }

    MainWindow main;
    main.show();

    return a.exec();
}
