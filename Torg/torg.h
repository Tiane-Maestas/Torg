#ifndef TORG_H
#define TORG_H

#include <QMainWindow>

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
    void on_actionDay_View_triggered();

    void on_actionWeek_View_triggered();

    void on_actionMonth_View_triggered();

    void on_actionDay_Event_triggered();

    void on_actionWeek_Event_triggered();

    void on_actionSingle_Event_triggered();

private:
    Ui::Torg *ui;
};
#endif // TORG_H
