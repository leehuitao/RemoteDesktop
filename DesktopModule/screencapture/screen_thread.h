#ifndef SCREENTHREAD_H
#define SCREENTHREAD_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include "capture/common.h"
#include "rtc_base/platform_thread_types.h"
#include "rtc_base/logging.h"

class CapThreadObServer {
public:
    CapThreadObServer() {}
    ~CapThreadObServer() {}
    virtual void CaptureFrame() = 0;
};

class capthread
{
public:
    capthread(CapThreadObServer* obs);
    ~capthread();

    void Start();
    void Stop();
    void Suspend();
    void Continue();

    bool IsStop() { return is_stop_; }
    bool IsSuspend() { return is_suspend_; }
    void CaptureFrame();
    void SetRate(int iRate);//每秒几帧
    int GetRate() { return iRate_; }
public:
    std::mutex					condition_mutex_;
    std::condition_variable		condition_;
    std::thread					capturthread_;
private:
    intptr_t					thread_id_;
    int							iRate_;
    bool						is_stop_;
    bool						is_suspend_;
    CapThreadObServer*			observer_;

};

#endif // SCREENTHREAD_H
