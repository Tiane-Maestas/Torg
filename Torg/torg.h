#ifndef TORG_H
#define TORG_H

#include <QMainWindow>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class Torg; }
QT_END_NAMESPACE

class Torg : public QMainWindow
{
    Q_OBJECT

public:
    Torg(QWidget *parent = nullptr);
    ~Torg();

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
    //Label stylesheet color options (I would like for these to be const QStrings but it doesn't like that)
    const QMap<QString, QString> colorMap = {{"red", "QLabel { background-color : red; } QLabel:hover{background-color: #ffaa00;color: black;}"},
                                            {"blue", "QLabel { background-color : blue; } QLabel:hover{background-color: #ffaa00;color: black;}"},
                                            {"green", "QLabel { background-color : green; } QLabel:hover{background-color: #ffaa00;color: black;}"},
                                            {"yellow", "QLabel { background-color : yellow; } QLabel:hover{background-color: #ffaa00;color: black;}"},
                                            {"pink", "QLabel { background-color : pink; } QLabel:hover{background-color: #ffaa00;color: black;}"}};
};
#endif // TORG_H
