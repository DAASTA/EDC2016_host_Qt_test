#ifndef WIDGET_H
#define WIDGET_H

#include <ctime>

#include <opencv2/opencv.hpp>

#include <QWidget>
#include <QTimer>
#include <QImage>

#include "frame_rate_counter.hpp"
#include "camera_opencv.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void capture_update();

private:
    Ui::Widget  *ui;

    // capture
    bool        capture_ready;
	QImage	    capture_image;
    QTimer      *capture_timer;

    Camera      camera;
    cv::Mat          capture_frame;
};

#endif // WIDGET_H
