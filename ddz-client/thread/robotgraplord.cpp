#include "robotgraplord.h"
#include <QThread>

RobotGrapLord::RobotGrapLord(Player* player, QObject *parent) : QObject(parent), QRunnable()
{
    m_player = player;
    setAutoDelete(true);
}

void RobotGrapLord::run()
{
    QThread::msleep(2000);
    m_player->thinkCallLord();
}
