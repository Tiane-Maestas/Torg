#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <QFile>
#include <QtCore>
#include <iostream>
#include <QIODevice>
#include <QDate>
#include <QTime>
#include <QMessageBox>

const QString days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

//Returns the day of the week given any date
QString dayOfWeek(QDate date);

//formats a QDate to the string representation I want
QString formatDate(QDate date);

//formats a QString date into a QDate
QDate toDate(QString date);

//Finds the closest top or bottom time to given time
QString findClosestTime(QString time);

//Creates an alert box to prompt the user
void alertUser(QString msg);

//Allows for delay when necessary.
void delay(int n);

#endif // HELPER_H
