#ifndef TORG_H
#define TORG_H

#include <QMainWindow>
#include <QMap>

#include "events.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Torg; }
QT_END_NAMESPACE

class Torg : public QMainWindow
{
    Q_OBJECT

public:
    Torg(QWidget *parent = nullptr);
    ~Torg();
    //Load user data from json file and populates the dayEvents hash map (unordered_map in std)
    void loadUserData(const std::filesystem::path path);

private slots:
    //Menu actions
    void on_actionDay_View_triggered();
    void on_actionWeek_View_triggered();
    void on_actionMonth_View_triggered();
    void on_actionDay_Event_triggered();
    void on_actionWeek_Event_triggered();
    void on_actionSingle_Event_triggered();

private:
    Ui::Torg *ui;

    //A map between dates and all the events in that day as DayEvent pointers. De-allocations needed for DayEvents
    QHash<QString, DayEvent*> dayEvents();

    //Label stylesheet color options (I would like for these to be const QStrings but it doesn't like that)
    const QMap<QString, QString> colorMap = {{"red", "QLabel { background-color : red; } QLabel:hover{background-color: #ffaa00;color: black;}"},
                                            {"blue", "QLabel { background-color : blue; } QLabel:hover{background-color: #ffaa00;color: black;}"},
                                            {"green", "QLabel { background-color : green; } QLabel:hover{background-color: #ffaa00;color: black;}"},
                                            {"yellow", "QLabel { background-color : yellow; } QLabel:hover{background-color: #ffaa00;color: black;}"},
                                            {"pink", "QLabel { background-color : pink; } QLabel:hover{background-color: #ffaa00;color: black;}"}};
    //Label name map (I would like for these to be const QStrings but it doesn't like that)
    const QMap<QString, QString> nameMap = {{"12:00 AM", "label_1200AM"}, {"12:30 AM", "label_1230AM"}, {"12:00 PM", "label_1200PM"}, {"12:30 PM", "label_1230PM"},
                                            {"11:00 AM", "label_1100AM"}, {"11:30 AM", "label_1130AM"}, {"11:00 PM", "label_1100PM"}, {"11:30 PM", "label_1130PM"},
                                            {"10:00 AM", "label_1200AM"}, {"10:30 AM", "label_1030AM"}, {"10:00 PM", "label_1000PM"}, {"10:30 PM", "label_1030PM"},
                                            {"9:00 AM", "label_900AM"}, {"9:30 AM", "label_930AM"}, {"9:00 PM", "label_900PM"}, {"9:30 PM", "label_930PM"},
                                            {"8:00 AM", "label_800AM"}, {"8:30 AM", "label_830AM"}, {"8:00 PM", "label_800PM"}, {"8:30 PM", "label_830PM"},
                                            {"7:00 AM", "label_700AM"}, {"7:30 AM", "label_730AM"}, {"7:00 PM", "label_700PM"}, {"7:30 PM", "label_730PM"},
                                            {"6:00 AM", "label_600AM"}, {"6:30 AM", "label_630AM"}, {"6:00 PM", "label_600PM"}, {"6:30 PM", "label_630PM"},
                                            {"5:00 AM", "label_500AM"}, {"5:30 AM", "label_530AM"}, {"5:00 PM", "label_500PM"}, {"5:30 PM", "label_530PM"},
                                            {"4:00 AM", "label_400AM"}, {"4:30 AM", "label_430AM"}, {"4:00 PM", "label_400PM"}, {"4:30 PM", "label_430PM"},
                                            {"3:00 AM", "label_300AM"}, {"3:30 AM", "label_330AM"}, {"3:00 PM", "label_300PM"}, {"3:30 PM", "label_330PM"},
                                            {"2:00 AM", "label_200AM"}, {"2:30 AM", "label_230AM"}, {"2:00 PM", "label_200PM"}, {"2:30 PM", "label_230PM"},
                                            {"1:00 AM", "label_100AM"}, {"1:30 AM", "label_130AM"}, {"1:00 PM", "label_100PM"}, {"1:30 PM", "label_130PM"}};
};
#endif // TORG_H
