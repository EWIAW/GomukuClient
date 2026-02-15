#ifndef GAMEHALLWIDGET_H
#define GAMEHALLWIDGET_H

#include <QWidget>
#include <QJsonObject>

#include "NetworkManager.h"

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
    void onStartMatchResult(bool result,QString reason);//开始匹配响应
    void onCancelMatchResult(bool result,QString reason);//取消匹配响应
    void onMatchResult(bool result,QString reason);//匹配成功响应

private:
    void startMatch();//开始匹配
    void cancelMatch();//取消匹配

private:
    Ui::GameHallWidget *ui;

    bool _isMatching;
    QString _userName;
    quint32 _userId;
    quint32 _userPoint;
    quint32 _totalCount;
    quint32 _winCount;
};

#endif // GAMEHALLWIDGET_H
