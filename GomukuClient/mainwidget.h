#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QStackedWidget>

#include "NetworkManager.h"
#include "RegisterWidget.h"
#include "LoginWidget.h"
#include "GameHallWidget.h"

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
    void gotoRegisterWidget();
    void gotoLoginWidget();
    void gotoGameHallWidget();

private:
    Ui::MainWidget *ui;
    QStackedWidget* m_stackWidget;

    RegisterWidget* m_registerWidget;
    LoginWidget* m_loginWidget;
    GameHallWidget* m_gamehallWidget;
};
#endif // MAINWIDGET_H
