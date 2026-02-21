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
    LOGIN_ACK = 1004,    // 登录响应

    // // 匹配模块 2000-2999
    MATCH_START_REQ = 2001,  // 发起匹配请求
    MATCH_START_ACK = 2002,  // 发起匹配响应
    MATCH_CANCEL_REQ = 2003, // 取消匹配请求
    MATCH_CANCEL_ACK = 2004, // 取消匹配响应

    MATCH_PUSH = 2006,   // 匹配结果推送

    // // 对战模块 3000-3999
    CHESS_DOWN_REQ = 3001, // 落子请求
    CHESS_DOWN_ACK = 3002, // 落子响应
//    CHESS_SYNC = 3002, // 棋局同步推送
//    AME_OVER = 3003,  // 对局结束推送

    // // 聊天模块 4000-4999
    GAME_CHAT_REQ = 4001, // 局内聊天请求
    GAME_CHAT_ACK = 4002, // 局内聊天响应
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
    void registerResponse(bool success,QString reason);//注册响应信号
    void loginResponse(bool success,QString reason);//登录响应信号
    void userInfoResponse(const QJsonObject& jsondata);//用户信息响应信号

    void startMatchResponse(bool result,QString reason);//开始匹配响应
    void cancelMatchResponse(bool result,QString reason);//取消匹配响应
    void matchResultPush(bool result,QString reason);//匹配结果推送

    void getColorResponse(int color);//获取颜色响应
    void chessDownResponse(int x, int y, bool success, bool win);//落子响应

    void chatResponse(QString msg);//聊天响应信号

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();

private:
    explicit NetworkManager(QObject *parent = nullptr);
    void parseMessage(QJsonDocument doc);//将服务器传过来的数据进行解析分发

    QTcpSocket *m_socket;
    QByteArray m_buffer;
    static NetworkManager *m_instance;
};

#endif // NETWORKMANAGER_H
