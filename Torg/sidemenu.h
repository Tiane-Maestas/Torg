#ifndef SIDEMENU_H
#define SIDEMENU_H

#include <QWidget>
#include <QEvent>
#include <QDebug>

class SideMenu : QWidget
{
    public:
        explicit SideMenu(QWidget* parent = nullptr);
        ~SideMenu(){}
        //void set(QWidget* widgetToManage);
    private:
        //QWidget* widgetToManage;
        bool event(QEvent* event);
        SideMenu *ui;
};

#endif // SIDEMENU_H
