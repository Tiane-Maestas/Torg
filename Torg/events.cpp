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
    return 1;
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
}

SingleEvent::SingleEvent(const SingleEvent& event){
    this->title = event.title;
    this->startTime = event.startTime;
    this->endTime = event.endTime;
    this->notes = event.notes;
    this->repeat = event.repeat;
    this->reminder = event.reminder;
    this->color = event.color;
    this->concrete = event.concrete;
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
void DayEvent::deleteEvent(std::string title){
    this->eventMap.erase(title);
}

//TO-DO: deal with reption. only update single events if they
//have not been saved yet, if they have been updated, and they are not
//a clone(repeated version). Idea: push back all events.
//If one event meets the criteria to be re-written
//completely re-build the written data for the day and delete the single
//day in the file.
bool DayEvent::save(std::string path){
    //Create all single events
    std::vector<QJsonObject> events;
    for (auto it = this->eventMap.begin(); it != this->eventMap.end(); it++){
        QJsonObject event;

        QJsonValue title(QString::fromStdString(it->second.getTitle()));
        QJsonValue notes(QString::fromStdString(it->second.getNotes()));
        event.insert("title", title);
        event.insert("notes", notes);

        events.push_back(event);
    }

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
    QByteArray bytes = document.toJson(QJsonDocument::Indented);
    if(!file.open(QIODevice::WriteOnly)){
        return false; //file open failed
    }
    file.write(document.toJson());
    file.close();

    return true;
}
