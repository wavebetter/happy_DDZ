//
// Created by subingwen
//

#include "RoomList.h"
#include "Log.h"

RoomList *RoomList::getInstance()
{
    static RoomList obj;
    return &obj;
}

void RoomList::addUser(std::string roomName, std::string userName, callback sendMessage)
{
    std::lock_guard<std::mutex> locker(m_mutex);
    // 在当前map中搜索roomName
    if(m_roomMap.find(roomName) != m_roomMap.end())
    {
        // 找到了
        auto &value = m_roomMap[roomName];
        value.insert(make_pair(userName, sendMessage));
    }
    else
    {
        // 没找到
        UserMap value = {{userName, sendMessage}};
        m_roomMap.insert(make_pair(roomName, value));
    }
}

UserMap RoomList::getPlayers(std::string roomName)
{
    std::lock_guard<std::mutex> locker(m_mutex);
    auto item = m_roomMap.find(roomName);
    if(m_roomMap.end() == item)
    {
        return UserMap();
    }
    UserMap user = item->second;
    return user;
}

UserMap RoomList::getPartners(std::string roomName, std::string userName)
{
    auto players = getPlayers(roomName);
    if(players.size() > 1)
    {
        auto self = players.find(userName);
        if(self != players.end())
        {
            players.erase(self);
            return players;
        }
    }
    return UserMap();
}

void RoomList::removePlayer(std::string roomName, std::string userName)
{
    std::lock_guard<std::mutex> locker(m_mutex);
    // 找房间
    auto item = m_roomMap.find(roomName);
    if(item != m_roomMap.end())
    {
        // 找人
        auto players = item->second;
        auto it = players.find(userName);
        if(it != players.end() && players.size() > 1)
        {
            m_roomMap[roomName].erase(it);
        }
        else if(it != players.end() && players.size() == 1)
        {
            m_roomMap.erase(item);
        }
    }
}

void RoomList::removeRoom(std::string roomName)
{
    std::lock_guard<std::mutex> locker(m_mutex);
    // 找房间
    auto item = m_roomMap.find(roomName);
    if(item != m_roomMap.end())
    {
        m_roomMap.erase(item);
    }
}
