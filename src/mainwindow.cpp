#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qpixmap.h>

using namespace std;

const int SIZEPIC = 50;
const int SIZESYM = 90;
const string MAP_FILENAME = "./data/map_预审.txt";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    ,ui(new Ui::MainWindow)
    ,game(MAP_FILENAME)
    ,mapper("./data/index_points.txt")
    ,camera(1, "./data/hd_usb_camera.xml")
    //, camera(0)
    ,capture_timer(NULL)
{
    ui->setupUi(this);

    // Serial port
    char tail[3] = {0x0D, 0x0A, 0};
    SerialPortProtol protol(5, tail);
    port = new SerialPort(12, 115200, protol);

    // game control
    status = GameWaiting;

    // timer_ui
    timer_ui = new QTimer(this);
    connect(timer_ui, SIGNAL(timeout()), this, SLOT(ui_update()));
    timer_ui->start(33);

    // timer_logic
    timer_logic = new QTimer(this);
    connect(timer_logic, SIGNAL(timeout()), this, SLOT(logic_update()));
    //timer_logic->start(100); 

    // timer_communication
    timer_communication = new QTimer(this);
    connect(timer_communication, SIGNAL(timeout()), this, SLOT(communicate_update()));
    timer_communication->start(100);

    // ui
    connect(ui->pushButton_change_0, SIGNAL(clicked()), this, SLOT(change_0()));
    connect(ui->pushButton_change_1, SIGNAL(clicked()), this, SLOT(change_1()));
    connect(ui->pushButton_reset, SIGNAL(clicked()), this, SLOT(game_reset()));
    connect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(game_status_change()));
    ui->pic_car_0->setPixmap(QPixmap(":/image/location_red").scaled(SIZEPIC, SIZEPIC));
    ui->pic_car_1->setPixmap(QPixmap(":/image/location_green").scaled(SIZEPIC, SIZEPIC));
    ui->pic_target->setPixmap(QPixmap(":/image/star_s").scaled(SIZEPIC, SIZEPIC));
    ui->pic_air->setPixmap(QPixmap(":/image/plane").scaled(SIZEPIC, SIZEPIC));
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
	ui->pushButton_start->setText(tr("START"));

	// logic
	game = Game(MAP_FILENAME);
	init_gameData();

	// game control
	status = GameWaiting;

	// timer logic
	timer_logic->stop();
}

void MainWindow::ui_update()
{

	//小车+飞机
    int x0 = 153 - SIZEPIC / 2;
    int y0 = 47 - SIZEPIC / 2;
    cv::Point t;

    t = mapper.MapToImage(gameData.carData[0].pos.x, gameData.carData[0].pos.y);
    ui->pic_car_0->move(x0 + t.x, y0 + t.y);

    t = mapper.MapToImage(gameData.carData[1].pos.x, gameData.carData[1].pos.y);
    ui->pic_car_1->move(x0 + t.x, y0 + t.y);

    if (!TARGET_NONEXISTENT) {
        t = mapper.MapToImage(gameData.planePoint.x, gameData.planePoint.y);
        ui->pic_air->move(x0 + t.x, y0 + t.y);
        if (gameData.targetHealth > 0) {
            ui->pic_target->setVisible(true);
            t = mapper.MapToImage(gameData.targetPoint.x, gameData.targetPoint.y);
            ui->pic_target->move(x0 + t.x, y0 + t.y);
        }
        else {
            ui->pic_target->setVisible(false);
        }
    }

    ui->pic_prop->setPixmap(QPixmap(QString(":/image/prop_%1").arg(int(gameData.propType))).scaled(SIZEPIC, SIZEPIC));
    t = mapper.MapToImage(gameData.propPoint.x, gameData.propPoint.y);
    ui->pic_prop->move(x0 + t.x, y0 + t.y);
	
}

void MainWindow::game_status_change()
{
    if (ui->pushButton_start->text() == "START") // TODO  change it to constant
    {
        // game control 
        status = GameStart;

        // ui
        ui->pic_car_0->setVisible(true);
        ui->pic_car_1->setVisible(true);
        
        if (!TARGET_NONEXISTENT) {
            ui->pic_target->setVisible(true);
            ui->pic_air->setVisible(true);
        }

        ui->pic_prop->setVisible(true);
        ui->pushButton_start->setText(tr("PAUSE"));

        // timer_logic
        timer_logic->start(100);
    }
    else if (ui->pushButton_start->text() == "PAUSE")
    {
        // game control
        status = GamePause;

        // ui
        ui->pushButton_start->setText(tr("START"));

        // timer_logic
        timer_logic->stop();
    }
}

