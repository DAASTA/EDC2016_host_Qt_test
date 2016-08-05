// Created by sugar10w, 2016.8
//      camera driven by opecv : videocapture & undistort
//      read the undistort file

#include "camera_opencv.h"

Camera::Camera(int camera_id, std::string config_file_name) :
    width(640), height(480)
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

cv::Mat Camera::getFrame() {

    capture >> frame;
    frame_rate = fps.getFrameRate();

    if (!undistort_enabled)
        return frame;
    else {
        cv::remap(frame, undistort_frame, undistort_map1, undistort_map2, CV_INTER_LINEAR);
        return undistort_frame;
    }
}

bool Camera::loadConfigFile(std::string config_file_name) {
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
    catch (...) {
        std::cout << "[Warning] Error occurred when reading camera config file " << config_file_name << std::endl;
        fs.release();
        return false;
    }

    cv::initUndistortRectifyMap(
        camera_matrix, distortion_coeffs, r,
        new_camera_matrix, cv::Size(width, height), CV_32FC1,
        undistort_map1, undistort_map2);
    return true;
}
bool Camera::saveConfigFile(std::string config_file_name) {
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
