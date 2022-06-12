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

#endif // HELPER_H
