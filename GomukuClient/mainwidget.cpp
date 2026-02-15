#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QVBoxLayout>
MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , m_stackWidget(new QStackedWidget(this))
{
    ui->setupUi(this);

    initAllWidget();//创建所有界面
    initAllGoto();//连接所有跳转信号
    NetworkManager::instance()->connectToServer("175.178.15.192", 3489);
    m_stackWidget->setCurrentWidget(m_loginWidget);//默认显示登录界面
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::initAllWidget()//创建所有界面
{
    setWindowTitle("五子棋客户端");
    resize(800, 600);

    // 布局管理器
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_stackWidget);
    setLayout(layout);


    m_registerWidget = new RegisterWidget(this);
    m_loginWidget = new LoginWidget(this);
    m_gamehallWidget = new GameHallWidget(this);
    m_gameroomWidget = new GameRoomWidget(this);

    m_stackWidget->addWidget(m_registerWidget);
    m_stackWidget->addWidget(m_loginWidget);
    m_stackWidget->addWidget(m_gamehallWidget);
    m_stackWidget->addWidget(m_gameroomWidget);
}

void MainWidget::initAllGoto()//连接所有跳转信号
{
    //注册界面的跳转
    connect(m_registerWidget,&RegisterWidget::gotoLoginWidget_Signal,this,&MainWidget::gotoLoginWidget);

    //登录界面的跳转
    connect(m_loginWidget,&LoginWidget::gotoRegisterWidget_Signal,this,&MainWidget::gotoRegisterWidget);
    connect(m_loginWidget,&LoginWidget::gotoGameHallWidget_Signal,this,&MainWidget::gotoGameHallWidget);

    //游戏大厅界面的跳转
    connect(m_gamehallWidget,&GameHallWidget::gotoGameRoomWidget_Signal,this,&MainWidget::gotoGameRoomWidget);

}

void MainWidget::gotoRegisterWidget()
{
    m_stackWidget->setCurrentWidget(m_registerWidget);
}

void MainWidget::gotoLoginWidget()
{
    m_stackWidget->setCurrentWidget(m_loginWidget);
}

void MainWidget::gotoGameHallWidget()
{
    m_stackWidget->setCurrentWidget(m_gamehallWidget);
}

void MainWidget::gotoGameRoomWidget()
{
    m_stackWidget->setCurrentWidget(m_gameroomWidget);
}
