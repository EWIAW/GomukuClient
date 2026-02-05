#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QtEndian>
#include <QTcpSocket>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>

enum Protocol
{
    REGISTER_REQUEST = 0,
    REGISTER_RESPONSE = 1,
    LOGIN_REQUEST = 2,
    LOGIN_RESPONSE = 3,
    MATCH_REQUEST = 4,
    MATCH_RESPONSE = 5
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
