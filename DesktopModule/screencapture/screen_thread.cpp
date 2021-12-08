#include "screen_thread.h"
#include <QDebug>
void ThreadRun(void* param) {
    capthread* p = (capthread*)param;
    while (!p->IsStop()) {
        if(p->IsSuspend()){
            std::unique_lock<std::mutex> lock(p->condition_mutex_);
            p->condition_.wait(lock);
            lock.unlock();
        }
        RTC_LOG(INFO) << "p->CaptureFrame()";
        p->CaptureFrame();
        int iRate = p->GetRate();
        int i = 1000 / iRate;
        std::this_thread::sleep_for(std::chrono::milliseconds(i));
    }

    RTC_LOG(INFO) << " Capturer thread is exit.";
}


capthread::capthread(CapThreadObServer* obs) {
    observer_ = obs;
    is_stop_ = true;
    is_suspend_ = false;
    iRate_ = 10;
}

capthread::~capthread() {
    Stop();
}

void capthread::Start() {
    is_stop_ = false;
    capturthread_ = std::thread(ThreadRun, this);
}

void capthread::Stop() {
    is_stop_ = true;
    std::unique_lock<std::mutex> lock(condition_mutex_);
    condition_.notify_all();
    lock.unlock();
}

void capthread::Suspend() {

    is_suspend_ = true;
}

void capthread::Continue() {
    is_suspend_ = false;
    std::unique_lock<std::mutex> lock(condition_mutex_);
    condition_.notify_all();
    lock.unlock();
}

void capthread::SetRate(int iRate) {
    iRate_ = iRate;
}

void capthread::CaptureFrame() {
    if (observer_){
        qDebug()<<"capthread::CaptureFrame";
        observer_->CaptureFrame();
    }

}
