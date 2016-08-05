// Created by sugar10w, 2016.8

#include "widget.h"
#include "ui_widget.h"

#include "camera_opencv.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    //camera(1, "hd_usb_camera.xml")
    camera(0)
{
    ui->setupUi(this);

    // set up capture
    if (!camera.isValid()) {
        ui->labelCapture->setText("Invalid camera.");
        ui->labelStatus->setText("Failed to load the camera.");
        capture_ready = false;
    }
    else {
        capture_ready = true;
    }

    capture_timer = NULL;
    if (capture_ready) {
        // set up capture timer
        capture_timer = new QTimer(this);
        connect(capture_timer, SIGNAL(timeout()), this, SLOT(capture_update()));
        capture_timer->start(20);
    }

}

void Widget::capture_update()
{
    if (capture_ready) {
        capture_frame = camera.getFrame();
        if (!capture_frame.empty()) {
            // update image
            capture_image = QImage(
                (const uchar*)capture_frame.data, capture_frame.cols, capture_frame.rows, QImage::Format_RGB888)
                .rgbSwapped();
            ui->labelCapture->setPixmap(QPixmap::fromImage(capture_image));
            // update status
            static const char* status_label_format = "%dfps"; // frame rate
            static char buffer[512];
            sprintf(buffer, status_label_format, camera.getFrameRate());
            ui->labelStatus->setText(buffer);
        }
    }
}

Widget::~Widget()
{
    delete ui;
    
    if (NULL!=capture_timer) delete capture_timer;
}
