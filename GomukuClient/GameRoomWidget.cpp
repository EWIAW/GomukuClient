#include "GameRoomWidget.h"
#include "ui_GameRoomWidget.h"

#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

GameRoomWidget::GameRoomWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameRoomWidget)
{
    ui->setupUi(this);
    initGame();
    //自身信号槽连接
    connect(ui->send_pushButton,&QPushButton::clicked,this,&GameRoomWidget::onSendButtonClicked);

    //与NetworkManager通信信号槽连接
    NetworkManager* NM = NetworkManager::instance();
    connect(NM,&NetworkManager::chatResponse,this,&GameRoomWidget::onMessageResult);
    connect(NM,&NetworkManager::getColorResponse,this,&GameRoomWidget::onGetColorResult);
    connect(NM,&NetworkManager::chessDownResponse,this,&GameRoomWidget::onChessDownResult);
}

GameRoomWidget::~GameRoomWidget()
{
    delete ui;
}

void GameRoomWidget::initGame()
{
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            _board[i][j] = 0;

    _play = false;
    _selfColor = _otherColor = 1;
}

void GameRoomWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);              // 创建画家对象
    painter.setRenderHint(QPainter::Antialiasing); // 启用抗锯齿，使线条更平滑

    drawBoard(painter);                  // 绘制棋盘网格
    drawStones(painter);                 // 绘制棋子
}

void GameRoomWidget::mousePressEvent(QMouseEvent *event)
{
    // 如果点击的不是左键 或 未轮到该玩家落子，则忽略事件
    if (event->button() != Qt::LeftButton || _play == false)
    {
        return;
    }

    // 获取鼠标点击的位置
    QPoint pos = event->pos();

    // 将屏幕坐标转换为棋盘坐标
    // 棋盘左上角在(25, 25)，每个格子大小为CELL_SIZE
    int row = (pos.y() - 10) / 30;      // 计算行号
    int col = (pos.x() - 10) / 30;      // 计算列号

    // 检查计算出的行列号是否在棋盘范围内
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
    {
        // 检查该位置是否已经有棋子
        if (_board[row][col] == 0)
        {
            // 在该位置放置当前玩家的棋子
            _board[row][col] = _selfColor;
            _play = false;
            update(); // 请求重绘界面，显示最新状态

            //发送落子信息
            QJsonObject message;
            message["x"] = row;
            message["y"] = col;
            NetworkManager::instance()->sendMessage(ProtocolId::CHESS_DOWN_REQ,message);
        }
    }
}

void GameRoomWidget::onSendButtonClicked()
{
    QString msg = ui->msg_lineEdit->text();
    if(msg.isEmpty())
    {
        QMessageBox::warning(this,"提示","发送消息不能为空");
    }
    else
    {
        ui->chat_textBrowser->append(msg);
        QJsonObject message;
        message["message"] = msg;
        NetworkManager::instance()->sendMessage(ProtocolId::GAME_CHAT_REQ,message);
    }
}

void GameRoomWidget::onMessageResult(QString msg)
{
    ui->chat_textBrowser->append(msg);
}

void GameRoomWidget::onGetColorResult(int color)
{
    if(color == 1)
    {
        _play = true;
        _selfColor = 1;
        _otherColor = 2;
        QMessageBox::warning(this,"提示","先手");
    }
    else
    {
        _play = false;
        _selfColor = 2;
        _otherColor = 1;
        QMessageBox::warning(this,"提示","后手");
    }
}

void GameRoomWidget::onChessDownResult(int x, int y, bool success, bool win)
{
    _board[x][y] = _otherColor;
    _play = true;
    update();

    if(success)
    {
        if(win)
        {
            QMessageBox::warning(this,"提示","恭喜你获胜了");
        }
        else
        {
            QMessageBox::warning(this,"提示","很遗憾，输了");
        }
    }
}

void GameRoomWidget::drawBoard(QPainter &painter)
{
    painter.setPen(QPen(Qt::black, 1)); // 设置画笔为黑色，线宽1像素

    // 绘制网格线：水平线和垂直线
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        // 绘制第i行的水平线
        // 从(25, 25+i*CELL_SIZE)到(25+(BOARD_SIZE-1)*CELL_SIZE, 25+i*CELL_SIZE)
        painter.drawLine(25, 25 + i * CELL_SIZE,
                         25 + (BOARD_SIZE - 1) * CELL_SIZE, 25 + i * CELL_SIZE);

        // 绘制第i列的垂直线
        // 从(25+i*CELL_SIZE, 25)到(25+i*CELL_SIZE, 25+(BOARD_SIZE-1)*CELL_SIZE)
        painter.drawLine(25 + i * CELL_SIZE, 25,
                         25 + i * CELL_SIZE, 25 + (BOARD_SIZE - 1) * CELL_SIZE);
    }

    // 绘制天元和星位标记
    drawStarPoints(painter);
}

void GameRoomWidget::drawStarPoints(QPainter &painter)
{
    painter.setBrush(Qt::black); // 设置填充色为黑色

    // 计算天元位置（棋盘中心）
    int center = (BOARD_SIZE - 1) / 2;
    // 绘制天元小圆点
    painter.drawEllipse(25 + center * CELL_SIZE - 3,
                        25 + center * CELL_SIZE - 3, 6, 6);

    // 四角星位的位置：3, 9, 12（相对于边界的距离）
    int x[2] = {25+3*CELL_SIZE-3,25+11*CELL_SIZE-3};
    int y[2] = {25+3*CELL_SIZE-3,25+11*CELL_SIZE-3};
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            painter.drawEllipse(x[i],y[j],6,6);
        }
    }
}

void GameRoomWidget::drawStones(QPainter &painter)
{
    // 遍历整个棋盘
    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int col = 0; col < BOARD_SIZE; ++col)
        {
            // 如果该位置有棋子
            if (_board[row][col] != 0)
            {
                // 计算棋子的中心坐标
                int x = 25 + col * CELL_SIZE;
                int y = 25 + row * CELL_SIZE;

                // 根据棋子颜色设置画刷和画笔
                if (_board[row][col] == 1)
                {
                    // 白棋：填充白色，边框黑色
                    painter.setBrush(Qt::white);
                    painter.setPen(Qt::black);
                }
                else if(_board[row][col] == 2)
                {
                    // 黑棋：填充和边框都是黑色
                    painter.setBrush(Qt::black);
                    painter.setPen(Qt::black);
                }

                // 绘制圆形棋子（直径24像素）
                painter.drawEllipse(x - 12, y - 12, 24, 24);
            }
        }
    }
}
