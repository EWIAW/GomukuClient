#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QStackedWidget>

#include "NetworkManager.h"
#include "RegisterWidget.h"
#include "LoginWidget.h"
#include "GameHallWidget.h"
#include "GameRoomWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void initAllWidget();//创建所有界面
    void initAllGoto();//连接所有跳转信号

private slots:
    void gotoRegisterWidget();//跳转到注册页面
    void gotoLoginWidget();//跳转到登录页面
    void gotoGameHallWidget();//跳转到游戏大厅页面
    void gotoGameRoomWidget();//跳转到游戏房间页面

private:
    Ui::MainWidget *ui;
    QStackedWidget* m_stackWidget;

    RegisterWidget* m_registerWidget;
    LoginWidget* m_loginWidget;
    GameHallWidget* m_gamehallWidget;
    GameRoomWidget* m_gameroomWidget;
};
#endif // MAINWIDGET_H
