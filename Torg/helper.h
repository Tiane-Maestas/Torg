#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <QFile>
#include <QtCore>
#include <iostream>
#include <QIODevice>
#include <QDate>
#include <QTime>

const QString days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

//Returns the day of the week given any date
QString dayOfWeek(QDate date);

//formats a QDate to the string representation I want
QString formatDate(QDate date);

//Finds the closest top or bottom time to given time
QString findClosestTime(QString time);

#endif // HELPER_H
