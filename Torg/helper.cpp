#include <helper.h>

QString dayOfWeek(QDate date){
    return days[date.dayOfWeek() - 1];
}

QString formatDate(QDate date){
    return date.toString("MM/dd/yyyy");
}
