#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QWidget>
#include <QImage>
#include <QEvent>

#include <opencv2/opencv.hpp>

#include "logic/GameData.h"
#include "logic/Game.h"
#include "display/log_reader.hpp"
#include "camera/camera_opencv.hpp"
#include "locator/locator.hpp"
#include "locator/mapper.hpp"
#include "communication/my_string.hpp"
#include "communication/serial_port.hpp"
#include "communication/time_stamp.hpp"
#include "airplane/dobby.hpp"

typedef enum { GameWaiting = 0, GameStart = 1, GamePause = 2, GameOver = 3 } GameControlStatus;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void closeEvent(QCloseEvent *event);

private slots:

	void change_0();
	void change_1();
	void game_reset();
	void ui_update();
	void game_status_change();
	void logic_update();
	void communicate_update();
	void capture_update();

private:
    void init_gameData();
    
    Ui::MainWindow *ui;

    GameControlStatus status;

	QTimer *timer_ui;            // UI刷新  30Hz
	QTimer *timer_capture;       // 摄像头，定位  30h
	QTimer *timer_logic;         //  逻辑 10Hz
	QTimer *timer_communication; //  通讯 10hz

    // logic
	LogReader logreader;
	GameData gameData;
	Game game;

    // communicate
	SerialPort *port;

	// capture
	QImage	    capture_image;
	QTimer      *capture_timer;
	Camera      camera;
	cv::Mat     capture_frame;
	Locator     locator;
	std::vector<cv::Point> locator_points;

    // locator 
    Mapper mapper;

    // airplane
    Dobby dobby;
};

#endif // MAINWINDOW_H