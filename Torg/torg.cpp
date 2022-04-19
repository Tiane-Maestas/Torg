#include "torg.h"
#include "ui_torg.h"

Torg::Torg(QWidget *parent): QMainWindow(parent), ui(new Ui::Torg){

    ui->setupUi(this);

    ui->Day_View->findChild<QLabel *>(nameMap["12:30 AM"])->setStyleSheet(colorMap["blue"]);
    ui->Day_View->findChild<QLabel *>(nameMap["7:00 AM"])->setStyleSheet(colorMap["green"]);

}

Torg::~Torg(){

    delete ui;
    //Delete events from events map

}

bool Torg::loadUserData(){

    QFile file(this->userDataPath);
    if( !file.open( QIODevice::ReadOnly ) ){
        return false; //file open failed
    }
    QJsonDocument allUserData = QJsonDocument::fromJson(file.readAll());
    QJsonArray allInfo = allUserData.array();

    for(auto it = allInfo.begin(); it != allInfo.end(); it++){
        //All days are saved as json objects with 1 key as the date
        QJsonObject currentDayObject = it->toObject();
        QString date = currentDayObject.keys()[0];
        //Single Events are stored in the QJsonValue of the day QJsonObject as a QJsonObject
        QJsonObject eventsInToday = currentDayObject.take(date).toObject();
        QStringList singleEvents = eventsInToday.keys();
        //For every event in today add it to a DayEvent
        DayEvent* today = new DayEvent(date);   //ALLOCATING DATA
        for(int i = 0; i < singleEvents.size(); i++){
            QJsonObject currentEvent = eventsInToday.take(singleEvents[i]).toObject();

            QString title = currentEvent.take("title").toString();
            QString startTime = currentEvent.take("start-time").toString(); //Could be done using the key values but we already know them.
            QString endTime = currentEvent.take("end-time").toString();     //QStringList values = currentEvent.keys();
            QString notes = currentEvent.take("notes").toString();          //for(int j = 0; j < values.size(); j++){
            QString reminder = currentEvent.take("reminder").toString();    //  qDebug() << currentEvent.take(values[j]).toString();
            QString repeat = currentEvent.take("repeat").toString();        //}
            QString color = currentEvent.take("color").toString();
            bool concrete = currentEvent.take("concrete").toBool();

            //Since these events are being loaded from a file we don't call needSave().
            SingleEvent eventToAdd(title, startTime, endTime, notes, repeat, date, reminder, color, concrete); //Maybe want to create this on the heap at somepoint
            today->addEvent(eventToAdd);
        }
        //Finally add our day event to the apps event map
        this->dayEvents[date] = today;
    }

    file.close();
    return true;

}

//Menu Controls
void Torg::on_actionDay_View_triggered(){ ui->stackedWidget->setCurrentWidget(ui->Day_View); }
void Torg::on_actionWeek_View_triggered(){ ui->stackedWidget->setCurrentWidget(ui->Week_View); }
void Torg::on_actionMonth_View_triggered(){ ui->stackedWidget->setCurrentWidget(ui->Month_View); }
void Torg::on_actionDay_Event_triggered(){ ui->stackedWidget->setCurrentWidget(ui->Create_Day); }
void Torg::on_actionWeek_Event_triggered(){ ui->stackedWidget->setCurrentWidget(ui->Create_Week); }
void Torg::on_actionSingle_Event_triggered(){ ui->stackedWidget->setCurrentWidget(ui->Create_Single); }

//Adding a single event
void Torg::on_pushButton_Add_Single_clicked()
{
    //Grab input variables
    QString title = ui->Create_Single->findChild<QLineEdit *>("lineEdit_Title_Single")->text();
    QString notes = ui->Create_Single->findChild<QTextEdit *>("textEdit_Notes_Single")->toPlainText();
    QString startTime = ui->Create_Single->findChild<QTimeEdit *>("timeEditStart_Single")->text();
    QString endTime = ui->Create_Single->findChild<QTimeEdit *>("timeEditEnd_Single")->text();
    QString date = ui->Create_Single->findChild<QDateEdit *>("dateEdit_Single")->text();
    QString repeat = ui->Create_Single->findChild<QComboBox *>("comboBox_Repeat_Single")->currentText();
    QString reminder = ui->Create_Single->findChild<QComboBox *>("comboBox_Reminder_Single")->currentText(); //Need to process at somepoint
    QString color = ui->Create_Single->findChild<QComboBox *>("comboBox_Color_Single")->currentText();
    bool concrete = ui->Create_Single->findChild<QRadioButton *>("radioButton_Concrete_Single")->isChecked();

    SingleEvent eventToAdd(title, startTime, endTime, notes, repeat, date, reminder, color, concrete);
    eventToAdd.needsSave(); //Need to manually call needs save because we are creating this event dynamically.

    //Add to day event map if it exists or create a new one if it doesn't
    if(this->dayEvents.contains(date)){
        this->dayEvents[date]->addEvent(eventToAdd);
    }else{
        this->dayEvents[date] = new DayEvent(date, eventToAdd);
    }

    //Finally save the new info to user file
    this->dayEvents[date]->save(this->userDataPath);
}

