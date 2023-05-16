#include "Dispatcher.h"
#include "SelectDispatcher.h"

SelectDispatcher::SelectDispatcher(EventLoop* evloop) : Dispatcher(evloop)
{
    FD_ZERO(&m_readSet);
    FD_ZERO(&m_writeSet);
    m_name = "Select";
}
SelectDispatcher::~SelectDispatcher()
{
    
}

void SelectDispatcher::setFdSet()
{
    if (m_channel->getEvent() & (int)FDEvent::ReadEvent)
    {
        FD_SET(m_channel->getSocket(), &m_readSet);
    }
    if (m_channel->getEvent() & (int)FDEvent::WriteEvent)
    {
        FD_SET(m_channel->getSocket(), &m_writeSet);
    }
}
void SelectDispatcher::clearFdSet()
{
    if (m_channel->getEvent() & (int)FDEvent::ReadEvent)
    {
        FD_CLR(m_channel->getSocket(), &m_readSet);
    }
    if (m_channel->getEvent() & (int)FDEvent::ReadEvent)
    {
        FD_CLR(m_channel->getSocket(), &m_readSet);
    }
}

// 添加
int SelectDispatcher::add()
{
    if (m_channel->getSocket() >= m_maxSize)
    {
        return -1;
    }
    setFdSet();
    return 0;
}
// 删除
int SelectDispatcher::remove()
{
    clearFdSet();
    // 通过 channel 释放对应的 TcpConnection 资源
    m_channel->destroyCallback(const_cast<void*>(m_channel->getArg()));

    return 0;
}
// 修改
int SelectDispatcher::modify()
{
    setFdSet();
    clearFdSet();
    return 0;
}
// 事件监测
int SelectDispatcher::dispatch(int timeout) // 单位: s
{
    struct timeval val;
    val.tv_sec = timeout;
    val.tv_usec = 0;
    fd_set rdtmp = m_readSet;
    fd_set wrtmp = m_writeSet; // 对原始数据进行备份。
    int count = select(m_maxSize, &rdtmp, &wrtmp, NULL, &val); // 传入传出参数
    if (count == -1)
    {
        perror("select");
        exit(0);
    }
    for (int i = 0; i < m_maxSize; ++i)
    {
        if (FD_ISSET(i, &rdtmp))
        {
            m_evLoop->eventActivate(i, (int)FDEvent::ReadEvent);
        }
        if (FD_ISSET(i, &wrtmp))
        {
            m_evLoop->eventActivate(i, (int)FDEvent::WriteEvent);
        }
    }
    return 0;
}