void MainWindow::logic_update()
{
    Point p;
    icons_update();

    if (game.GetGameStatus() == Running) {

        if (!gameData.carData[0].air_command && !gameData.carData[1].air_command) {
            dobby.SetTarget(gameData.targetPoint);
        }

        for (int i = 0; i < 2; ++i) {
            if (gameData.carData[i].out_of_range()) gameData.carData[i].health -= OUT_OF_RANGE;
        }

        game.Refresh(gameData);
        gameData = game.getGameData();
    }
}

void MainWindow::communicate_update()
{
    char mes[4] = { 0 };
    mes[0] = (gameData.carData[0].long_attack_map ? 1 : 0) << 7
        | (gameData.carData[0].short_attack_map ? 1 : 0) << 6
        | (gameData.carData[0].air_command ? 1 : 0) << 5
        | (gameData.carData[0].heal_plane ? 1 : 0) << 4
        | (gameData.carData[0].attack_plane ? 1 : 0) << 3
        | (gameData.carData[0].out_of_range() ? 1 : 0) << 2
        | status;

    mes[1] = (gameData.carData[1].long_attack_map ? 1 : 0) << 7
        | (gameData.carData[1].short_attack_map ? 1 : 0) << 6
        | (gameData.carData[1].air_command ? 1 : 0) << 5
        | (gameData.carData[1].heal_plane ? 1 : 0) << 4
        | (gameData.carData[1].attack_plane ? 1 : 0) << 3
        | (gameData.carData[1].out_of_range() ? 1 : 0) << 2
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

    //MyString ms(res, 18);
    //ui->Status->setText(ms.hex_str());
    port->send(res, 18);


    // receive
    vector<MyString> ll = port->receive();
    if (ll.size() > 0)
    {
        MyString& ms = ll[ll.size() - 1];
        if (ms.length() == 5) {
            if (gameData.carData[ms[0]].air_command) {
                dobby.SetTarget(Point(ms[1], ms[2]));
            }
        }
        //ui->Status->setText(ms.hex_str());
    }
}

void MainWindow::init_gameData()
{
    gameData.round = 0;
    gameData.carData[0].health = 200;
    gameData.carData[0].pos = Point(0, 0);
    gameData.carData[1].health = 200;
    gameData.carData[1].pos = Point(0, 0);
    gameData.planePoint = dobby.GetPos();
}

