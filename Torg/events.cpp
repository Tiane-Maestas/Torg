#include <events.h>
#include <sstream>
#include <QtCore>
#include <iostream>
#include <QIODevice>
#include <QFile> //Include somewhere else becuse it is also used in main

Reminder::Reminder(std::string date,  std::string reminder){
    this->date = date;
    return; //TO-DO
    std::istringstream string_stream(reminder);
    std::string num;
    while(getline(string_stream, num, ' ')){
        reminderList.push_back(stoi(num));
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
    this->color = color;
    this->concrete = concrete;
    this->duplicate = false;
    this->updated = false;
}

SingleEvent::SingleEvent(const SingleEvent& event,  bool isDuplicate){
    this->title = event.title;
    this->startTime = event.startTime;
    this->endTime = event.endTime;
    this->notes = event.notes;
    this->repeat = event.repeat;
    this->reminder = event.reminder;
    this->color = event.color;
    this->concrete = event.concrete;
    this->duplicate = isDuplicate;
    this->updated = false;
}

const std::string SingleEvent::toString() const{
    return this->title + ": " + this->notes;
}

DayEvent::DayEvent(std::string date, SingleEvent event){
    this->date = date;
    this->dayOfTheWeek = "Monday"; //TO-DO
    this->eventMap[event.getTitle()] = event;
}

void DayEvent::addEvent(SingleEvent event){
    this->eventMap[event.getTitle()] = event;
}

//TO-DO: delete from save file.
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

        //TO-DO
        QJsonValue title(QString::fromStdString(it->second.getTitle()));
        QJsonValue notes(QString::fromStdString(it->second.getNotes()));
        event.insert("title", title);
        event.insert("notes", notes);

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
