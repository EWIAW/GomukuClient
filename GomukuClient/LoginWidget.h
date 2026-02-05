#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

signals:
    void gotoRegisterWidget_Signal();
    void gotoGameHallWidget_Signal();

private slots:
    //自身信号槽函数
    void onRegisterButtonClicked();//跳转到注册页面
    void onLoginButtonClicked();//点击登录

    //与NetworkManager通信槽函数
    void onLoginResult(bool success);

private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
