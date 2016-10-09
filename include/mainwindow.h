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

	void change_0();
	void change_1();
	void game_reset();
	void ui_update();
	void game_status_change();
	void next_round();


private:
	Ui::MainWindow *ui;

	bool game_status;

	QTimer *timer_ui;            // UI刷新  30Hz
	QTimer *timer_capture;       // 摄像头，定位  30h
	QTimer *timer_logic;         //  逻辑 10Hz
	QTimer *timer_communication; //  通讯 10hz

	LogReader logreader;
	GameData gameData;
	Game game;

	void init_gameData();
};

#endif // MAINWINDOW_H