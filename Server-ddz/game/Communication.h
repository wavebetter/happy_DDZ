//
// Created by subingwen
//

#ifndef SERVER_DDZ_COMMUNICATION_H
#define SERVER_DDZ_COMMUNICATION_H
#include "Buffer.h"
#include "Codec.h"
#include "AesCrypto.h"
#include "MySqlConn.h"
#include "Room.h"
#include "RoomList.h"

class Communication
{
public:
    Communication();
    ~Communication();
    // 解析客户端发送过来的数据
    void parseRequest(Buffer* buf);
    // 设置回调
    using sendCallback = function<void(std::string)>;
    using deleteCallback = function<void()>;
    void setCallback(sendCallback func1, deleteCallback fucn2);
    // 处理秘钥分发
    void handleAesFenfa(Message* reqMsg, Message& resMsg);
    void handleRegister(Message* reqMsg, Message& resMsg);
    void handleLogin(Message* reqMsg, Message& resMsg);
    void handleAddRoom(Message* reqMsg, Message& resMsg);
    void handleLeaveRoom(Message* reqMsg, Message& resMsg);
    void handleSearchRoom(Message* reqMsg, Message& resMsg);
    void handleGoodBye(Message* reqMsg);
    void handleGameOver(Message* reqMsg);
    // 准备开始游戏
    void readyForPlay(std::string roomName, std::string data);
    // 发牌
    void dealCards(UserMap players);
    // 洗牌
    void initCards();
    // 随机取出一张牌
    std::pair<int, int> takeOneCard();
    // 转发数据
    void notifyOtherPlayers(std::string data, std::string roomName, std::string userName);
    // 重新开始游戏
    void restartGame(Message* reqMsg);
    // 开始游戏
    void startGame(std::string roomName, UserMap players);
private:
    sendCallback sendMessage;
    deleteCallback disconnect;
    AesCrypto* m_aes = nullptr;
    MySqlConn *m_mysql = nullptr;
    Room* m_redis = nullptr;
    std::multimap<int, int> m_cards;
};


#endif //SERVER_DDZ_COMMUNICATION_H
