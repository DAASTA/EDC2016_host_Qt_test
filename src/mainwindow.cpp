#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qpixmap.h>
//#include <qtextcodec.h>

const int SIZEPIC = 50;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    ,ui(new Ui::MainWindow)
    ,game("./data/test.txt")
    ,mapper("./data/index_points.txt")
    ,camera(0, "./data/hd_usb_camera.xml")
    //, camera(0)
    ,capture_timer(NULL)
{
    ui->setupUi(this);

    port = new SerialPort( 3, 115200/*, protol*/);

    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));

    // game control
    status = GameWaiting;

    // timer_ui
    timer_ui = new QTimer(this);
    connect(timer_ui, SIGNAL(timeout()), this, SLOT(ui_update()));
    timer_ui->start(33);

    // timer_logic
    timer_logic = new QTimer(this);
    connect(timer_logic, SIGNAL(timeout()), this, SLOT(next_round()));
    //timer_logic->start(100); 

    // timer_communication
    timer_communication = new QTimer(this);
    connect(timer_communication, SIGNAL(timeout()), this, SLOT(communicate()));
    timer_communication->start(100);

    // ui
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
    
    // init logic
    init_gameData();

    // set up the camera
    if (!camera.isValid()) {
        ui->pic->setText("Invalid camera.");
        ui->Status->setText("Failed to load the camera.");
    }
    else {
        capture_timer = new QTimer(this);
        connect(capture_timer, SIGNAL(timeout()), this, SLOT(capture_update()));
        capture_timer->start(20);

        locator.addTargetColor(Locator::Red);
        locator.addTargetColor(Locator::Green);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
    
    if (NULL != timer_ui) delete timer_ui; 
    timer_ui = NULL;
    
    if (NULL != timer_capture) delete timer_capture;
    timer_capture = NULL;

    if (NULL != timer_logic) delete timer_logic;
    timer_logic = NULL;

    if (NULL != timer_communication) delete timer_communication;
    timer_communication = NULL;
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
    // ui 
    ui->pic_target->setVisible(false);
    ui->pic_air->setVisible(false);
    ui->pic_prop->setVisible(false);
    ui->pushButton_start->setText(tr("start"));

    // logic
    game = Game("./data/test.txt");
    init_gameData();

    // game control
    status = GameWaiting;

    // timer logic
    timer_logic->stop();
}

void MainWindow::ui_update()
{
    //ui->pic->setText(QString::number(gameData.targetHealth));
    ui->label_aircommond_0->setText(QString::number((int)gameData.carData[0].air_command));
    ui->label_color_0->setText(QString::number((int)(gameData.carData[0].color)));
    ui->label_x_0->setText(QString::number(gameData.carData[0].pos.x));
    ui->label_y_0->setText(QString::number(gameData.carData[0].pos.y));
    ui->label_hp_0->setText(QString::number(gameData.carData[0].health));
    ui->label_TeamName_0->setText("Team Red");

    ui->label_hp_1->setText(QString::number(gameData.carData[0].health));
    ui->label_aircommond_1->setText(QString::number((int)gameData.carData[1].air_command));
    ui->label_color_1->setText(QString::number((int)(gameData.carData[1].color)));
    ui->label_x_1->setText(QString::number(gameData.carData[1].pos.x));
    ui->label_y_1->setText(QString::number(gameData.carData[1].pos.y));
    ui->label_hp_1->setText(QString::number(gameData.carData[1].health));
    ui->label_TeamName_1->setText("Team Blue");
    
    ui->label_prop_type->setText(QString::number(int(gameData.propType)));
    ui->label_prop_x->setText(QString::number(gameData.propPoint.x));
    ui->label_prop_y->setText(QString::number(gameData.propPoint.y));
    ui->label_tower_x->setText(QString::number(gameData.targetPoint.x));
    ui->label_tower_y->setText(QString::number(gameData.targetPoint.y));
    ui->label_tower_color->setText(QString::number((int)(gameData.planeStatus)));
    ui->label_air_status->setText(QString::number(int(gameData.planeStatus)));
    ui->label_air_x->setText(QString::number(gameData.planePoint.x));
    ui->label_air_y->setText(QString::number(gameData.planePoint.y));
    
    int x0 = 199 - SIZEPIC / 2;
    int y0 = 114 - SIZEPIC / 2;
    cv::Point t;

    t = mapper.MapToImage(gameData.carData[Red].pos.x, gameData.carData[Red].pos.y);
    ui->pic_car_0->move(x0 + t.x, y0 + t.y);

    t = mapper.MapToImage(gameData.carData[Blue].pos.x, gameData.carData[Blue].pos.y);
    ui->pic_car_1->move(x0 + t.x, y0 + t.y);

    t = mapper.MapToImage(gameData.planePoint.x, gameData.planePoint.y);
    ui->pic_air->move(x0 + t.x, y0 + t.y);

    if (gameData.targetHealth > 0)
    {
        ui->pic_target->setVisible(true);

        t = mapper.MapToImage(gameData.targetPoint.x, gameData.targetPoint.y);
        ui->pic_target->move(x0 + t.x, y0 + t.y);
    }
    else
    {
        ui->pic_target->setVisible(false);
    }
    ui->pic_prop->setPixmap(QPixmap(QString(":/image/prop_%1").arg(int(gameData.propType))).scaled(SIZEPIC, SIZEPIC));
    t = mapper.MapToImage(gameData.propPoint.x, gameData.propPoint.y);
    ui->pic_prop->move(x0 + t.x, y0 + t.y);
}

