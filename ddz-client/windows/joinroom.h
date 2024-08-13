#ifndef JOINROOM_H
#define JOINROOM_H

#include "Infomation.pb.h"
#include <QDialog>

namespace Ui {
class JoinRoom;
}

enum class DialogType:char{Manual, Search};
class JoinRoom : public QDialog
{
    Q_OBJECT

public:
    explicit JoinRoom(DialogType type, QWidget *parent = nullptr);
    ~JoinRoom();

    void searchRoom();
    void joinRoom();

private:
    void encodeMessage(RequestCode code);

private:
    Ui::JoinRoom *ui;
};

#endif // JOINROOM_H
