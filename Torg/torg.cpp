#include "torg.h"
#include "ui_torg.h"

Torg::Torg(QWidget *parent): QMainWindow(parent), ui(new Ui::Torg){

    ui->setupUi(this);

    //Get User Data
    this->loadUserData();

    //Launch app in Day View
    ui->stackedWidget->setCurrentWidget(ui->Day_View);

    //Get the date and setup display
    this->todaysDate = QDate::currentDate();
    this->workingDate = this->todaysDate;
    this->setWorkingDateLabels();
    this->setEventLabels();

    //Gonna need a configure startup method
    ui->progressBar_Single->setValue(0);

    //Set defaultColorFromTheme TODO

    //Testing
    //ui->Day_View->findChild<QLabel *>(nameMap["12:30 AM"])->setStyleSheet(colorMap["Blue"]);
    //ui->Day_View->findChild<QLabel *>(nameMap["7:00 AM"])->setStyleSheet(colorMap["Green"]);
}

Torg::~Torg(){

    delete ui;
    //Delete events from events map

}

void Torg::setWorkingDateLabels(){
    //Set custom labels to differentiate today, tomorrow, yesturday, and a random day
    if(this->todaysDate == this->workingDate){
        //Today
        ui->Day_View->findChild<QLabel *>("workingDayLabel")->setText("Today: " + formatDate(this->workingDate));

    }else if(this->todaysDate.addDays(1) == this->workingDate){
        //Tomorrow
        ui->Day_View->findChild<QLabel *>("workingDayLabel")->setText("Tomorrow: " + formatDate(this->workingDate));

    }else if(this->todaysDate.addDays(-1) == this->workingDate){
        //Yesturday
        ui->Day_View->findChild<QLabel *>("workingDayLabel")->setText("Yesturday: " + formatDate(this->workingDate));

    }else{
        //Random Day
        ui->Day_View->findChild<QLabel *>("workingDayLabel")->setText(dayOfWeek(this->workingDate) + ": " + formatDate(this->workingDate));
    }
}

void Torg::setEventLabels(){
    //Clear the event labels so they can be reset
    if(!labelsRecentlyCleared) this->clearEventLabels();
    labelsRecentlyCleared = true;
    if(!this->dayEvents.contains(formatDate(this->workingDate))){
        //Do nothing if there are no events that day
        return;
    }

    //For all single events in the current day set the labels for each scene
    for(auto it = dayEvents[formatDate(this->workingDate)]->eventMap.begin(); it != dayEvents[formatDate(this->workingDate)]->eventMap.end(); it++){
        //Setting the Dat View
        setDayViewTimePeriod(it->second);
    }

    //Labels need to be cleared next time
    labelsRecentlyCleared = false;
}

void Torg::clearEventLabels(){
    //Clearing Day View
    for(auto it : nameMap.keys()){
        //Clearing the Color
        ui->Day_View->findChild<QLabel *>(nameMap.value(it))->setStyleSheet(defaultColorFromTheme);
        //Text TODO
    }
}

