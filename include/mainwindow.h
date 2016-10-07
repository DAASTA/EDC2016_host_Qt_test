#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>

#include "logic/GameData.h"
#include "logic/Game.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void ui_update();
    void paintEvent(QPaintEvent *);

private:
    Ui::MainWindow *ui;

    QTimer *timer_ui;            // UI刷新  30Hz
    QTimer *timer_capture;       // 摄像头，定位  30hz

    QTimer *timer_logic;         //  逻辑 10Hz
    QTimer *timer_communication; //  通讯 10hz

    GameData gameData;
    Game game;
};

#endif // MAINWINDOW_H