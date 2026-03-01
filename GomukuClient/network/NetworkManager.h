#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QtEndian>
#include <QTcpSocket>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "ProtocolId.h"

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

    void userInfoResponse(const QJsonObject& jsondata);//用户所有信息响应信号
    void userBattleInfoResponse(const QJsonObject& jsondata);//用户对局结束后需要更新的信息

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
