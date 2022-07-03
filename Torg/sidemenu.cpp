#include <sidemenu.h>

//void SideMenu::set(QWidget* widgetToManage){
//    this->widgetToManage = widgetToManage;
//}

bool SideMenu::event(QEvent* event){
    event->accept();
    qDebug() << "Event";
    if(event->type() == QEvent::HoverEnter){
        qDebug() << "Mouse Entered";
        return true;
    }else if(event->type() == QEvent::HoverLeave){
        qDebug() << "Mouse Left";
        return true;
    }
    return false;
}
