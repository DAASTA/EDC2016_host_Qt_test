// Created by sugar10w, 2016.8
//      camera driven by opecv : videocapture & undistort
//      read the undistort file

#pragma once

#include <string>
#include <iostream>

#include <opencv2/opencv.hpp>

#include "frame_rate_counter.hpp"

class Camera {
public:
    Camera(int camera_id, std::string config_file_name = "");

    cv::Mat getFrame();
    
    inline bool isValid() { return valid; }
    inline int getFrameRate() { return frame_rate; }

private:

    bool loadConfigFile(std::string config_file_name);
    bool saveConfigFile(std::string config_file_name);

    bool        valid;
    cv::VideoCapture    capture;
    cv::Mat     frame;

    int         width;
    int         height;
    
    bool        undistort_enabled;
    cv::Mat             undistort_frame;    
    cv::Mat_<double>    r;
    cv::Mat_<double>    camera_matrix;
    cv::Mat_<double>    distortion_coeffs;
    cv::Mat             new_camera_matrix;
    cv::Mat             undistort_map1;
    cv::Mat             undistort_map2;

    FrameRateCounter    fps;
    int                 frame_rate;

};