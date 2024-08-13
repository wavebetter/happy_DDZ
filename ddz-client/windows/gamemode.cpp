#include "gamemode.h"
#include "codec.h"
#include "datamanager.h"
#include "joinroom.h"
#include "ui_gamemode.h"
#include "gamepanel.h"

#include <QCloseEvent>

GameMode::GameMode(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GameMode)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->information->setVisible(false);
    setFixedSize(800, 500);

    // 取出通信类的实例对象
    Communication* comm = DataManager::getInstance()->getCommunication();
    connect(comm, &Communication::playerCount, this, [=](int count){
        showInfo(count);
        ui->information->setVisible(true);
    });

    // 更新房间中的玩家数量 - 有玩家退出房间了
    connect(comm, &Communication::somebodyLeave, this, &GameMode::showInfo);

    // 开始游戏
    connect(comm, &Communication::startGame, this, [=](QByteArray msg){
        this->hide();
        GamePanel* panel = new GamePanel;
        connect(panel, &GamePanel::panelClose, this, &GameMode::show);
        panel->show();
        // 初始化数据
        panel->initGamePanel(msg);
        // 断开信号槽连接
        disconnect(comm, &Communication::startGame, this, nullptr);
    });

    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [=](int index){
        if(index == 0)
        {
            ui->information->setVisible(false);
        }
    });

    connect(ui->singleModeBtn, &QPushButton::clicked, this, [=](){
        GamePanel* panel = new GamePanel;
        connect(panel, &GamePanel::panelClose, this, &GameMode::show);
        panel->show();
        hide();
        DataManager::getInstance()->setGameMode(DataManager::Single);
    });

    connect(ui->netModeBtn, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(1);
        DataManager::getInstance()->setGameMode(DataManager::Network);
    });

    // 自动加入某个房间
    connect(ui->autoBtn, &QPushButton::clicked, this, [=](){
        Message msg;
        msg.userName = DataManager::getInstance()->getUserName();
        msg.reqcode = RequestCode::AutoRoom;
        DataManager::getInstance()->getCommunication()->sendMessage(&msg);
        DataManager::getInstance()->setRoomMode(DataManager::Auto);
    });
    connect(ui->manualBtn, &QPushButton::clicked, this, [=](){
        JoinRoom room(DialogType::Manual);
        room.setWindowTitle("创建房间");
        room.exec();
    });
    connect(ui->searchBtn, &QPushButton::clicked, this, [=](){
        JoinRoom room(DialogType::Search);
        room.setWindowTitle("搜索房间");
        room.exec();
    });
}

GameMode::~GameMode()
{
    delete ui;
}

void GameMode::showInfo(int count)
{
    QString tip = QString("当前<%1>房间玩家人数为 %2 人, 正在等待其他玩家进入, 请稍候...")
                      .arg(DataManager::getInstance()->getRoomName().data())
                      .arg(count);
    ui->information->setText(tip);
}

void GameMode::closeEvent(QCloseEvent *ev)
{
    if(ui->stackedWidget->currentIndex() == 1)
    {
        ui->stackedWidget->setCurrentIndex(0);
        ev->ignore();
    }
    else
    {
        ev->accept();
        Message msg;
        msg.reqcode = RequestCode::Goodbye;
        msg.userName = DataManager::getInstance()->getUserName();
        msg.roomName = DataManager::getInstance()->getRoomName();
        DataManager::getInstance()->getCommunication()->sendMessage(&msg);
        DataManager::getInstance()->getCommunication()->stopLoop();

    }
}
