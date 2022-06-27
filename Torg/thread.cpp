#include "thread.h"
#include "helper.h"

void CallbackQueueThread::run(){
    while(!callbackQueue.isEmpty()){
        callbackQueue.dequeue().function();
    }
}

void CallbackQueueThread::queue(Task task){ callbackQueue.enqueue(task); }

void ProgressBarAnimator::run(){
    running = true;
    for(int i = currPercentage; i < targetPercentage; i++){
        emit this->updateBar(1);
        currPercentage++;
        delay(delayInMs);
    }
    running = false;
}

bool ProgressBarAnimator::isRunning(){ return running; }

void ProgressBarAnimator::setTargetPercentage(int target){ this->targetPercentage = target; }
