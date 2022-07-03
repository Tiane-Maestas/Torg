#include <helper.h>

QString dayOfWeek(QDate date)
{
    return days[date.dayOfWeek() - 1];
}

QString formatDate(QDate date)
{
    return date.toString("MM/dd/yyyy");
}

QDate toDate(QString date)
{
    int year = date.split("/")[2].toInt();
    int month = date.split("/")[0].toInt();
    int day = date.split("/")[1].toInt();
    return QDate(year, month, day);
}

QString findClosestTime(QString time)
{
    //Keep Hour
    QString first = time.split(":")[0];
    QString toReturn = first + ":";

    //Check minutes and set remainder of returned string
    QString second = time.split(":")[1];
    int toCheck = second.split(" ")[0].toInt();
    toReturn = (toCheck < 30) ? toReturn + "00 " + second.split(" ")[1]: toReturn + "30 " + second.split(" ")[1];

    return toReturn;
}

void alertUser(QString msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}

void delay(int n)
{
    QTime dieTime= QTime::currentTime().addMSecs(n);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
