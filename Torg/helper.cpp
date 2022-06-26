#include <helper.h>

QString dayOfWeek(QDate date){
    return days[date.dayOfWeek() - 1];
}

QString formatDate(QDate date){
    return date.toString("MM/dd/yyyy");
}

QString findClosestTime(QString time){
    //Keep Hour
    QString first = time.split(":")[0];
    QString toReturn = first + ":";

    //Check minutes and set remainder of returned string
    QString second = time.split(":")[1];
    int toCheck = second.split(" ")[0].toInt();
    toReturn = (toCheck < 30) ? toReturn + "00 " + second.split(" ")[1]: toReturn + "30 " + second.split(" ")[1];

    return toReturn;
}
