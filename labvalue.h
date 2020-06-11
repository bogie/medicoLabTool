#ifndef LABVALUE_H
#define LABVALUE_H

#include <QString>
#include <QRegExp>
#include <QStringList>
#include <QDebug>

class LabValue
{
public:
    LabValue(QString line, bool multi);
    QString getValue(int delim);
    QString param;
    QString paramName;
    QString value;
    QString unit;
    QString flag;
    QString reference;
    QString status;
    QString unused;
    QString comment;
    bool success;
};

#endif // LABVALUE_H
