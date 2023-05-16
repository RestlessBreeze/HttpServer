#pragma once

#include <string>

using namespace std;

class Buffer
{
public:
    // 初始化
    Buffer(int size);
    ~Buffer();

    // 扩容
    void extendRoom(int size);

    // 得到剩余的可写的内存容量
    inline int writeableSize()
    {
        return m_capacity - m_writePos;
    }

    // 得到剩余的可读的内存容量
    inline int readableSize()
    {
        return m_writePos - m_readPos;
    }

    // 写内存 1. 直接写 2. 接收套接字数据
    int appendString(const char* data, int size);

    int appendString(const char* data);

    int appendString(const string data);

    int socketRead(int fd);

    // 根据\r\n取出一行，找到其在数据块的位置，返回该位置
    char* findCRLF();

    int sendData(int socket);

    inline char* data()
    {
        return m_data + m_readPos;
    }

    inline int readPosIncrease(int count)
    {
        m_readPos += count;
        return m_readPos;
    }

private:
    char* m_data;
    int m_capacity;
    int m_readPos = 0;
    int m_writePos = 0;
};
