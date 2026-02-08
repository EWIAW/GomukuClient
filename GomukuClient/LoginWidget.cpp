#include "LoginWidget.h"
#include "ui_LoginWidget.h"

#include <QJsonObject>
#include <QMessageBox>

#include "NetworkManager.h"

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    //自身信号槽连接
    connect(ui->register_pushButton,&QPushButton::clicked,this,&LoginWidget::onRegisterButtonClicked);
    connect(ui->login_pushButton,&QPushButton::clicked,this,&LoginWidget::onLoginButtonClicked);

    //与NetworkManager通信信号槽连接
    NetworkManager* NM = NetworkManager::instance();
    connect(NM,&NetworkManager::loginResponse,this,&LoginWidget::onLoginResult);
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

//自身信号槽函数
void LoginWidget::onRegisterButtonClicked()
{
    emit gotoRegisterWidget_Signal();//发送信号告诉MainWidget跳转页面
}

void LoginWidget::onLoginButtonClicked()
{
    QString username = ui->username_lineEdit->text();
    QString password = ui->password_lineEdit->text();
    if (!username.isEmpty() && !password.isEmpty())
    {
        QJsonObject message;
        message["username"] = username;
        message["password"] = password;
        NetworkManager::instance()->sendMessage(ProtocolId::LOGIN_REQ,message);
    }
}

//与NetworkManager通信槽函数
void LoginWidget::onLoginResult(bool success,QString reason)
{
    QMessageBox::warning(this,"提示",reason);
    if(success)
    {
        emit gotoGameHallWidget_Signal();//发送信号告诉MainWidget跳转页面
    }
}
