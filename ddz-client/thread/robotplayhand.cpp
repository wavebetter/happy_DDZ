#include "robotplayhand.h"
#include <QDebug>
#include <QThread>

RobotPlayHand::RobotPlayHand(Player* player, QObject *parent) : QObject(parent), QRunnable()
{
    m_player = player;
    setAutoDelete(true);
}

void RobotPlayHand::run()
{
    QThread::msleep(2000);
    m_player->thinkPlayHand();
}
