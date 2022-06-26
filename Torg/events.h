#ifndef EVENTS_H
#define EVENTS_H

#include "helper.h"
#include <vector>
#include <map>

/*
 * Events are to be structured by days; single dates.
 * Every day will have a list of single events. The days
 * keep track of the date and day of the week. The single events
 * keep track of the specifics.
 * Days = big picture, SignleEvents = little picture.
*/

class Reminder
{
 public:
    //@param - "reminder" should be a string of ints separated by ' '.
    Reminder(QString date, QString reminder);
    Reminder(){ date = "none"; }
    //No data should be allocated
    ~Reminder(){}

    //checks to see if any of the reminder conditions have been met.
    //if a condtion is met it returns true.
    bool check();

 private:
    //Keeps track of the number of days before the date to remind the user.
    //This is a list so that mulitple reminders can be set.
    std::vector<int> reminderList;
    QString date;
};



//This class keeps track of repition of an event but does not apply any repition logic.
//Any repeated events of this event should be created outside of this class and passed to their own
//day events. Repition is only held for save logistics.
class SingleEvent
{
public:
    //Assign member varibles. Construct reminder object. @param - "reminder" should be a string of ints separated by ' '.
    SingleEvent(QString title, QString startTime, QString endTime, QString notes,
                QString repeat, QString date,  QString reminder, QString color, bool concrete);
    SingleEvent(const SingleEvent& event, bool isDuplicate);
    SingleEvent(){ this->title = "none"; this->startTime = "none"; this->endTime = "none"; this->notes = "none"; this->repeat = "none"; this->rem = "none";
                   this->reminder = Reminder(); this->color = "none"; this->concrete = 1; this->duplicate = false; this->updated = false; }
    //No data should be allocated
    ~SingleEvent(){}

    //Returns a string to represent the event on the display.
    const QString toString() const;

    //Getters
    const QString getStartTime() const{ return this->startTime; }
    const QString getEndTime() const{ return this->endTime; }
    const QString getTitle() const{ return this->title; }
    const QString getNotes() const{ return this->notes; }
    const QString getColor() const{ return this->color; }
    const QString getRem() const{ return this->rem; }
    const QString getRepeat() const{ return this->repeat; }
    bool getConcrete() const{ return this->concrete; }
    Reminder getReminder() const{ return this->reminder; }
    bool getUpdatedStatus() const{ return this->updated; }
    bool getDuplicateStatus() const{ return this->duplicate; }
    //Return a list of the time blocks between the start and end time of the event
    QStringList getTimeBlocks();

    //Setters
    void editTime(QString startTime, QString endTime){ this->startTime = startTime; this->endTime = endTime; this->updated = true; }
    void editColor(QString color){ this->color = color; this->updated = true; }
    void editNotes(QString notes){ this->notes = notes; this->updated = true; }
    void editTitle(QString title){ this->title = title; this->updated = true; }
    //Must call if event is created from code and not from a file load.
    void needsSave(){ this->updated = true; }

private:
    //Initialization/Save variables
    QString title; QString startTime; QString endTime; QString notes;
    QString rem; QString repeat; Reminder reminder; QString color; bool concrete;
    //Save functionality members
    bool updated; //Default: false -> if true then save to file. (assumes event was loaded from an existing file)
    bool duplicate; //Default: false -> if true then don't save. (base events are kept. any repitions of an
                    //event are calculated on creation based off of a base events repetition)
};



//This class allows for two day events to be created on the same date.
//In practice, this should not be allowed but will be handled outside of this class.
//NOTE: If a method taks a QString 'date' then must be converted to my representation of
//a date using the helper 'formatDate' function.
class DayEvent
{
 public:
    DayEvent(QString date, SingleEvent event);
    DayEvent(QString date);
    //No data should be allocated
    ~DayEvent(){}

    //All single events in this day.
    //Held as a map between title and event to allow for
    //deleting of single events and use of the map iterator.
    std::map<QString, SingleEvent> eventMap;

    //adds a SingleEvent to event map.
    void addEvent(SingleEvent event);

    //erases all data of the day in save file. returns true if delete was succesful.
    bool deleteEvent(QString date, QString path);

    //Removes a single event from the eventMap.
    void removeEvent(QString title);

    //Saves all events in the event map.
    //Writes each single event to the save file under this date.
    //Conditions that should permit a save:
    //-The event was updated.
    //-The event does not exist in the save file already. (handled by user if created from code and not loaded from a file)
    //-Not a duplicate event. (due to repition)
    //Returns true if saved succesfully.
    bool save(QString path);

    //Getters
    const QDate getDate() const{ return this->date; }
    const QString getDayOfTheWeek() const{ return this->dayOfTheWeek; }

 private:
    QDate date;
    QString dayOfTheWeek;
};

#endif // EVENTS_H
