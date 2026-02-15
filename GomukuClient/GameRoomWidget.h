#ifndef GAMEROOMWIDGET_H
#define GAMEROOMWIDGET_H

#include <QWidget>
#include <QJsonObject>
#include <QVector>

#include "NetworkManager.h"

#define BOARD_SIZE 15 // 棋盘大小
#define CELL_SIZE 30  // 格子大小
#define WHITE_CHESS 1 // 白方的棋子
#define BLACK_CHESS 2 // 黑方的棋子

namespace Ui {
class GameRoomWidget;
}

class GameRoomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameRoomWidget(QWidget *parent = nullptr);
    ~GameRoomWidget();

    void initGame();//初始化游戏棋盘

protected:
    void paintEvent(QPaintEvent *event) override;//重写paintEvent事件处理函数
    void mousePressEvent(QMouseEvent *event) override;//重写mousePressEvent事件处理函数

private slots:
    //自身信号槽函数
    void onSendButtonClicked();//点击发送消息按钮

    //与NetworkManager通信槽函数
    void onMessageResult(QString msg);//聊天消息响应

private:
    void drawBoard(QPainter &painter);      // 绘制棋盘网格
    void drawStarPoints(QPainter &painter); // 绘制天元和星位标记
    void drawStones(QPainter &painter);     // 绘制棋子

private:
    Ui::GameRoomWidget *ui;

    int _board[BOARD_SIZE][BOARD_SIZE];//棋盘
    int _selfColor;//己方棋子颜色
    int _otherColor;//对方棋子颜色
};

#endif // GAMEROOMWIDGET_H
