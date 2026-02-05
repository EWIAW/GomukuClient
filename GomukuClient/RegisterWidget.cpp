#include "RegisterWidget.h"
#include "ui_RegisterWidget.h"

#include <QJsonObject>
#include <QMessageBox>

#include "NetworkManager.h"

RegisterWidget::RegisterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterWidget)
{
    ui->setupUi(this);

    //自身信号槽连接
    connect(ui->register_pushButton,&QPushButton::clicked,this,&RegisterWidget::onRegisterButtonClicked);

    //与NetworkManager通信信号槽连接
    NetworkManager* NM = NetworkManager::instance();
    connect(NM,&NetworkManager::registerResponse,this,&RegisterWidget::onRegisterResult);
}

RegisterWidget::~RegisterWidget()
{
    delete ui;
}

//自身信号槽函数
void RegisterWidget::onRegisterButtonClicked()
{
    QString username = ui->username_lineEdit->text();
    QString password = ui->password_lineEdit->text();
    if (!username.isEmpty() && !password.isEmpty())
    {
        QJsonObject message;
        message["username"] = username;
        message["password"] = password;
        NetworkManager::instance()->sendMessage(Protocol::REGISTER_REQUEST,message);
    }
}

//与NetworkManager通信槽函数
void RegisterWidget::onRegisterResult(bool success,QString reason)
{
    if(success)
    {
        emit gotoLoginWidget_Signal();//发送信号告诉MainWidget跳转页面
    }
    else
    {
        QMessageBox::warning(this,"提示",reason);
    }
}
