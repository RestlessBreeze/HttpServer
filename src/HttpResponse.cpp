#include "HttpResponse.h"
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ResHeaderSize 16;

HttpResponse::HttpResponse()
{
    m_statusCode = StatusCode::Unknown;
    m_headers.clear();
    m_fileName = string();
    m_sendDataFunc = nullptr;
}

HttpResponse::~HttpResponse()
{

}

// 添加响应头
void HttpResponse::addHeader(const string key, const string value)
{
    if (key.empty() || value.empty())
    {
        return;
    }
    m_headers.insert(make_pair(key, value));
}

// 组织http响应数据
void HttpResponse::prepareMsg(Buffer* sendBuf, int socket)
{
    // 状态行
    char tmp[1024] = {0};
    int code = static_cast<int>(m_statusCode);
    sprintf(tmp, "HTTP/1.1 %d %s\r\n", (int)m_statusCode, m_info.at(code).data());
    sendBuf->appendString(tmp);
    // 响应头
    for (auto it = m_headers.begin(); it != m_headers.end(); it++)
    {
        sprintf(tmp, "%s: %s\r\n", it->first.data(), it->second.data());
        sendBuf->appendString(tmp);
    }
    // 空行
    sendBuf->appendString("\r\n");

#ifndef MSG_SEND_AUTO
    sendBuf->sendData(socket);
#endif

    // 回复的数据
    m_sendDataFunc(m_fileName, sendBuf, socket);
}
