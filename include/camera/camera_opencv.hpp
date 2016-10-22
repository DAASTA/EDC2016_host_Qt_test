// Created by sugar10w, 2016.8
//  Camera driven by opecv : videocapture & undistort
//
//  - Camera(id, file_name=""): ����id�����������file_name�ļ�����֮
//      - getFrame(): ��ȡ��ǰͼ��
//      - getFrameRate(): ��ȡ��ǰ֡��

#pragma once

#include <string>
#include <iostream>
#include <exception>

#include <opencv2/opencv.hpp>

#include "camera/frame_rate_counter.hpp"

class Camera {
public:
    Camera(int camera_id, std::string config_file_name = "")
    {
        capture.open(camera_id);
        valid = capture.isOpened();
        if (!valid)
            std::cout << "[WARNING] failed to open camera " << camera_id << std::endl;
        else {
            undistort_enabled = loadConfigFile(config_file_name);
            capture.set(CV_CAP_PROP_FRAME_WIDTH, width);
            capture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
        }
    }

    cv::Mat getFrame() {

        try {
            capture >> frame;
        }
        catch (...) {
            valid = false;
            return cv::Mat::zeros(480, 640, CV_8UC3);
        }
        frame_rate = fps.getFrameRate();

        if (!undistort_enabled)
            return frame;
        else {
            try {
                cv::remap(frame, undistort_frame, undistort_map1, undistort_map2, CV_INTER_LINEAR);
                return undistort_frame;
            }
            catch (...) {
                valid = false;
                return cv::Mat::zeros(480, 640, CV_8UC3);
            }
        }
    }
    
    inline bool isValid() { return valid; }
    inline int getFrameRate() { return frame_rate; }

private:

    bool loadConfigFile(std::string config_file_name) {
        if (config_file_name.empty()) return false;

        cv::FileStorage fs(config_file_name, cv::FileStorage::READ);
        if (!fs.isOpened()) {
            std::cout << "[Warning] Failed to load camera config file " << config_file_name << std::endl;
            return false;
        }

        try {
            fs["r"] >> r;
            fs["camera_matrix"] >> camera_matrix;
            fs["distortion_coeffs"] >> distortion_coeffs;
            fs["width"] >> width;
            fs["height"] >> height;
        }
        catch (const std::exception& e) {
            std::cout << "[Warning] Error occurred when reading camera config file " << config_file_name << ", " << e.what() << std::endl;
            fs.release();
            return false;
        }

        cv::initUndistortRectifyMap(
            camera_matrix, distortion_coeffs, r,
            new_camera_matrix, cv::Size(width, height), CV_32FC1,
            undistort_map1, undistort_map2);
        return true;
    }
    bool saveConfigFile(std::string config_file_name) {
        // [Warning] for DEV only
        if (config_file_name.empty()) return false;

        cv::FileStorage fs(config_file_name, cv::FileStorage::WRITE);

        r = cv::Mat_<double>(3, 3);
        camera_matrix = cv::Mat_<double>(3, 3);
        distortion_coeffs = cv::Mat_<double>(1, 5);

        r <<
            1, 0, 0,
            0, 1, 0,
            0, 0, 1;
        camera_matrix <<
            4.1972966873351527e+002, 0, 3.1238591883450624e+002,
            0, 4.2398355813671100e+002, 2.4681104565919080e+002,
            0, 0, 1;
        distortion_coeffs <<
            -4.1806297419806321e-001, 3.3357086669686503e-001,
            1.9940433129924952e-003, 4.1782241358449829e-004,
            -1.9843542021226693e-001;

        fs << "width" << 640;
        fs << "height" << 480;
        fs << "r" << r;
        fs << "camera_matrix" << camera_matrix;
        fs << "distortion_coeffs" << distortion_coeffs;

        fs.release();

        return true;
    }

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