void MainWindow::game_status_change()
{
    if (ui->pushButton_start->text() == "start") // TODO  change it to constant
    {
        // game control 
        status = GameStart;

        // ui
        ui->pic_car_0->setVisible(true);
        ui->pic_car_1->setVisible(true);
        ui->pic_target->setVisible(true);
        ui->pic_air->setVisible(true);
        ui->pic_prop->setVisible(true);
        ui->pushButton_start->setText(tr("pause"));

        // timer_logic
        timer_logic->start(100);
    }
    else if (ui->pushButton_start->text() == "pause")
    {
        // game control
        status = GamePause;

        // ui
        ui->pushButton_start->setText(tr("start"));

        // timer_logic
        timer_logic->stop();
    }
}

void MainWindow::next_round()
{
    game.Refresh(gameData);
    gameData = game.getGameData();
}

void MainWindow::communicate()
{
    char mes[4] = { 0 };
    mes[0] = (gameData.carData[0].long_attack_map ? 1 : 0) << 7
        | (gameData.carData[1].long_attack_map ? 1 : 0) << 6
        | (gameData.carData[0].short_attack_map ? 1 : 0) << 5
        | (gameData.carData[1].short_attack_map ? 1 : 0) << 4
        | (gameData.carData[0].attack_plane ? 1 : 0) << 3
        | (gameData.carData[1].attack_plane ? 1 : 0) << 2
        | status;

    mes[1] = (gameData.carData[0].out_of_range ? 1 : 0) << 7
        | (gameData.carData[1].out_of_range ? 1 : 0) << 6
        | (gameData.carData[0].air_command ? 1 : 0) << 5
        | (gameData.carData[1].air_command ? 1 : 0) << 4
        | (gameData.carData[0].heal_plane ? 1 : 0) << 3
        | (gameData.carData[1].heal_plane ? 1 : 0) << 2
        | (gameData.targetHealth > 0 ? 1 : 0) << 1
        | gameData.planeStatus;

    mes[2] = gameData.round & 0xFF;

    mes[3] = gameData.propType << 6
        | ((gameData.round >> 8) & 0x3F);

    char res[18] = {
        mes[0],
        mes[1],
        gameData.carData[0].pos.x,
        gameData.carData[0].pos.y,
        gameData.carData[1].pos.x,
        gameData.carData[1].pos.y,
        gameData.targetPoint.x,
        gameData.targetPoint.y,
        gameData.planePoint.x,
        gameData.planePoint.y,
        (unsigned char)(gameData.carData[0].health),
        (unsigned char)(gameData.carData[1].health),
        gameData.propPoint.x,
        gameData.propPoint.y,
        mes[2],
        mes[3],
        0x0D,
        0x0A }; 

    MyString ms(res, 18);

    //ui->Status->setText(ms.hex_str());

    port->send(res, 18);

}

void MainWindow::init_gameData()
{
    gameData.round = 0;
    gameData.carData[0] = { 200,Point(0,0) };
    gameData.carData[1] = { 200,Point(0,0) };
    gameData.planePoint = Point(255, 255);
}

static void setPoint(cv::Point& p, Point& pp, bool& out) {
    
    out = false;

    pp.x = p.x;
    if (p.x < 0) {
        out = true;
        pp.x = 0;
    }
    if (p.x > 255) {
        out = true;
        pp.x = 255;
    }

    pp.y = p.y;
    if (p.y < 0) {
        out = true;
        pp.y = 0;
    }
    if (p.y > 255) {
        out = true;
        pp.y = 255;
    }

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
            cv::Point t;

            if (locator_points[0] != cv::Point(0, 0)) {
                t = mapper.ImageToMap(locator_points[0]);
                setPoint(t, gameData.carData[Red].pos, gameData.carData[Red].out_of_range);
            }

            if (locator_points[1] != cv::Point(0, 0)) {
                t = mapper.ImageToMap(locator_points[1]);
                setPoint(t, gameData.carData[Blue].pos, gameData.carData[Blue].out_of_range);
            }

            // update image
            capture_image = QImage(
                (const uchar*)capture_frame.data, capture_frame.cols, capture_frame.rows, QImage::Format_RGB888)
                .rgbSwapped();
            ui->pic->setPixmap(QPixmap::fromImage(capture_image));
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {

    status = GameOver;

    communicate();

    QMainWindow::closeEvent(event);

}