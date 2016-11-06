
#include "camera\camera_opencv.hpp"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace cv;

const string filename = "./data/index_points.txt";

vector<string> name_list = {"Red   ", "Green ", "Blue  ", "Black "};
vector<Point> point_list;
int idx = 0;

 // The following program might fail.
void onMouse(int event, int x, int y, int flags, void* param)
{
    Mat *im = reinterpret_cast<Mat*>(param);

    if (NULL == im || im->empty()) {
        cout << "invalid picture" << endl;
        return;
    }

    switch (event)
    {
    case CV_EVENT_LBUTTONDOWN:     //鼠标左键按下响应：返回坐标和灰度  
        if (idx < name_list.size()) {
            cout << name_list[idx] << "\t" << x << "\t" << y << endl;
            ++idx;
            point_list.push_back(Point(x, y));
        }
        else {
            cout << "Press `space` to finish." << endl;
        }
        break;
    }

}

// back up the file 
void write_to_file() {

    if (point_list.size() == name_list.size()) {
        ofstream ofs(filename);
        for (int i = 0; i < point_list.size(); ++i) {
            ofs << point_list[i].x << "\t" << point_list[i].y << endl;
        }
        ofs.close();
    }

}

int main()
{

    Camera camera(1, "./data/hd_usb_camera.xml");
    
    Mat frame;
    Mat photo;
    bool isRunning = true;

    if (!camera.isValid()) {
        cout << "[Error] Invalid camera! " << endl;
        system("pause");
    }

    while (isRunning) {

        frame = camera.getFrame();
        putText(frame, "Press `space` to continue.", cv::Point(0, 20), 0, 0.4, cv::Scalar(0, 0, 0), 1);

        imshow("camera", frame);

        char c = waitKey(5);

        switch (c)
        {
        case ' ':
            isRunning = false;
            break;
        default:
            break;
        }

    }

    photo = camera.getFrame();

    putText(photo, "Please click Red, Green, Blue, Black. Press `space` to finish.", cv::Point(0, 20), 0, 0.4, cv::Scalar(0,0,0), 1);
    imshow("camera", photo);
    setMouseCallback("camera", onMouse, reinterpret_cast<void*> (&photo));

    waitKey(0);

    write_to_file();

    return 0;
}