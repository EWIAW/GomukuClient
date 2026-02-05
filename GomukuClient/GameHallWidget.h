#ifndef GAMEHALLWIDGET_H
#define GAMEHALLWIDGET_H

#include <QWidget>
#include <QJsonObject>

#include "NetworkManager.h"

namespace Ui {
class GameHallWidget;
}

class GameHallWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameHallWidget(QWidget *parent = nullptr);
    ~GameHallWidget();

private slots:
    //与NetworkManager通信槽函数
    void onUserInfoResult(const QJsonObject& data);

private:
    Ui::GameHallWidget *ui;
};

#endif // GAMEHALLWIDGET_H
