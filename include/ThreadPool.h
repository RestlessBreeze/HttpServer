#pragma once
#include "EventLoop.h"
#include "WorkerThread.h"
#include <vector>

using namespace std;

class ThreadPool
{
public:
    // 初始化线程池
    ThreadPool(EventLoop* mainLoop, int count);
    ~ThreadPool();

    // 启动线程池
    void run();

    // 取出线程池中某个子线程的反应堆实例
    EventLoop* takeWorkerEventLoop();

private:
    // 主线程的反应堆模型
    EventLoop* m_mainLoop;
    bool m_isStart;
    int m_threadNum;
    vector<WorkerThread*> m_workerThreads;
    int m_index;
};