void Torg::setDayViewTimePeriod(SingleEvent event){
    //For all time blocks between startTime and endTime set those labels the color of the event
    QStringList allTimeBlocks = event.getTimeBlocks();

    for(auto timeBlock : allTimeBlocks){
        //Set the labels for each time block of the event
        ui->Day_View->findChild<QLabel *>(nameMap[timeBlock])->setStyleSheet(colorMap[event.getColor()]); //Color
        //Text TODO
    }
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

//Methods for getting the create single event status to change progress bar
void Torg::on_lineEdit_Title_Single_textEdited(const QString &arg1)
{
    if(!sTitleChanged){
        sTitleChanged = true;
        singlePercentageDone += 25;
        this->updateSingleProgressBarStatus(); //Only add percentage if this is the first time the title has been changed
    }
}
void Torg::on_dateEdit_Single_dateChanged(const QDate &date)
{
    if(!sDateChanged){
        sDateChanged = true;
        singlePercentageDone += 25;
        this->updateSingleProgressBarStatus(); //Only add percentage if this is the first time the date has been changed
    }
}
void Torg::on_timeEditStart_Single_timeChanged(const QTime &time)
{
    if(!sStartChanged){
        sStartChanged = true;
        singlePercentageDone += 25;
        this->updateSingleProgressBarStatus(); //Only add percentage if this is the first time the start time has been changed
    }
}
void Torg::on_timeEditEnd_Single_timeChanged(const QTime &time)
{
    if(!sEndChanged){
        sEndChanged = true;
        singlePercentageDone += 25;
        this->updateSingleProgressBarStatus(); //Only add percentage if this is the first time the end time has been changed
    }
}

//Handling Progress Bars Status
void Torg::updateSingleProgressBarStatus(){
    //Create a queued connection (across threads) between the threads update bar method and the animate method. Only do this once so that only one message is sent at a time.
    if(!singleBarLinked){ singleBarLinked = true; connect(&sProgressBarThread, SIGNAL(updateBar(int)), this, SLOT(animateSingleProgressBar(int)), Qt::QueuedConnection); }
    sProgressBarThread.setTargetPercentage(singlePercentageDone);
    if(!sProgressBarThread.isRunning()){ sProgressBarThread.run(); } //Only run the thread if it isn't running
}

//Recieves Messages from the thread to increment the progress
void Torg::animateSingleProgressBar(int inc){
    ui->progressBar_Single->setValue(ui->progressBar_Single->value() + inc);
}

//Adding a single event
void Torg::on_pushButton_Add_Single_clicked()
{
    //User Error Handling
    {
        //They haven't changed the title
        if(!sTitleChanged){
            alertUser("Input a new Title for your Event.");
            return;
        }
        //They haven't changed the date
        if(!sDateChanged){
            alertUser("Input a new Date for your Event.");
            return;
        }
        //They haven't changed the start time
        if(!sStartChanged){
            alertUser("Input a new Start Time for your Event.");
            return;
        }
        //They haven't changed the end time
        if(!sEndChanged){
            alertUser("Input a new End Time for your Event.");
            return;
        }
    }

    //Grab input variables
    QString title = ui->Create_Single->findChild<QLineEdit *>("lineEdit_Title_Single")->text();
    QString notes = ui->Create_Single->findChild<QTextEdit *>("textEdit_Notes_Single")->toPlainText();
    QString startTime = ui->Create_Single->findChild<QTimeEdit *>("timeEditStart_Single")->text();
    QString endTime = ui->Create_Single->findChild<QTimeEdit *>("timeEditEnd_Single")->text();
    QDate date = ui->Create_Single->findChild<QDateEdit *>("dateEdit_Single")->date();
    QString repeat = ui->Create_Single->findChild<QComboBox *>("comboBox_Repeat_Single")->currentText();
    QString reminder = ui->Create_Single->findChild<QComboBox *>("comboBox_Reminder_Single")->currentText(); //Need to process at somepoint
    QString color = ui->Create_Single->findChild<QComboBox *>("comboBox_Color_Single")->currentText();
    bool concrete = ui->Create_Single->findChild<QRadioButton *>("radioButton_Concrete_Single")->isChecked();

    SingleEvent eventToAdd(title, startTime, endTime, notes, repeat, formatDate(date), reminder, color, concrete);
    eventToAdd.needsSave(); //Need to manually call needs save because we are creating this event dynamically and not from a file.

    //Add to day event map if it exists or create a new one if it doesn't
    if(this->dayEvents.contains(formatDate(date))){
        this->dayEvents[formatDate(date)]->addEvent(eventToAdd);
    }else{
        this->dayEvents[formatDate(date)] = new DayEvent(formatDate(date), eventToAdd);
    }

    //Revert Creation Boolean Statuses

    //Finally save the new info to user file
    this->dayEvents[formatDate(date)]->save(this->userDataPath);
    //Change to Day View with the working day on the date that was just created
    this->workingDate = date;
    this->setWorkingDateLabels();
    this->setEventLabels();
    ui->stackedWidget->setCurrentWidget(ui->Day_View);
}

//TODO Toggle Stylesheet
void Torg::on_actionTheme_triggered()
{
    QFile styleSheetFile(QDir::currentPath() + "/lightTheme.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    centralWidget()->setStyleSheet(styleSheet);
    styleSheetFile.close();
}


void Torg::on_incButton_clicked()
{
    this->workingDate = this->workingDate.addDays(1);
    this->setWorkingDateLabels();
    this->setEventLabels();
}


void Torg::on_decButton_clicked()
{
    this->workingDate = this->workingDate.addDays(-1);
    this->setWorkingDateLabels();
    this->setEventLabels();
}
