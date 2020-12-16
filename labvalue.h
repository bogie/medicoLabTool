#ifndef LABVALUE_H
#define LABVALUE_H

#include <QString>
#include <QRegularExpression>
#include <QStringList>
#include <QDebug>

class LabValue
{
public:
    LabValue(QString line);
    LabValue(QString parameter, QString paramName, QString unit, QString refRange, QString value);
    QString getValue(int delim);
    QString getValue();
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
    bool isPassthrough;
};

#endif // LABVALUE_H
