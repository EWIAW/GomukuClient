#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QtEndian>
#include <QTcpSocket>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>

// 协议号定义：按模块分段，后续扩展新增即可
enum ProtocolId
{
    // 通用 0-999
    ACK = 0, // 通用响应包

    // 账号模块 1000-1999
    REGISTER_REQ = 1001, // 注册请求
    REGISTER_ACK = 1002, // 注册响应
    LOGIN_REQ = 1003,    // 登录请求
    LOGIN_ACK = 1004,    // 登录请求

    // // 匹配模块 2000-2999
    // MATCH_START = 2001,   // 发起匹配
    // MATCH_CANCEL = 2002,  // 取消匹配
    // MATCH_SUCCESS = 2003, // 匹配成功推送

    // // 对战模块 3000-3999
    // CHESS_DOWN = 3001, // 落子请求
    // CHESS_SYNC = 3002, // 棋局同步推送
    // AME_OVER = 3003,  // 对局结束推送

    // // 聊天模块 4000-4999
    // PROTO_ID_GAME_CHAT = 4001 // 局内聊天
};

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    //网络管理器基础函数
    static NetworkManager *instance();
    void connectToServer(const QString &host, quint16 port);
    void disconnectToServer();
    bool isConnected() const;
    void sendMessage(int protocol, const QJsonObject &data);

signals:
    void registerResponse(bool success,QString reason);//注册响应
    void loginResponse(bool success);//登录响应信号
    void userInfoResponse(const QJsonObject& jsondata);

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();

private:
    explicit NetworkManager(QObject *parent = nullptr);
    void parseMessage(QByteArray data);//将服务器传过来的数据进行解析分发

    QTcpSocket *m_socket;
    QByteArray m_buffer;
    static NetworkManager *m_instance;
};

#endif // NETWORKMANAGER_H
