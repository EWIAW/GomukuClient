#include "GameHallWidget.h"
#include "ui_GameHallWidget.h"

#include <QMessageBox>
#include <QDebug>

GameHallWidget::GameHallWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameHallWidget),
    _isMatching(false)
{
    ui->setupUi(this);

    //自身信号槽连接
    connect(ui->match_pushButton,&QPushButton::clicked,this,&GameHallWidget::onMatchButtonClicked);

    //与NetworkManager通信信号槽连接
    NetworkManager* NM = NetworkManager::instance();
    connect(NM,&NetworkManager::userInfoResponse,this,&GameHallWidget::onUserInfoResult);
    connect(NM,&NetworkManager::startMatchResponse,this,&GameHallWidget::onStartMatchResult);
    connect(NM,&NetworkManager::cancelMatchResponse,this,&GameHallWidget::onCancelMatchResult);
    connect(NM,&NetworkManager::matchResultPush,this,&GameHallWidget::onMatchResult);
}

GameHallWidget::~GameHallWidget()
{
    delete ui;
}

void GameHallWidget::onMatchButtonClicked()
{
    if(_isMatching == false)
    {
        startMatch();
    }
    else
    {
        cancelMatch();
    }
}

void GameHallWidget::onUserInfoResult(const QJsonObject& data)
{
    _userId = data["userId"].toInt();
    _userName = data["userName"].toString();
    _userPoint = data["userPoint"].toInt();
    _totalCount = data["totalCount"].toInt();
    _winCount = data["winCount"].toInt();

    // 将信息格式化并显示到 QTextBrowser
    QString text = QString("用户ID: %1, 用户名: %2，用户积分：%3，总对战场数：%4，获胜场数：%5").arg(_userId).arg(_userName).arg(_userPoint).arg(_totalCount).arg(_winCount);
    ui->userinfo_textBrowser->setText(text);
}

void GameHallWidget::onStartMatchResult(bool result,QString reason)
{
    if(result == false)
    {
        QMessageBox::warning(this,"提示",reason);
    }
    else
    {
        _isMatching = true;
        ui->match_pushButton->setText("取消匹配");
    }
}

void GameHallWidget::onCancelMatchResult(bool result,QString reason)
{
    if(result == false)
    {
        QMessageBox::warning(this,"提示",reason);
    }
    else
    {
        _isMatching = false;
        ui->match_pushButton->setText("开始匹配");
    }
}

void GameHallWidget::onMatchResult(bool result, QString reason)
{
//    qDebug()<<"进入onMatchSuccessResult函数";
    if(result == false)
    {
        QMessageBox::warning(this,"提示",reason);
    }
    else
    {
        _isMatching = false;
        ui->match_pushButton->setText("开始匹配");
        QMessageBox::warning(this,"提示",reason);
        emit gotoGameRoomWidget_Signal();//跳转到游戏房间
    }
}

void GameHallWidget::startMatch()
{
    QJsonObject message;
    message["userId"] = static_cast<qint64>(_userId);
    NetworkManager::instance()->sendMessage(ProtocolId::MATCH_START_REQ,message);
}

void GameHallWidget::cancelMatch()
{
    QJsonObject message;
    message["userId"] = static_cast<qint64>(_userId);
    NetworkManager::instance()->sendMessage(ProtocolId::MATCH_CANCEL_REQ,message);
}
