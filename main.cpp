#include "viewselectordialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    ViewSelectorDialog d;
    d.show();

    return a.exec();
}
