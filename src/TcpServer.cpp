#include "TcpServer.h"
#include "TcpConnection.h"
#include "Log.h"

TcpServer::TcpServer(unsigned short port, int threadNum)
{
    m_port = port;
    m_mainLoop = new EventLoop;
    m_threadNum = threadNum;
    m_threadpool = new ThreadPool(m_mainLoop, threadNum);
    setListen();
}
TcpServer::~TcpServer()
{
    
}

    // 初始化监听
void TcpServer::setListen()
{
    // 1. 创建监听的fd
    m_lfd = socket(AF_INET, SOCK_STREAM, 0);  // AF_INET: IPV4 SOCK_STREAM 0 :TCP
    if (m_lfd == -1)
    {
        perror("socket");
        return;
    }

    // 2. 设置端口复用
    int opt = 1;
    int ret = setsockopt(m_lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
    if (ret == -1)
    {
        perror("setsockopt");
        return;
    }

    // 3. 绑定
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = INADDR_ANY;
    ret = bind(m_lfd, (struct sockaddr*) &addr, sizeof addr);
    if (ret == -1)
    {
        perror("bind");
        return;
    }

    // 4. 设置监听
    ret = listen(m_lfd, 128);
    if (ret == -1)
    {
        perror("listen");
        return;
    }
}

int TcpServer::acceptConnection(void* arg)
{
    TcpServer* server = static_cast<TcpServer*>(arg);

    // 建立连接
    int cfd = accept(server->m_lfd, NULL, NULL);

    // 从线程池中取出某个子线程的反应堆实例
    EventLoop* evLoop = server->m_threadpool->takeWorkerEventLoop();

    new TcpConnection(cfd, evLoop);

    return 0;
}

    // 启动服务器
void TcpServer::run()
{
    // 启动线程池
    m_threadpool->run();
    // channel实例初始化
    Channel* channel = new Channel(m_lfd, FDEvent::ReadEvent, acceptConnection, nullptr, nullptr, this);
    // 添加检测的任务 (此时只有监听的文件描述符，添加到主线程的eventLoop中)
    m_mainLoop->addTask(channel, ElemType::ADD);

    // 启动反应堆模型
    Debug("服务器程序已经启动...");
    m_mainLoop->run();
}