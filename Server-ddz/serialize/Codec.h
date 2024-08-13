//
// Created by subingwen
//

#ifndef SERVER_DDZ_CODEC_H
#define SERVER_DDZ_CODEC_H


#include <string>
#include "Infomation.pb.h"
#include <memory>
// 数据和protobuf消息体中的数据是对应的
// 序列化 -> 基于这个结构体中的数据进行
// 反序列化 -> 将解析出的数据存储到这个结构体中
struct Message
{
    std::string userName;
    std::string roomName;
    std::string data1;
    std::string data2;
    std::string data3;
    RequestCode reqcode;
    RespondCode rescode;
};

class Codec
{
public:
    // 序列化
    Codec(Message* msg);
    // 反序列化
    Codec(std::string msg);
    // 数据编码
    std::string encodeMsg();
    // 数据解码
    std::shared_ptr<Message> decodeMsg();
    // 重新加载数据
    void reLoad(std::string msg);
    void reLoad(Message* msg);

private:
    std::string m_msg;
    Information m_obj;  // 通过这个对象处理数据的序列化和反序列化
};



#endif //SERVER_DDZ_CODEC_H
