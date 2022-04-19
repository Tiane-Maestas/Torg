#include "torg.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDir>

using std::filesystem::current_path;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Allows for translation into different langauges.
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Torg_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    //Configure app and show main window.
    QFile styleSheetFile(current_path() / "orange-blackStyleSheet.qss"); //qDebug() << QString::fromStdString(QDir::currentPath().toStdString());
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    a.setStyleSheet(styleSheet);
    styleSheetFile.close();

    QString iconPath = QDir::currentPath() + "/TorgLogo.png";
    a.setWindowIcon(QIcon(iconPath));

    //Testing
    SingleEvent event1("title1", "startTime1", "endTime1", "notes1", "repeat1", "TestDate",  "1 2 3 ", "color1", 0);
    SingleEvent event2("title2", "startTime2", "endTime2", "notes2", "repeat2", "TestDate",  "1 2 3 ", "color2", 0);
    SingleEvent event3("title3", "startTime3", "endTime3", "notes3", "repeat3", "TestDate",  "1 2 3 ", "color3", 0);
    event1.needsSave();
    event2.needsSave();
    event3.needsSave();
    DayEvent day("TestDate", event1);
    day.addEvent(event2);
    day.addEvent(event3);
    day.save(QDir::currentPath() + "/test.json");

    DayEvent day2("TestDate2", event1);
    day2.save(QDir::currentPath() + "/test.json");

    Torg w;
    w.loadUserData();
    w.testData();
    w.show();

    return a.exec();
}
