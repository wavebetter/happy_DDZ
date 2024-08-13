#include "TcpConnection.h"
#include "HttpRequest.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>

#include "Codec.h"
#include "RsaCrypto.h"
#include "Log.h"
#include "Room.h"

int TcpConnection::processRead(void* arg)
{
    TcpConnection* conn = static_cast<TcpConnection*>(arg);
    // 接收数据
    int socket = conn->m_channel->getSocket();
    int count = conn->m_readBuf->socketRead(socket);

    Debug("接收到的http请求数据: %s", conn->m_readBuf->data());
    if (count > 0)
    {
        // 解析斗地主数据
        conn->m_reply->parseRequest(conn->m_readBuf);
    }
    else if(count <= 0)
    {
        // 断开和客户端的连接诶
        conn->addDeleteTask();
        Debug("和客户端的连接已经断开了....");
    }
    return 0;
}

int TcpConnection::processWrite(void* arg)
{
    //Debug("开始发送数据了(基于写事件发送)....");
    TcpConnection* conn = static_cast<TcpConnection*>(arg);
    // 发送数据
    int count = conn->m_writeBuf->sendData(conn->m_channel->getSocket());
    if (count > 0)
    {
        // 判断数据是否被全部发送出去了
        if (conn->m_writeBuf->readableSize() == 0)
        {
            // 1. 不再检测写事件 -- 修改channel中保存的事件
            conn->m_channel->setCurrentEvent(FDEvent::ReadEvent);
            // 2. 修改dispatcher检测的集合 -- 添加任务节点
            conn->m_evLoop->addTask(conn->m_channel, ElemType::MODIFY);
            Debug("数据发送完毕...................");
        }
    }
    return 0;
}

int TcpConnection::destroy(void* arg)
{
    TcpConnection* conn = static_cast<TcpConnection*>(arg);
    if (conn != nullptr)
    {
        delete conn;
    }
    return 0;
}

TcpConnection::TcpConnection(int fd, EventLoop* evloop)
{
    m_evLoop = evloop;
    m_readBuf = new Buffer(10240);
    m_writeBuf = new Buffer(10240);
    m_reply = new Communication;

    auto delFunc = std::bind(&TcpConnection::addDeleteTask, this);
    auto writeFunc = std::bind(&TcpConnection::addWriteTask, this, std::placeholders::_1);
    m_reply->setCallback(writeFunc, delFunc);

    m_name = "Connection-" + to_string(fd);
    prepareSecretKey();
    m_channel = new Channel(fd, FDEvent::WriteEvent, processRead,
                            processWrite, destroy, this);
    evloop->addTask(m_channel, ElemType::ADD);
}

TcpConnection::~TcpConnection()
{
    if (m_readBuf && m_readBuf->readableSize() == 0 &&
        m_writeBuf && m_writeBuf->readableSize() == 0)
    {
        delete m_readBuf;
        delete m_writeBuf;
        m_evLoop->freeChannel(m_channel);
    }
    Debug("连接断开, 释放资源, gameover, connName: %s", m_name.data());
}

void TcpConnection::prepareSecretKey()
{
    Room redis;
    redis.initEnvironment();
    // 读公钥
    std::string pubkey = redis.rsaSecKey("PublicKey");
    // 发送数据
    Message msg;
    msg.rescode = RespondCode::RsaFenFa;
    msg.data1 = pubkey;
    RsaCrypto rsa;
    rsa.parseStringToKey(redis.rsaSecKey("PrivateKey"), RsaCrypto::PrivateKey);
    std::string data = rsa.sign(pubkey);
    msg.data2 = data;
    Codec codec(&msg);
    data = codec.encodeMsg();
    // 写数据
    m_writeBuf->appendPackage(data);
}

void TcpConnection::addWriteTask(std::string data)
{
    m_writeBuf->appendPackage(data);
#if 0 // 通过事件进行发送
    // 1. 检测写事件 -- 修改channel中保存的事件
    m_channel->setCurrentEvent(FDEvent::WriteEvent);
    // 2. 修改dispatcher检测的集合 -- 添加任务节点
        // 1. 检测写事件 -- 修改channel中保存的事件
    m_channel->setCurrentEvent(FDEvent::WriteEvent);
    // 2. 修改dispatcher检测的集合 -- 添加任务节点
    m_evLoop->addTask(m_channel, ElemType::MODIFY);
#else // 直接发送
    m_writeBuf->sendData(m_channel->getSocket());
#endif
}

void TcpConnection::addDeleteTask()
{
    Debug("断开了和客户端的连接, connName = %s", m_name.data());
    m_evLoop->addTask(m_channel, ElemType::DELETE);
}
