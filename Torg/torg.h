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

private:
    Ui::Torg *ui;
};
#endif // TORG_H
