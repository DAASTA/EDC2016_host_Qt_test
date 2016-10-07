#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    game("./data/test.txt")
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
    game.Refresh(Point(127,127), Point(127, 127), Point(127, 127));
    gameData = game.getGameData();

    ui->label_aircommond_1->setText(QString::number((int)gameData.carData[0].air_command));
    ui->label_color_1->setText(QString::number((int)(gameData.carData[0].color)));
    ui->label_x_1->setText(QString::number(gameData.carData[0].pos.x));
    ui->label_y_1->setText(QString::number(gameData.carData[0].pos.y));
    ui->label_hp_1->setText(QString::number(gameData.carData[0].health));
    ui->label_TeamName_1->setText("NO.1");
    ui->label_hp_2->setText(QString::number(gameData.carData[0].health));
    ui->label_aircommond_2->setText(QString::number((int)gameData.carData[1].air_command));
    ui->label_color_2->setText(QString::number((int)(gameData.carData[1].color)));
    ui->label_x_2->setText(QString::number(gameData.carData[1].pos.x));
    ui->label_y_2->setText(QString::number(gameData.carData[1].pos.y));
    ui->label_hp_2->setText(QString::number(gameData.carData[1].health));
    ui->label_TeamName_2->setText("NO.2");
    ui->label_prop_type->setText(QString::number(int(gameData.propType)));
    ui->label_prop_x->setText(QString::number(gameData.propPoint.x));
    ui->label_prop_y->setText(QString::number(gameData.propPoint.y));
    ui->label_tower_x->setText(QString::number(gameData.targetPoint.x));
    ui->label_tower_y->setText(QString::number(gameData.targetPoint.y));
    ui->label_tower_color->setText(QString::number((int)(gameData.planeStatus)));
    ui->label_air_status->setText(QString::number(int(gameData.planeStatus)));
    ui->label_air_x->setText(QString::number(gameData.planePoint.x));
    ui->label_air_y->setText(QString::number(gameData.planePoint.y));
}

void MainWindow::paintEvent(QPaintEvent *)
{
      QPainter painter(this);
      int x=ui->layoutWidget1->x()+ui->pic->x()+ui->label_x_1->text().toInt();
      int y=ui->layoutWidget1->y()+ui->pic->y()+380-ui->label_y_1->text().toInt();
      painter.drawPixmap(x,y,QPixmap(":/image/1.png").scaled(100,100));
}



