#pragma once
#include "EventLoop.h"
#include "Buffer.h"
#include "Channel.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Communication.h"

//#define MSG_SEND_AUTO

class TcpConnection
{
public:
    TcpConnection(int fd, EventLoop* evloop);
    ~TcpConnection();

    // 发送数据
    void addWriteTask(std::string data);
    // 释放资源
    void addDeleteTask();

    // 准备秘钥
    void prepareSecretKey();
    static int processRead(void* arg);
    static int processWrite(void* arg);
    static int destroy(void* arg);
private:
    string m_name;
    EventLoop* m_evLoop;
    Channel* m_channel;
    Buffer* m_readBuf;
    Buffer* m_writeBuf;
    Communication* m_reply = nullptr;
};