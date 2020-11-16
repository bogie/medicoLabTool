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
        this->value = this->value.remove("<").remove(">").remove("N").remove("?");
        QList<QString> urineParams;
        urineParams.append("9LEUU");
        urineParams << "9HBU" << "9BILU" << "9UROU" << "9TPU" << "9KETU" << "9NITU" << "9GLUU";
        if(this->value != "+" && !urineParams.contains(this->param))
            this->value = this->value.remove("+");

        if(urineParams.contains(this->param)) {
            int level = this->value.count("+");
            if(level>0)
                this->value = QString::number(level);
        }
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
    if(output=="s.Bem" || output == "Mat.fehlt" || output == "s.u.")
        return output;
    if(delim== 0){
        output = output.replace(".",",");
    } else {
        output = output.replace(",",".");
    }
    return output;
}
