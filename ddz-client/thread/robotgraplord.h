#ifndef ROBOTGRAPLORD_H
#define ROBOTGRAPLORD_H

#include <QRunnable>
#include "player.h"

class RobotGrapLord : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit RobotGrapLord(Player* player, QObject *parent = nullptr);

protected:
    void run();

signals:
private:
    Player* m_player;

};

#endif // ROBOTGRAPLORD_H
