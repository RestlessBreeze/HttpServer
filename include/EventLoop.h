#pragma once
#include "Dispatcher.h"
#include "Channel.h"
#include <thread>
#include <string>
#include <queue>
#include <map>
#include <mutex>

using namespace std;

// 处理该节点中的channel的方式
enum class ElemType:char{ADD, DELETE, MODIFY};

// 定义任务队列的节点
struct ChannelElement
{
    ElemType type;   // 如何处理该节点中的channel
    Channel* channel;
};

class Dispatcher;

class EventLoop
{
public:
    EventLoop();
    EventLoop(string threadName);
    ~EventLoop();

    // 启动反应堆模型
    int run();
    // 处理别激活的文件fd
    int eventActivate(int fd, int event);
    // 添加任务到任务队列
    int addTask(struct Channel* channel, ElemType type);
    // 处理任务队列中的任务
    int processTaskQ();
    // 处理dispatcher中的节点
    int Add(Channel* channel);
    int Remove(Channel* channel);
    int Modify(Channel* channel);
    // 释放channel
    int freeChannel(Channel* channel);

    static int readLocalMessage(void* arg);

    int readMessage();

    inline thread::id getThreadID()
    {
        return m_threadID;
    }

    inline string getThreadName()
    {
        return m_threadName;
    }

private:
    bool m_isQuit;
    // 该指针指向子类的实例
    Dispatcher* m_dispatcher;
    // 任务队列
    queue<ChannelElement*>m_taskQ;
    // map
    map<int, Channel*> m_channelMap;
    // 线程id, name, mutex
    thread::id m_threadID;
    string m_threadName;
    mutex m_mutex;
    int m_socketPair[2];  // 存储本地通信的fd 通过socketpair 初始化

private:    
    void taskWakeup();

};

