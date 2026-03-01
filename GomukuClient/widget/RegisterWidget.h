#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>

namespace Ui {
class RegisterWidget;
}

class RegisterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterWidget(QWidget *parent = nullptr);
    ~RegisterWidget();

signals:
    void gotoLoginWidget_Signal();

private slots:
    //自身信号槽函数
    void onRegisterButtonClicked();//点击注册

    //与NetworkManager通信槽函数
    void onRegisterResult(bool success,QString reason);
private:
    Ui::RegisterWidget *ui;
};

#endif // REGISTERWIDGET_H
