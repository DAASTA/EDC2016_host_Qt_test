#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GameData.h"
#include <QDebug>
extern GameData gameData;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(33);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update()
{
    ui->label_aircommond_1->setText(QString(gameData.carData[0]._air_command));
    ui->label_color_1->setText(QString(gameData.carData[0]._color));
    ui->label_x_1->setText(QString::number(gameData.carData[0]._pos.x));
    ui->label_y_1->setText(QString::number(gameData.carData[0]._pos.y));
    ui->label_TeamName_1->setText("NO.1");
    ui->label_hp_2->setText(QString::number(gameData.carData[0]._health));
    ui->label_aircommond_2->setText(QString(gameData.carData[1]._air_command));
    ui->label_color_2->setText(QString(gameData.carData[0]._color));
    ui->label_x_2->setText(QString::number(gameData.carData[1]._pos.x));
    ui->label_y_2->setText(QString::number(gameData.carData[1]._pos.y));
    ui->label_hp_2->setText(QString::number(gameData.carData[1]._health));
    ui->label_TeamName_2->setText("NO.2");
    ui->label_prop_type->setText(QString::number(int(gameData._prop)));
    ui->label_prop_x->setText(QString::number(gameData._prop_pos.x));
    ui->label_prop_y->setText(QString::number(gameData._prop_pos.y));
    //ui->label_tower_x->setText(QString::number(gameData..x));
    //ui->label_tower_y->setText(QString::number(gameData..y));
    //ui->label_tower_color->setText(QString(QLatin1String(gameData.)));
    ui->label_air_status->setText(QString::number(int(gameData.planeStatus)));
    ui->label_air_x->setText(QString::number(gameData.planePos.x));
    ui->label_air_y->setText(QString::number(gameData.planePos.y));
}

void MainWindow::paintEvent(QPaintEvent *)
{
      QPainter painter(this);
      int x=ui->layoutWidget1->x()+ui->pic->x()+ui->label_x_1->text().toInt();
      int y=ui->layoutWidget1->y()+ui->pic->y()+380-ui->label_y_1->text().toInt();
      painter.drawPixmap(x,y,QPixmap(":/image/1.png").scaled(100,100));
}



