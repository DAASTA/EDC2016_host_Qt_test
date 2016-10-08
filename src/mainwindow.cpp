#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qpixmap.h>

const int SIZEPIC = 50;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    game("./data/test.txt")
{
    ui->setupUi(this);

    timer_ui = new QTimer(this);
    connect(timer_ui, SIGNAL(timeout()), this, SLOT(ui_update()));
	timer_ui->start(33);

    connect(ui->pushButton_change_0, SIGNAL(clicked()), this, SLOT(change_0()));
    connect(ui->pushButton_change_1, SIGNAL(clicked()), this, SLOT(change_1()));
    connect(ui->pushButton_reset, SIGNAL(clicked()), this, SLOT(game_reset()));
    connect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(game_status_change()));

	ui->pic_car_1->setPixmap(QPixmap(":/image/car_1").scaled(SIZEPIC, SIZEPIC));
	ui->pic_car_2->setPixmap(QPixmap(":/image/car_2").scaled(SIZEPIC, SIZEPIC));
	ui->pic_target->setPixmap(QPixmap(":/image/star").scaled(SIZEPIC, SIZEPIC));
	ui->pic_air->setPixmap(QPixmap(":/image/ufo").scaled(SIZEPIC, SIZEPIC));

    logreader.LoadMap("./data/test.txt");
    logreader.LoadLog("./data/log_sample.txt");

}

MainWindow::~MainWindow()
{
    delete ui;

    if (NULL != timer_ui) delete timer_ui;
}

void MainWindow::change_0()
{
    gameData.carData[0].health += ui->lineEdit_hpchange_0->text().toInt();
    ui->lineEdit_hpchange_0->clear();
}


void MainWindow::change_1()
{
	gameData.carData[1].health += ui->lineEdit_hpchange_1->text().toInt();
	ui->lineEdit_hpchange_1->clear();
}

void MainWindow::game_reset()
{
}

void MainWindow::ui_update()
{
    //game.Refresh(gameData);
    //gameData = game.getGameData();
    static int round = 0;
    gameData = logreader.getGameData(++round);

    ui->label_aircommond_0->setText(QString::number((int)gameData.carData[0].air_command));
    ui->label_color_0->setText(QString::number((int)(gameData.carData[0].color)));
    ui->label_x_0->setText(QString::number(gameData.carData[0].pos.x));
    ui->label_y_0->setText(QString::number(gameData.carData[0].pos.y));
    ui->label_hp_0->setText(QString::number(gameData.carData[0].health));
    ui->label_TeamName_0->setText("NO.1");
    ui->label_hp_1->setText(QString::number(gameData.carData[0].health));
    ui->label_aircommond_1->setText(QString::number((int)gameData.carData[1].air_command));
    ui->label_color_1->setText(QString::number((int)(gameData.carData[1].color)));
    ui->label_x_1->setText(QString::number(gameData.carData[1].pos.x));
    ui->label_y_1->setText(QString::number(gameData.carData[1].pos.y));
    ui->label_hp_1->setText(QString::number(gameData.carData[1].health));
    ui->label_TeamName_1->setText("NO.2");
    ui->label_prop_type->setText(QString::number(int(gameData.propType)));
    ui->label_prop_x->setText(QString::number(gameData.propPoint.x));
    ui->label_prop_y->setText(QString::number(gameData.propPoint.y));
    ui->label_tower_x->setText(QString::number(gameData.targetPoint.x));
    ui->label_tower_y->setText(QString::number(gameData.targetPoint.y));
    ui->label_tower_color->setText(QString::number((int)(gameData.planeStatus)));
    ui->label_air_status->setText(QString::number(int(gameData.planeStatus)));
    ui->label_air_x->setText(QString::number(gameData.planePoint.x));
    ui->label_air_y->setText(QString::number(gameData.planePoint.y));
    
	int x0 = ui->layoutWidget1->x() + ui->pic->x();
	int y0 = ui->layoutWidget1->y() + ui->pic->y() + 480 - SIZEPIC;
	ui->pic_car_1->move(x0 + gameData.carData[0].pos.x, y0 - gameData.carData[0].pos.y);
	ui->pic_car_2->move(x0 + gameData.carData[1].pos.x, y0 - gameData.carData[1].pos.y);
	ui->pic_air->move(x0 + gameData.planePoint.x, y0 - gameData.planePoint.y);
    if (gameData.targetHealth > 0) ui->pic_target->move(x0 + gameData.targetPoint.x, y0 - gameData.targetPoint.y);
	ui->pic_prop->setPixmap(QPixmap(QString(":/image/prop_%1").arg(int(gameData.propType))).scaled(SIZEPIC, SIZEPIC));
	ui->pic_prop->move(x0 + gameData.propPoint.x, y0 - gameData.propPoint.y);
}

void MainWindow::game_status_change()
{
}


