#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QQueue>
#include <QProgressBar>
#include <QDebug>

//The type of each callback in the queue
struct Task {int id; void(*function)(); };

//----For Me----
//Task task = {0, [](){
//                 for(int i = 0; i < 25; i++){
//                         Torg::animateSingleProgressBar(1);
//                     }
//                 }};
//----For Me----

//This class will run through its queue of callback methods and
//run them in the order that they were added.
class CallbackQueueThread : QThread
{
    public:
        void run();
        void queue(Task task);
    private:
        QQueue<Task> callbackQueue;
};

//Thread for animating a progress bar up "targetPercentage" amount
//So if the bar is at 25% and the target percentage is 25% then it will put the bar at 50%.
class ProgressBarAnimator : public QThread
{
    Q_OBJECT
    public:
        ProgressBarAnimator(){}
        ~ProgressBarAnimator(){}
        void setTargetPercentage(int target);
        void run();
        bool isRunning();
    signals:
        void updateBar(int amt);
    private:
        bool running = false;
        int targetPercentage;
        int currPercentage = 0;
        int delayInMs = 100; //Delay between each increments
};

#endif // THREAD_H
