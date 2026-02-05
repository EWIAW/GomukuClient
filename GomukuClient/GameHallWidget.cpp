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
    int userid = data["userid"].toInt();
    QString username = data["username"].toString();

    // 将信息格式化并显示到 QTextBrowser
    QString text = QString("用户ID: %1, 用户名: %2").arg(userid).arg(username);
    ui->userinfo_textBrowser->setText(text);
}