void MainWindow::icons_update()
{
    int x_s = 10, hp_r, hp_g;
    float l_a = 912, l_r, l_g;

    //黑白区
    ui->color_of_R->setPixmap(QPixmap(gameData.carData[0].color ? ":/image/map_white" : ":/image/map_black"));
    ui->color_of_G->setPixmap(QPixmap(gameData.carData[1].color ? ":/image/map_white" : ":/image/map_black"));
    //ui->pic->setText(QString::number(gameData.targetHealth));
    //飞机控制
    ui->Air_con_R->setPixmap(QPixmap((gameData.carData[0].air_command) ? ":/image/plane" : ":/image/prop_0"));
    ui->Air_con_G->setPixmap(QPixmap((gameData.carData[1].air_command) ? ":/image/plane" : ":/image/prop_0"));

    //飞机攻击、加血
    if (gameData.carData[0].attack_plane)
        ui->Air_A_R->setPixmap(QPixmap(":/image/lightning"));
    else ui->Air_A_R->setPixmap(QPixmap(gameData.carData[0].heal_plane ? ":/image/heart" : ":/image/prop_0"));
    
    if (gameData.carData[1].attack_plane)
        ui->Air_A_G->setPixmap(QPixmap(":/image/lightning"));
    else ui->Air_A_G->setPixmap(QPixmap(gameData.carData[1].heal_plane ? ":/image/heart" : ":/image/prop_0"));

    //出界
    ui->O_range_R->setPixmap(QPixmap(gameData.carData[0].out_of_range() ? ":/image/location_not_found" : ":/image/location_red"));
    ui->O_range_G->setPixmap(QPixmap(gameData.carData[1].out_of_range() ? ":/image/location_not_found" : ":/image/location_green"));

    //地图攻击
    if (gameData.carData[0].short_attack_map || gameData.carData[0].out_of_range())
        ui->Att_R->setPixmap(QPixmap(":/image/warning_1"));
    else if (gameData.carData[0].long_attack_map)
        ui->Att_R->setPixmap(QPixmap(":/image/warning_0"));
    else ui->Att_R->setPixmap(QPixmap(":/image/verified"));
    
    if (gameData.carData[1].short_attack_map || gameData.carData[1].out_of_range())
        ui->Att_G->setPixmap(QPixmap(":/image/warning_1"));
    else if (gameData.carData[1].long_attack_map)
        ui->Att_G->setPixmap(QPixmap(":/image/warning_0"));
    else ui->Att_G->setPixmap(QPixmap(":/image/verified"));

    //血条
    hp_r = gameData.carData[0].health;
    hp_r = hp_r > 0 ? hp_r : 0;
    hp_g = gameData.carData[1].health;
    hp_g = hp_g > 0 ? hp_g : 0;
    l_r = l_a * hp_r / (hp_r + hp_g);
    l_g = l_a * hp_g / (hp_r + hp_g);
    ui->HP_R->resize(l_r, 25);
    ui->HP_R->move(x_s, 10);
    ui->HP_R->setText(QString::number(hp_r));
    ui->HP_G->resize(l_g, 25);
    ui->HP_G->move(x_s + l_r, 10);
    ui->HP_G->setText(QString::number(hp_g));

    //时间
    ui->time->setText(QString("TIME:") + QString::number(gameData.round / FREQ) + QString("s"));

    //飞机当前状态
    ui->air_status->setPixmap(QPixmap((gameData.planeStatus == PlaneHeal) ? ":/image/heart" : ":/image/lightning"));

}

static void setPoint(cv::Point& p, Point& pp/*, bool& out*/) {
    
    //out = false;

    pp.x = p.x;
    if (p.x < 0) {
        //out = true;
        pp.x = 0;
    }
    if (p.x > 255) {
        //out = true;
        pp.x = 255;
    }

    pp.y = p.y;
    if (p.y < 0) {
        //out = true;
        pp.y = 0;
    }
    if (p.y > 255) {
        //out = true;
        pp.y = 255;
    }

}

void MainWindow::capture_update()
{
    gameData.planePoint = dobby.GetPos();

    if (camera.isValid()) {
        capture_frame = camera.getFrame();
        
        if (!camera.isValid()) {
            ui->pic->setText("Camera connection halted.");
            ui->Status->setText("Camera connection halted.");
            status = GameOver;
            timer_logic->stop();
            return;
        }

        if (!capture_frame.empty()) {

            // update status
            /*static const char* status_label_format = "%dfps"; // frame rate
            static char buffer[512];
            sprintf(buffer, status_label_format, camera.getFrameRate());
            if (camera.getFrameRate()<20) ui->Status->setText(buffer);*/

            // locator
            locator.Refresh(capture_frame);
            locator_points = locator.getTargetPoints();

            cv::circle(capture_frame, locator_points[0], 5, cv::Scalar(0, 0, 255), -1);
            cv::circle(capture_frame, locator_points[1], 5, cv::Scalar(0, 255, 0), -1);

            // set the point
            cv::Point t;
            if (locator_points[0] != cv::Point(0, 0)) {
                t = mapper.ImageToMap(locator_points[0]);
                setPoint(t, gameData.carData[0].pos/*, gameData.carData[0].out_of_range*/);
                //if (t.x <= 0 || t.x >= 255 || t.y <= 0 || t.y >= 255) gameData.carData[0].out_of_range = true;
            }
            if (locator_points[1] != cv::Point(0, 0)) {
                t = mapper.ImageToMap(locator_points[1]);
                setPoint(t, gameData.carData[1].pos/*, gameData.carData[1].out_of_range*/);
                //if (t.x <= 0 || t.x >= 255 || t.y <= 0 || t.y >= 255) gameData.carData[1].out_of_range = true;
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

    communicate_update();

    QMainWindow::closeEvent(event);

}