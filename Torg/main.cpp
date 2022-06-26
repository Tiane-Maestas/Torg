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
    QFile styleSheetFile(current_path() / "orange-blackStyleSheet.qss"); //qDebug() << QString::fromStdString(QDir::currentPath().toStdString()); darkTheme.qss
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    a.setStyleSheet(styleSheet);
    styleSheetFile.close();

    //Set the Icon
    QString iconPath = QDir::currentPath() + "/TorgLogo.png";
    a.setWindowIcon(QIcon(iconPath));

    Torg w;
    w.show();

    return a.exec();
}
