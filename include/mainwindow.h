#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>

#include "logic/GameData.h"
#include "logic/Game.h"
#include "display/log_reader.hpp"

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
	void change_1();
	void change_2();
	void game_reset();
	void ui_update();
	void game_status_change();

private:
    Ui::MainWindow *ui;
    QTimer *timer_ui;
	bool game_status;

    LogReader logreader;
    GameData gameData;
    Game game;
};

#endif // MAINWINDOW_H
