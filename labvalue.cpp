#include "labvalue.h"

LabValue::LabValue(QString line, bool multi)
{
    if(!multi){
        QStringList split = line.split("\t");
        if(split.length()<2) {
            this->success = false;
            return;
        }
        this->param = split[0];
        this->paramName = split[1];
        this->value = split[2];
        this->unit = split[3];
        this->flag = split[4];
        this->reference = split[5];
        this->status = split[6];
        this->unused = split[7];
        this->comment = split[8]; // Comment in single view

        this->value = this->value.simplified().remove(" ");
        this->value = this->value.remove("<").remove(">").remove("+").remove("N").remove("?");
        if(this->value.endsWith("-"))
            this->value = this->value.remove("-");
        qDebug() << "p: " << this->param << "output: " << this->value;
        if(this->param == "9ALBO") {
            // mg/dl statt g/dl! FAKTOR 1000!
            double albo = this->value.toDouble();
            albo  /= 1000;
            qDebug() << "output is now: " << this->value;
            qDebug() << "albo is now: " << albo;
            this->value = QString::number(albo);
            qDebug() << "output is now: " << this->value;
        }
        this->success = true;

    }
}

QString LabValue::getValue(int delim) {
    QString output = this->value;
    if(output=="s.Bem")
        return output;
    if(delim== 0){
        output = output.replace(".",",");
    } else {
        output = output.replace(",",".");
    }
    return output;
}
