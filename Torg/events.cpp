#include <events.h>
#include <sstream>

Reminder::Reminder(QString date,  QString reminder){
    this->date = date;
    std::istringstream string_stream(reminder.toStdString());
    std::string num;
    //while(getline(string_stream, num, ' ')){
        //reminderList.push_back(std::stoi(num)); //Needs formatting for any string passed in.
    //}
}

//TO-DO
bool Reminder::check(){
    return 0;
}

SingleEvent::SingleEvent(QString title, QString startTime, QString endTime, QString notes,
                    QString repeat, QString date,  QString reminder, QString color, bool concrete){
    this->title = title;
    this->startTime = startTime;
    this->endTime = endTime;
    this->notes = notes;
    this->repeat = repeat;
    this->reminder = Reminder(date, reminder);
    this->rem = reminder;
    this->color = color;
    this->concrete = concrete;
    this->duplicate = false;
    this->updated = false;
}

SingleEvent::SingleEvent(const SingleEvent& event,  bool isDuplicate){
    this->title = event.getTitle();
    this->startTime = event.getStartTime();
    this->endTime = event.getEndTime();
    this->notes = event.getNotes();
    this->repeat = event.getRepeat();
    this->reminder = event.getReminder();
    this->rem = event.getRem();
    this->color = event.getColor();
    this->concrete = event.getConcrete();
    this->duplicate = isDuplicate;
    this->updated = false;
}

QStringList SingleEvent::getTimeBlocks(){
    //Add the start time
    QStringList list(this->startTime);

    //Get the current time and end time
    QString closestStartBlock = findClosestTime(this->startTime);
    QTime currTime(0, 0);
    currTime = currTime.fromString(closestStartBlock, "h:mm AP");
    QString closestEndBlock = findClosestTime(this->endTime);
    QTime endTime(0, 0);
    endTime = endTime.fromString(closestEndBlock, "h:mm AP");

    //While the currTime is less than the end time add 30 min and add to the list
    while(currTime < endTime){
        currTime = currTime.addSecs(1800); //Add 30 min in seconds
        list.append(currTime.toString("h:mm AP"));
    }

    return list;
}

const QString SingleEvent::toString() const{
    return this->title + ": " + this->notes;
}

DayEvent::DayEvent(QString date, SingleEvent event){
    int year = date.split("/")[2].toInt();
    int month = date.split("/")[0].toInt();
    int day = date.split("/")[1].toInt();
    this->date = QDate(year, month, day);
    this->dayOfTheWeek = dayOfWeek(this->date);
    this->eventMap[event.getTitle()] = event;
}

DayEvent::DayEvent(QString date){
    int year = date.split("/")[2].toInt();
    int month = date.split("/")[0].toInt();
    int day = date.split("/")[1].toInt();
    this->date = QDate(year, month, day);
    this->dayOfTheWeek = dayOfWeek(this->date);
}

void DayEvent::addEvent(SingleEvent event){
    this->eventMap[event.getTitle()] = event;
}

bool DayEvent::deleteEvent(QString date, QString path){

    QFile file(path);
    if( !file.open( QIODevice::ReadWrite ) ){
        return false; //file open failed
    }
    QJsonDocument jsonOrginal = QJsonDocument::fromJson(file.readAll());
    QJsonArray allInfo = jsonOrginal.array();

    for(auto it = allInfo.begin(); it != allInfo.end(); it++){
        //All days are saved as json objects with 1 key as the date
        QStringList currentDayObject = it->toObject().keys(); //qDebug() << currentDayObject[0];

        if(date.compare(currentDayObject[0]) == 0){
            allInfo.erase(it);
            //Because the end iterator changes after erasing.
            it--;
        }
    }

    //Clear contents
    file.resize(0);
    //Re-write everything else
    QJsonDocument document(allInfo);
    file.write(document.toJson());
    file.close();

    return true;
}

void DayEvent::removeEvent(QString title){
    this->eventMap.erase(title);
}

bool DayEvent::save(QString path){
    //Create all single events as Json Objects
    std::vector<QJsonObject> events;
    bool needsSave = false;
    for (auto it = this->eventMap.begin(); it != this->eventMap.end(); it++){

        //Don't add duplicate events
        if( it->second.getDuplicateStatus() ) continue;

        QJsonObject event;

        QJsonValue title(it->second.getTitle());
        QJsonValue notes(it->second.getNotes());
        QJsonValue startTime(it->second.getStartTime());
        QJsonValue endTime(it->second.getEndTime());
        QJsonValue repeat(it->second.getRepeat());
        QJsonValue reminder(it->second.getRem());
        QJsonValue color(it->second.getColor());
        QJsonValue concrete(it->second.getConcrete());
        event.insert("title", title);
        event.insert("notes", notes);
        event.insert("start-time", startTime);
        event.insert("end-time", endTime);
        event.insert("repeat", repeat);
        event.insert("reminder", reminder);
        event.insert("color", color);
        event.insert("concrete", concrete);

        events.push_back(event);

        //If the event needs saving re-write all the events for this day.
        if( it->second.getUpdatedStatus() ){
            needsSave = true;
        }
    }

    //Breaks out of function if no save is needed
    if(!needsSave){ return false; }

    //delete day save data from file so that it can be re-written
    this->deleteEvent(formatDate(this->date), path);

    //Add all single events to today
    QJsonObject allEvents;
    for(int i = 0; i < (int)events.size(); i++){
        QJsonValue event(events[i]);
        QString eventNum("event");
        eventNum.append(QString::number(i));
        allEvents.insert(eventNum, event);
    }

    //Create a single JSON object for todays date
    QJsonValue todaysEvents(allEvents);
    QJsonObject today;
    today.insert(formatDate(this->date), todaysEvents);

    //Read existing information from the file to allow appending
    QFile file(path);
    if( !file.open( QIODevice::ReadOnly ) ){
        return false; //file open failed
    }
    QJsonDocument jsonOrginal = QJsonDocument::fromJson(file.readAll());
    file.close();

    //Append todays information.
    QJsonArray allInfo = jsonOrginal.array();
    allInfo.push_back(today);

    //Create a document to write
    QJsonDocument document(allInfo);

    //Write all information.
    if(!file.open(QIODevice::WriteOnly)){
        return false; //file open failed
    }
    file.write(document.toJson());
    file.close();

    return true;
}
