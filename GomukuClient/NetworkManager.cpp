#include "NetworkManager.h"

#include <QDebug>
#include <QThread>

NetworkManager *NetworkManager::m_instance = nullptr;

NetworkManager::NetworkManager(QObject *parent)
  : QObject(parent),
    m_socket(new QTcpSocket(this))
{
    connect(m_socket, &QTcpSocket::connected, this, &NetworkManager::onConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);
}

NetworkManager *NetworkManager::instance()
{
    if (!m_instance)
    {
        m_instance = new NetworkManager();
    }
    return m_instance;
}

void NetworkManager::connectToServer(const QString &host, quint16 port)
{
    m_socket->connectToHost(host, port);
}

void NetworkManager::disconnectToServer()
{
    if (m_socket->state() == QTcpSocket::ConnectedState)
    {
        m_socket->disconnectFromHost();
    }
}

bool NetworkManager::isConnected() const
{
    return m_socket->state() == QTcpSocket::ConnectedState;
}

void NetworkManager::sendMessage(int protocol, const QJsonObject &data)
{
    if (!isConnected())
    {
        qWarning() << "Not connected to server";
        return;
    }

    //构造json数据，即 协议号+data数据
    QJsonObject jsonData;
    jsonData["protocol"] = protocol;
    jsonData["data"] = data;

    //将上面的json数据转换成ByteArray
    QJsonDocument doc(jsonData);
    QByteArray jsonByteArray = doc.toJson(QJsonDocument::Compact);

    // 构造消息格式: [数据长度][协议号][JSON数据]
    QByteArray messagePacket;//真正要发送的数据
    qint32 jsonStringLength = jsonByteArray.size();
    qint32 bigEndianLen = qToBigEndian(jsonStringLength);//将长度转换为大端字节序
    messagePacket.append(reinterpret_cast<const char *>(&bigEndianLen), sizeof(quint32));
    messagePacket.append(jsonByteArray);

    qDebug()<<"发送实际数据大小 : "<<4+jsonStringLength<<" , 有效数据大小 : "<<jsonStringLength;
    qDebug()<<doc;
    m_socket->write(messagePacket);
    m_socket->flush();
}

void NetworkManager::onReadyRead()
{
    m_buffer.append(m_socket->readAll());

    //判断是否有完整的消息
    while (m_buffer.size() >= 4)
    {
        //获取消息体的大小
        int netLength;//网络字节序
        memcpy(&netLength, m_buffer.constData(), sizeof(int));
        int hostLength = qFromBigEndian(netLength);//主机字节序

        if (hostLength > m_buffer.size() - 4)
        {
            // 数据不完整，等待更多数据
            qDebug()<<"数据不完整 , 等待更多数据: "<<(m_buffer.size() - 4)<<"/"<<hostLength;
            qDebug()<<m_buffer;
            break;
        }

        // 提取一条完整JSON数据
        QByteArray jsonData = m_buffer.mid(4,hostLength);
        m_buffer.remove(0,4+hostLength);

        //将提取出来的数据转换成json
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);
        if (error.error != QJsonParseError::NoError)
        {
            qWarning() << "无法解析成json数据 : " << error.errorString();
            qWarning() << jsonData;
            return;
        }

        qDebug()<<"接收实际数据大小 : "<<4+hostLength<<" , 有效数据大小 : "<<hostLength;
        qDebug()<<doc;
        qDebug()<<" ";
//        QThread::msleep(1000);  // 暂停1秒
        // 解析成json类型并处理
        parseMessage(doc);
    }
}

void NetworkManager::onConnected()
{

}

void NetworkManager::onDisconnected()
{

}

void NetworkManager::parseMessage(QJsonDocument doc)
{
    QJsonObject message = doc.object();

    int protocolId = message["protocol"].toInt();
    QJsonObject jsondata = message["data"].toObject();
    if(protocolId == 1002)//注册响应
    {
        bool result = jsondata["success"].toBool();
        QString reason = jsondata["reason"].toString();
        emit registerResponse(result,reason);
    }
    else if(protocolId == 1004)//登录响应
    {
        bool result = jsondata["success"].toBool();
        QString reason = jsondata["reason"].toString();
        QJsonObject userInfo;
        userInfo["userId"] = jsondata["userId"];
        userInfo["userName"] = jsondata["userName"];
        userInfo["userPoint"] = jsondata["userPoint"];
        userInfo["totalCount"] = jsondata["totalCount"];
        userInfo["winCount"] = jsondata["winCount"];
        emit loginResponse(result,reason);//登录响应信号
        emit userInfoResponse(userInfo);//用户信息响应信号
    }
    else if(protocolId == 2002)// 发起匹配响应
    {
        bool result = jsondata["success"].toBool();
        QString reason = jsondata["reason"].toString();
        emit startMatchResponse(result,reason);
    }
    else if(protocolId == 2004)// 取消匹配响应
    {
        bool result = jsondata["success"].toBool();
        QString reason = jsondata["reason"].toString();
        emit cancelMatchResponse(result,reason);
    }
    else if(protocolId == 2006)// 匹配结果推送
    {
        bool result = jsondata["success"].toBool();
        QString reason = jsondata["reason"].toString();
        int color = jsondata["color"].toInt();
        emit matchResultPush(result,reason);
        emit getColorResponse(color);
    }
    else if(protocolId == CHESS_DOWN_ACK)
    {
        int x = jsondata["x"].toInt();
        int y = jsondata["y"].toInt();
        bool success = jsondata["success"].toBool();
        bool win;
        if(success)
            win = jsondata["win"].toBool();
        emit chessDownResponse(x,y,success,win);
    }
    else if(protocolId == 4002)//聊天响应信号
    {
        QString msg = jsondata["message"].toString();
        emit chatResponse(msg);
    }
}
