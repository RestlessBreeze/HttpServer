#pragma once
#include <map>
#include "Buffer.h"
#include "HttpResponse.h"
#include <functional>

using namespace std;

// 当前的解析状态
enum class ProcessState:char
{
    ParaseReqLine,
    ParaseReqHeaders,
    ParaseReqBody,
    ParaseReqDone
};

// 定义http请求结构体
class HttpRequest
{
public:
    HttpRequest();
    ~HttpRequest();

    // 重置
    void reset();

    inline ProcessState getState()
    {
        return m_curState;
    }

    string getHeader(const string key);

    void addHeader(string key, string value);

    // 解析请求行
    bool parseRequestLine(Buffer* readBuf);

    // 解析请求头
    bool parseRequestHeader(Buffer* readBuf);

    // 解析请求
    bool parseRequest(Buffer* readBuf, HttpResponse* response, Buffer* buffer, int socket);

    // 处理请求
    bool processRequest(HttpResponse* response);

    string decodeMsg(string from);

    string getFileType(const string name);

    static void sendFile(string fileName, Buffer* sendBuf, int cfd);

    static void sendDir(string dirName, Buffer* sendBuf, int cfd);

    inline void setMethod(string method)
    {
        m_method = method;
    }

    inline void setVersion(string version)
    {
        m_version = version;
    }

    inline void seturl(string url)
    {
        m_url = url;
    }

    inline void setState(ProcessState state)
    {
        m_curState = state;
    }

private:
    string m_method;
    string m_url;
    string m_version;
    map<string, string> m_reqHeaders;
    ProcessState m_curState;

private:
    char* splitRequestLine(const char* start, const char* end, const char* sub, std::function<void(string)> callback);
    int hexToDec(char c);
};