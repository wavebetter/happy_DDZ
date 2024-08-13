#include "joinroom.h"
#include "codec.h"
#include "datamanager.h"
#include "ui_joinroom.h"

JoinRoom::JoinRoom(DialogType type, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::JoinRoom)
{
    ui->setupUi(this);

    ui->joinRoom->setFixedSize(100, 45);
    ui->joinRoom->setImage(":/images/addRoom-normal.png", ":/images/addRoom-hover.png",
                           ":/images/addRoom-pressed.png", ":/images/addRoom-disable.png");
    ui->joinRoom->setBtnDisable(true);

    const QString titles[] = {
        ":/images/info1.png",
        ":/images/info2.png"
    };
    int index = static_cast<int>(type);
    ui->describe->setPixmap(QPixmap(titles[index]));
    ui->stackedWidget->setCurrentIndex(index);
    setFixedSize(300, 180);

    Communication* comm = DataManager::getInstance()->getCommunication();
    connect(comm, &Communication::roomExist, this, [=](bool flag){
        if(flag)
        {
            ui->joinRoom->setBtnDisable(false);
        }
        else
        {
            ui->joinRoom->setBtnDisable(true);
        }
    });

    connect(ui->createRoom, &QPushButton::clicked, this,  &JoinRoom::joinRoom);
    connect(ui->searchRoom, &QPushButton::clicked, this, &JoinRoom::searchRoom);
    connect(ui->joinRoom, &QPushButton::clicked, this, &JoinRoom::joinRoom);
}

JoinRoom::~JoinRoom()
{
    delete ui;
}

void JoinRoom::searchRoom()
{
    encodeMessage(RequestCode::SearchRoom);
}

void JoinRoom::joinRoom()
{
    encodeMessage(RequestCode::ManualRoom);
    DataManager::getInstance()->setRoomMode(DataManager::Manual);
    accept();
}

void JoinRoom::encodeMessage(RequestCode code)
{
    Message msg;
    msg.reqcode = code;
    msg.userName = DataManager::getInstance()->getUserName();
    msg.roomName = ui->roomName->text().toUtf8();
    DataManager::getInstance()->getCommunication()->sendMessage(&msg);
}
