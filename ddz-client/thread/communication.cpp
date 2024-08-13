#include "communication.h"
#include "card.h"
#include "cards.h"
#include "datamanager.h"
#include "rsacrypto.h"
#include "taskqueue.h"
#include <QThread>
#include <QDebug>
#include <QDateTime>
#include <QDataStream>

Communication::Communication(Message& msg, QObject *parent)
    : QObject{parent}, m_msgInfo(msg)
{
    setAutoDelete(true);
}

Communication::~Communication()
{
    if(m_aes)
    {
        delete m_aes;
    }
}

void Communication::sendMessage(Message *msg, bool encrypt)
{
    Codec codec(msg);
    QByteArray data = codec.encodeMsg();
    // 加密数据
    if(encrypt)
    {
        data = m_aes->encrypt(data);
    }
    m_socket->sendMsg(data);
}

void Communication::parseRecvMessage()
{
    // 接收数据
    QByteArray data = m_socket->recvMsg(2);
    // qDebug() << "接收到的数据: " << data;
    if(data.isEmpty()) return;
    // 反序列化数据
    Codec codec(data);
    QSharedPointer<Message> ptr = codec.decodeMsg();
    qDebug() << "ResCode: " << (int)ptr->rescode << ", " << ptr->data1 << ", " << ptr->data2;
    switch(ptr->rescode)
    {
    case RespondCode::LoginOk:
        emit loginOk();
        qDebug() << "登录成功了...";
        break;
    case RespondCode::RegisterOk:
        emit registerOk();
        qDebug() << "注册成功了...";
        break;
    case RespondCode::RsaFenFa:
        handleRsaFenfa(ptr.get());
        break;
    case RespondCode::AesVerifyOK:
        m_aes = new AesCrypto(AesCrypto::AES_CBC_256, m_aesKey);
        sendMessage(&m_msgInfo);
        qDebug() << m_msgInfo.userName << m_msgInfo.reqcode;
        qDebug() << "Aes 的秘钥分发成功了...";
        break;
    case RespondCode::JoinRoomOK:
        DataManager::getInstance()->setRoomName(ptr->roomName);
        emit playerCount(ptr->data1.toInt());
        break;
    case RespondCode::DealCards:
        parseCards(ptr->data1, ptr->data2);
        break;
    case RespondCode::StartGame:
        emit startGame(ptr->data1);
        break;
    case RespondCode::OtherGrabLord:
    {
        Task t;
        t.bet = ptr->data1.toInt();
        TaskQueue::getInstance()->add(t);
        break;
    }
    case RespondCode::OtherPlayHand:
    {
        // data1-=> 数量, data2->数据
        Task t;
        QDataStream stream(ptr->data2);
        for(int i=0; i<ptr->data1.toInt(); ++i)
        {
            Card c;
            stream >> c;
            t.cs.add(c);
        }
        TaskQueue::getInstance()->add(t);
        break;
    }
    case RespondCode::SearchRoomOK:
    {
        bool flag = ptr->data1 == "true" ? true : false;
        emit roomExist(flag);
        break;
    }
    case RespondCode::OtherLeaveRoom:
    {
        int count = ptr->data1.toInt();
        emit somebodyLeave(count);
        break;
    }
    case RespondCode::Failed:
        qDebug() << "Error: " << ptr->data1;
    default:
        break;
    }
}

void Communication::handleRsaFenfa(Message* msg)
{
    RsaCrypto rsa;
    rsa.parseStringToKey(msg->data1, RsaCrypto::PublicKey);
    // 校验签名
    bool flag = rsa.verify(msg->data2, msg->data1);
    assert(flag);
    // 生成对称加密的秘钥
    m_aesKey = generateAesKey(KeyLen::L32);
    Message resMsg;
    resMsg.reqcode = RequestCode::AesFenfa;
    resMsg.data1 = rsa.pubKeyEncrypt(m_aesKey);
    resMsg.data2 = QCryptographicHash::hash(m_aesKey, QCryptographicHash::Sha224).toHex();    // 16进制数字串格式的哈希
    sendMessage(&resMsg, false);
}

QByteArray Communication::generateAesKey(KeyLen len)
{
    QByteArray time = QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm:ss.zzz").toUtf8();
    // 进行哈希计算
    QCryptographicHash hash(QCryptographicHash::Sha3_384);
    hash.addData(time);
    time = hash.result();
    time = time.left(len);
    return time;
}

void Communication::parseCards(QByteArray data1, QByteArray data2)
{
    // xx-xx#xxx-xx#xx-xx
    auto func = std::bind([=](QByteArray msg){
        auto lst = msg.left(msg.length()-1).split('#');
        Cards cs;
        for(const auto& item : lst)
        {
            auto sub = item.split('-');
            Card card(static_cast<Card::CardPoint>(sub.last().toInt()),
                      static_cast<Card::CardSuit>(sub.first().toInt()));
            cs.add(card);
        }
        return cs;
    }, std::placeholders::_1);
    Cards cards = func(data1);
    Cards last = func(data2);
    // 存储数据
    DataManager::getInstance()->setCards(cards, last);
}

void Communication::run()
{
    m_socket = new TcpSocket;
    // 连接服务器
    DataManager* obj = DataManager::getInstance();
    bool flag = m_socket->connectToHost(obj->getIP(), obj->getPort().toInt());
    qDebug() << "========== 通信的子线程id: " << QThread::currentThread() << "connected ? " << flag;
    if(!flag)
    {
        // 通知主线程连接诶失败了
        emit connectFailed();
        delete m_socket;
        return;
    }
    // 连接成功了
    while(!m_stop)
    {
        // 处理服务器回复的数据
        // 首先得到非对称加密的公钥, 发送加密之后的对称加密秘钥给服务器, 服务器回复接收秘钥成功的消息, 然后发送登录或者注册请求
        parseRecvMessage();
    }
    m_socket->disConnect();
    delete m_socket;
}
