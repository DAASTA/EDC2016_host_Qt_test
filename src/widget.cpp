// Created by sugar10w, 2016.8

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent)
    ,ui(new Ui::Widget)
    ,camera(1, "./data/hd_usb_camera.xml")
    //,camera(0)
    ,capture_timer(NULL)
{
    ui->setupUi(this);

    if (!camera.isValid()) {
        ui->labelCapture->setText("Invalid camera.");
        ui->labelStatus->setText("Failed to load the camera.");
    } else {
        // set up capture_timer
        capture_timer = new QTimer(this);
        connect(capture_timer, SIGNAL(timeout()), this, SLOT(capture_update()));
        capture_timer->start(20);

        locator.addTargetColor(Locator::Blue);
        locator.addTargetColor(Locator::Green);
        locator.addTargetColor(Locator::Red);
    }

}

void Widget::capture_update()
{
    if (camera.isValid()) {
        capture_frame = camera.getFrame();
        if (!capture_frame.empty()) {
            
            // update status
            static const char* status_label_format = "%dfps"; // frame rate
            static char buffer[512];
            sprintf(buffer, status_label_format, camera.getFrameRate());
            ui->labelStatus->setText(buffer);

            // locator
            locator.Refresh(capture_frame);
            locator_points = locator.getTargetPoints();
            static const char* status_label_format_2 = "Blue  %d,%d\nGreen %d,%d\nRed   %d,%d"; // frame rate
            static char buffer_2[512];
            sprintf(buffer_2, status_label_format_2,
                locator_points[0].x, locator_points[0].y,
                locator_points[1].x, locator_points[1].y,
                locator_points[2].x, locator_points[2].y);
            ui->labelSubCapture->setText(buffer_2);
            cv::circle(capture_frame, locator_points[0], 5, cv::Scalar(255, 0, 0), -1);
            cv::circle(capture_frame, locator_points[1], 5, cv::Scalar(0, 255, 0), -1);
            cv::circle(capture_frame, locator_points[2], 5, cv::Scalar(0, 0, 255), -1);

            // update image
            capture_image = QImage(
                (const uchar*)capture_frame.data, capture_frame.cols, capture_frame.rows, QImage::Format_RGB888)
                .rgbSwapped();
            ui->labelCapture->setPixmap(QPixmap::fromImage(capture_image));




            
        }
    }
}

Widget::~Widget()
{
    delete ui;
    
    if (NULL!=capture_timer) delete capture_timer;
}
