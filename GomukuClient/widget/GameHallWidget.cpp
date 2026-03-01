#include "GameHallWidget.h"
#include "ui_GameHallWidget.h"

#include <QMessageBox>
#include <QDebug>
#include <QStyle>

GameHallWidget::GameHallWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameHallWidget),
    _isMatching(false),
    _countdownTimer(nullptr),
    _countdownSeconds(0)
{
    ui->setupUi(this);

    //初始化倒计时定时器
    _countdownTimer = new QTimer(this);
    connect(_countdownTimer, &QTimer::timeout, this, &GameHallWidget::onCountdown);

    //自身信号槽连接
    connect(ui->match_pushButton,&QPushButton::clicked,this,&GameHallWidget::onMatchButtonClicked);

    //与NetworkManager通信信号槽连接
    NetworkManager* NM = NetworkManager::instance();
    connect(NM,&NetworkManager::userInfoResponse,this,&GameHallWidget::onUserInfoResult);
    connect(NM,&NetworkManager::userBattleInfoResponse,this,&GameHallWidget::onUserBattleInfoResult);
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
    displayUserInfo();
}

void GameHallWidget::onUserBattleInfoResult(const QJsonObject &data)
{
    _userPoint = data["userPoint"].toInt();
    _totalCount = data["totalCount"].toInt();
    _winCount = data["winCount"].toInt();
    displayUserInfo();
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
        ui->match_pushButton->setProperty("matching", true);

        // 触发样式更新（重要！）
        ui->match_pushButton->style()->unpolish(ui->match_pushButton);
        ui->match_pushButton->style()->polish(ui->match_pushButton);
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
        ui->match_pushButton->setProperty("matching", false);

        // 触发样式更新（重要！）
        ui->match_pushButton->style()->unpolish(ui->match_pushButton);
        ui->match_pushButton->style()->polish(ui->match_pushButton);
    }
}

void GameHallWidget::onMatchResult(bool result, QString reason)
{
    if(result == false)
    {
        QMessageBox::warning(this,"提示",reason);
    }
    else
    {
        _isMatching = false;
        ui->match_pushButton->setText("开始匹配");
        ui->match_pushButton->setProperty("matching", false);
        
        //开始5秒倒计时
        _countdownSeconds = 5;
        _countdownTimer->start(1000); // 1000ms = 1秒
        
        //创建并显示倒计时对话框
        QMessageBox *countdownBox = new QMessageBox(this);
        countdownBox->setWindowTitle("匹配成功");
        countdownBox->setText(QString("匹配成功！将在 %1 秒后进入游戏房间...").arg(_countdownSeconds));
        countdownBox->setStandardButtons(QMessageBox::NoButton);
        countdownBox->setModal(true);
        countdownBox->show();
        
        //保存对话框指针，以便在倒计时槽函数中更新
        _countdownTimer->setProperty("countdownBox", QVariant::fromValue(countdownBox));
    }
}

void GameHallWidget::onCountdown()
{
    _countdownSeconds--;
    
    //获取并更新倒计时对话框
    QMessageBox *countdownBox = nullptr;
    QVariant boxVariant = _countdownTimer->property("countdownBox");
    if (boxVariant.isValid()) {
        countdownBox = boxVariant.value<QMessageBox*>();
    }
    
    if (_countdownSeconds > 0 && countdownBox) {
        countdownBox->setText(QString("匹配成功！将在 %1 秒后进入游戏房间...").arg(_countdownSeconds));
    } else {
        //倒计时结束
        _countdownTimer->stop();
        
        //关闭倒计时对话框
        if (countdownBox) {
            countdownBox->close();
            countdownBox->deleteLater();
        }
        
        //跳转到游戏房间
        emit gotoGameRoomWidget_Signal();
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

void GameHallWidget::displayUserInfo()
{
    // 将信息格式化并显示到 QTextBrowser
    QString text = QString("用户ID: %1, 用户名: %2，用户积分：%3，总对战场数：%4，获胜场数：%5").arg(_userId).arg(_userName).arg(_userPoint).arg(_totalCount).arg(_winCount);
    ui->userinfo_textBrowser->setText(text);
}
