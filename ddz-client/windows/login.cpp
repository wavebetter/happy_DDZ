#include "login.h"
#include "communication.h"
#include "datamanager.h"
#include "ui_login.h"
#include "gamemode.h"
#include <QCryptographicHash>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QThreadPool>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    setFixedSize(480, 350);

    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->homeBtn, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->regUser, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->netSetBtn, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(2);
    });

    // 数据校验
    QRegularExpression expreg("^[a-zA-Z0-9_]{3,16}$");
    QRegularExpressionValidator *val1 = new QRegularExpressionValidator(expreg, this);
    ui->userName->setValidator(val1);
    ui->regUserName->setValidator(val1);

    /*
     * 密码:
     *  1. 长度为4到12个字符
     *  2. 包含至少一个大写字母、小写字母、数字和特殊字符
    */
    expreg.setPattern("^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{4,20}$");
    QRegularExpressionValidator *val2 = new QRegularExpressionValidator(expreg, this);
    ui->password->setValidator(val2);
    ui->regPasswrod->setValidator(val2);

    // 手机号校验
    expreg.setPattern("^1[3456789]\\d{9}$");
    QRegularExpressionValidator *val3 = new QRegularExpressionValidator(expreg, this);
    ui->phone->setValidator(val3);

    // IP校验
    expreg.setPattern("^((\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])\\.){3}(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])$");
    QRegularExpressionValidator *val4 = new QRegularExpressionValidator(expreg, this);
    ui->ipAddr->setValidator(val4);

    // 端口校验
    expreg.setPattern("^(0|([1-9]\\d{0,3})|([1-5]\\d{4})|(6[0-4]\\d{3})|(65[0-4]\\d{2})|(655[0-2]\\d)|(6553[0-5]))$");
    QRegularExpressionValidator *val5 = new QRegularExpressionValidator(expreg, this);
    ui->port->setValidator(val5);

    // 处理按钮点击事件
    connect(ui->loginBtn, &QPushButton::clicked, this, &Login::onLogin);
    connect(ui->registerBtn, &QPushButton::clicked, this, &Login::onRegister);
    connect(ui->netOkBtn, &QPushButton::clicked, this, &Login::onNetOK);

    // 设置线程池最大的线程数量
    QThreadPool::globalInstance()->setMaxThreadCount(8);

    // test code
    // ui->userName->setText("hello");
    // ui->password->setText("1Aa*");

    loadUserInfo();
}

bool Login::verifyData(QLineEdit* edit)
{
    if(edit->hasAcceptableInput() == false)
    {
        edit->setStyleSheet("border: 2px solid red;");
        return false;
    }
    else
    {
        edit->setStyleSheet("none");
    }
    return true;
}

void Login::startConnect(Message *msg)
{
    if(!m_isConnected)
    {
        Communication *task = new Communication(*msg);
        connect(task, &Communication::connectFailed, this, [=](){
            QMessageBox::critical(this, "连接服务器", "连接服务器失败");
            m_isConnected = false;
        });
        connect(task, &Communication::loginOk, this, [=](){
            // 将用户名保存到单例对象
            DataManager::getInstance()->setUserName(ui->userName->text().toUtf8());
            // 保存用户名和密码
            saveUserInfo();
            // 显示游戏模式窗口-> 单机版, 网络版
            GameMode* mode = new GameMode;
            mode->show();
            accept();
        });
        connect(task, &Communication::registerOk, this, [=](){
            ui->stackedWidget->setCurrentIndex(0);
        });
        connect(task, &Communication::failedMsg, this, [=](QByteArray msg){
            QMessageBox::critical(this, "ERROR", msg);
        });
        m_isConnected = true;
        QThreadPool::globalInstance()->start(task);
        DataManager::getInstance()->setCommunication(task);
    }
    else
    {
        // 和服务器进行通信
        DataManager::getInstance()->getCommunication()->sendMessage(msg);
    }
}

void Login::onLogin()
{
    bool flag1 = verifyData(ui->userName);
    bool flag2 = verifyData(ui->password);
    if(flag1 && flag2)
    {
        Message msg;
        msg.userName = ui->userName->text().toUtf8();
        msg.reqcode = RequestCode::UserLogin;
        QByteArray passwd = ui->password->text().toUtf8();
        passwd = QCryptographicHash::hash(passwd, QCryptographicHash::Sha224).toHex();
        msg.data1 = passwd;
        // 连接服务器
        startConnect(&msg);
    }
}

void Login::onRegister()
{
    bool flag1 = verifyData(ui->regUserName);
    bool flag2 = verifyData(ui->regPasswrod);
    bool flag3 = verifyData(ui->phone);
    if(flag1 && flag2 && flag3)
    {
        Message msg;
        msg.userName = ui->regUserName->text().toUtf8();
        msg.reqcode = RequestCode::Register;
        QByteArray passwd = ui->regPasswrod->text().toUtf8();
        passwd = QCryptographicHash::hash(passwd, QCryptographicHash::Sha224).toHex();
        msg.data1 = passwd;
        msg.data2 = ui->phone->text().toUtf8();
        // 连接服务器
        startConnect(&msg);
    }
}

void Login::onNetOK()
{
    bool flag1 = verifyData(ui->ipAddr);
    bool flag2 = verifyData(ui->port);
    if(flag1 && flag2)
    {
        DataManager* instance = DataManager::getInstance();
        instance->setIP(ui->ipAddr->text().toUtf8());
        instance->setPort(ui->port->text().toUtf8());
    }
}

void Login::saveUserInfo()
{
    if(ui->savePwd->isChecked())
    {
        QJsonObject obj;
        obj.insert("user", ui->userName->text());
        obj.insert("passwd", ui->password->text());
        QJsonDocument doc(obj);
        QByteArray json = doc.toJson();
        // aes 加密
        AesCrypto aes(AesCrypto::AES_CBC_128, KEY.left(16));
        json = aes.encrypt(json);
        // 写文件
        QFile file("passwd.bin");
        file.open(QFile::WriteOnly);
        file.write(json);
        file.close();
    }
    else
    {
        QFile file("passwd.bin");
        file.remove();
    }
}

void Login::loadUserInfo()
{
    QFile file("passwd.bin");
    bool flag  = file.open(QFile::ReadOnly);
    if(flag)
    {
        ui->savePwd->setChecked(true);
        QByteArray all = file.readAll();
        AesCrypto aes(AesCrypto::AES_CBC_128, KEY.left(16));
        all = aes.decrypt(all);
        QJsonDocument doc = QJsonDocument::fromJson(all);
        QJsonObject obj = doc.object();
        QString name = obj.value("user").toString();
        QString passwd = obj.value("passwd").toString();
        ui->userName->setText(name);
        ui->password->setText(passwd);
    }
    else
    {
        ui->savePwd->setChecked(false);
    }
}

Login::~Login()
{
    delete ui;
}
