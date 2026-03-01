#ifndef GAMEHALLWIDGET_H
#define GAMEHALLWIDGET_H

#include <QWidget>
#include <QJsonObject>
#include <QTimer>

#include "network/NetworkManager.h"

namespace Ui {
class GameHallWidget;
}

class GameHallWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameHallWidget(QWidget *parent = nullptr);
    ~GameHallWidget();

signals:
    void gotoGameRoomWidget_Signal();//跳转到游戏房间信号

private slots:
    //自身信号槽函数
    void onMatchButtonClicked();//点击匹配按钮

    //与NetworkManager通信槽函数
    void onUserInfoResult(const QJsonObject& data);//获取用户信息响应
    void onUserBattleInfoResult(const QJsonObject& data);//对局结束后，修改用户信息响应
    void onStartMatchResult(bool result,QString reason);//开始匹配响应
    void onCancelMatchResult(bool result,QString reason);//取消匹配响应
    void onMatchResult(bool result,QString reason);//匹配成功响应
    
    //倒计时槽函数
    void onCountdown();//倒计时处理函数

private:
    void startMatch();//开始匹配
    void cancelMatch();//取消匹配

    void displayUserInfo();//展示用户信息

private:
    Ui::GameHallWidget *ui;

    bool _isMatching;
    QString _userName;
    quint32 _userId;
    quint32 _userPoint;
    quint32 _totalCount;
    quint32 _winCount;
    
    //倒计时相关
    QTimer *_countdownTimer;
    int _countdownSeconds;
};

#endif // GAMEHALLWIDGET_H
