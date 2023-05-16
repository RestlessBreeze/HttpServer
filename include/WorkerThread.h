#pragma once

#include <thread>
#include <string>
#include <condition_variable>
#include <mutex>
#include "EventLoop.h"

using namespace std;

class WorkerThread
{
public:
    WorkerThread(int index);
    ~WorkerThread();
    // 启动线程
    void run();

    inline EventLoop* getEventLoop()
    {
        return m_evLoop;
    }

private:
    thread* m_thread;
    thread::id m_threadID;
    string m_name;
    mutex m_mutex;
    condition_variable m_cond;
    class EventLoop* m_evLoop;

    void* running();
};