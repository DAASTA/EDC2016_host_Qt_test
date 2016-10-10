#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qpixmap.h>
#include <qtextcodec.h>

const int SIZEPIC = 50;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    ,ui(new Ui::MainWindow)
    ,game("./data/test.txt")
    //,camera(1, "./data/hd_usb_camera.xml")
    , camera(0)
    , capture_timer(NULL)
{
    ui->setupUi(this);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    timer_ui = new QTimer(this);
    timer_logic = new QTimer(this);
    timer_ui->start(33);
    connect(timer_ui, SIGNAL(timeout()), this, SLOT(ui_update()));
    connect(timer_logic, SIGNAL(timeout()), this, SLOT(next_round()));

    connect(ui->pushButton_change_0, SIGNAL(clicked()), this, SLOT(change_0()));
    connect(ui->pushButton_change_1, SIGNAL(clicked()), this, SLOT(change_1()));
    connect(ui->pushButton_reset, SIGNAL(clicked()), this, SLOT(game_reset()));
    connect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(game_status_change()));

    ui->pic_car_0->setPixmap(QPixmap(":/image/car_0").scaled(SIZEPIC, SIZEPIC));
    ui->pic_car_1->setPixmap(QPixmap(":/image/car_1").scaled(SIZEPIC, SIZEPIC));
    ui->pic_target->setPixmap(QPixmap(":/image/star").scaled(SIZEPIC, SIZEPIC));
    ui->pic_air->setPixmap(QPixmap(":/image/ufo").scaled(SIZEPIC, SIZEPIC));
    ui->pic_car_0->setVisible(false);
    ui->pic_car_1->setVisible(false);
    ui->pic_target->setVisible(false);
    ui->pic_air->setVisible(false);
    ui->pic_prop->setVisible(false);
    init_gameData();

    if (!camera.isValid()) {
        ui->pic->setText("Invalid camera.");
        ui->Status->setText("Failed to load the camera.");
    }
    else {
        // set up capture_timer
        capture_timer = new QTimer(this);
        connect(capture_timer, SIGNAL(timeout()), this, SLOT(capture_update()));
        capture_timer->start(20);

        locator.addTargetColor(Locator::Red);
        locator.addTargetColor(Locator::Blue);
    }

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
    init_gameData();
    game = Game("./data/test.txt");
    timer_logic->stop();
    ui->pushButton_start->setText(tr("start"));
}

void MainWindow::ui_update()
{
    //ui->pic->setText(QString::number(gameData.targetHealth));
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
    int x0 = 510 - 127;
    int y0 = 329 - 127;
    ui->pic_car_0->move(x0 + gameData.carData[0].pos.x, y0 + gameData.carData[0].pos.y);
    ui->pic_car_1->move(x0 + gameData.carData[1].pos.x, y0 + gameData.carData[1].pos.y);
    ui->pic_air->move(x0 + gameData.planePoint.x, y0 + gameData.planePoint.y);
    if (gameData.targetHealth > 0)
    {
        ui->pic_target->setVisible(true);
        ui->pic_target->move(x0 + gameData.targetPoint.x, y0 + gameData.targetPoint.y);
    }
    else
    {
        ui->pic_target->setVisible(false);
    }
    ui->pic_prop->setPixmap(QPixmap(QString(":/image/prop_%1").arg(int(gameData.propType))).scaled(SIZEPIC, SIZEPIC));
    ui->pic_prop->move(x0 + gameData.propPoint.x, y0 + gameData.propPoint.y);
}

void MainWindow::game_status_change()
{
    if (ui->pushButton_start->text() == "start")
    {
        ui->pic_car_0->setVisible(true);
        ui->pic_car_1->setVisible(true);
        ui->pic_target->setVisible(true);
        ui->pic_air->setVisible(true);
        ui->pic_prop->setVisible(true);
        ui->pushButton_start->setText(tr("pause"));
        timer_logic->start(100);
    }
    else if (ui->pushButton_start->text() == "pause")
    {
        ui->pushButton_start->setText(tr("start"));
        timer_logic->stop();
    }
}

void MainWindow::next_round()
{
    game.Refresh(gameData);
    gameData = game.getGameData();
}

void MainWindow::init_gameData()
{
    gameData.round = 0;
    gameData.carData[0] = { 200,Point(0,0) };
    gameData.carData[1] = { 200,Point(0,0) };
    gameData.planePoint = Point(255, 255);
}

void MainWindow::capture_update()
{
    if (camera.isValid()) {
        capture_frame = camera.getFrame();
        if (!capture_frame.empty()) {

            // update status
            static const char* status_label_format = "%dfps"; // frame rate
            static char buffer[512];
            sprintf(buffer, status_label_format, camera.getFrameRate());
            ui->Status->setText(buffer);

            // locator
            locator.Refresh(capture_frame);
            locator_points = locator.getTargetPoints();

            cv::circle(capture_frame, locator_points[0], 5, cv::Scalar(0, 0, 255), -1);
            cv::circle(capture_frame, locator_points[1], 5, cv::Scalar(255, 0, 0), -1);

            // set the point
            gameData.carData[Red].pos.x = locator_points[0].x - 192.5;
            gameData.carData[Red].pos.y = locator_points[0].y - 112.5;

            gameData.carData[Blue].pos.x = locator_points[1].x - 192.5;
            gameData.carData[Blue].pos.y = locator_points[1].y - 112.5;


            // update image
            capture_image = QImage(
                (const uchar*)capture_frame.data, capture_frame.cols, capture_frame.rows, QImage::Format_RGB888)
                .rgbSwapped();
            ui->pic->setPixmap(QPixmap::fromImage(capture_image));
        }
    }
}