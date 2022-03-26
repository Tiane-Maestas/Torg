#include <events.h>
#include <sstream>
#include <QtCore>
#include <iostream>
#include <QIODevice>

Reminder::Reminder(std::string date,  std::string reminder){
    this->date = date;
    std::istringstream string_stream(reminder);
    std::string num;
    while(getline(string_stream, num, ' ')){
        reminderList.push_back(std::stoi(num));
    }
}

//TO-DO
bool Reminder::check(){
    return 0;
}

SingleEvent::SingleEvent(std::string title, std::string startTime, std::string endTime, std::string notes,
                    std::string repeat, std::string date,  std::string reminder, std::string color, bool concrete){
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

const std::string SingleEvent::toString() const{
    return this->title + ": " + this->notes;
}

DayEvent::DayEvent(std::string date, SingleEvent event){
    this->date = date;
    this->dayOfTheWeek = dayOfWeek(date);
    this->eventMap[event.getTitle()] = event;
}

void DayEvent::addEvent(SingleEvent event){
    this->eventMap[event.getTitle()] = event;
}

bool DayEvent::deleteEvent(std::string date, std::string path){

    QFile file(QString::fromStdString(path));
    if( !file.open( QIODevice::ReadWrite ) ){
        return false; //file open failed
    }
    QJsonDocument jsonOrginal = QJsonDocument::fromJson(file.readAll());
    QJsonArray allInfo = jsonOrginal.array();

    for(auto it = allInfo.begin(); it != allInfo.end(); it++){
        //All days are saved as json objects with 1 key as the date
        QStringList currentDayObject = it->toObject().keys(); //qDebug() << currentDayObject[0];

        if(QString::fromStdString(date).compare(currentDayObject[0]) == 0){
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

void DayEvent::removeEvent(std::string title){
    this->eventMap.erase(title);
}

bool DayEvent::save(std::string path){
    //Create all single events as Json Objects
    std::vector<QJsonObject> events;
    bool needsSave = false;
    for (auto it = this->eventMap.begin(); it != this->eventMap.end(); it++){

        //Don't add duplicate events
        if( it->second.getDuplicateStatus() ) continue;

        QJsonObject event;

        QJsonValue title(QString::fromStdString(it->second.getTitle()));
        QJsonValue notes(QString::fromStdString(it->second.getNotes()));
        QJsonValue startTime(QString::fromStdString(it->second.getStartTime()));
        QJsonValue endTime(QString::fromStdString(it->second.getEndTime()));
        QJsonValue repeat(QString::fromStdString(it->second.getRepeat()));
        QJsonValue reminder(QString::fromStdString(it->second.getRem()));
        QJsonValue color(QString::fromStdString(it->second.getColor()));
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
    this->deleteEvent(this->date, path);

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
    today.insert(QString::fromStdString(this->getDate()), todaysEvents);

    //Read existing information from the file to allow appending
    QFile file(QString::fromStdString(path));
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
