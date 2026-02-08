#include "GameHallWidget.h"
#include "ui_GameHallWidget.h"

GameHallWidget::GameHallWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameHallWidget)
{
    ui->setupUi(this);

    //与NetworkManager通信信号槽连接
    NetworkManager* NM = NetworkManager::instance();
    connect(NM,&NetworkManager::userInfoResponse,this,&GameHallWidget::onUserInfoResult);
}

GameHallWidget::~GameHallWidget()
{
    delete ui;
}

void GameHallWidget::onUserInfoResult(const QJsonObject& data)
{
    int userId = data["userId"].toInt();
    QString userName = data["userName"].toString();
    int userPoint = data["userPoint"].toInt();
    int totalCount = data["totalCount"].toInt();
    int winCount = data["winCount"].toInt();

    // 将信息格式化并显示到 QTextBrowser
    QString text = QString("用户ID: %1, 用户名: %2，用户积分：%3，总对战场数：%4，获胜场数：%5").arg(userId).arg(userName).arg(userPoint).arg(totalCount).arg(winCount);
    ui->userinfo_textBrowser->setText(text);
}
