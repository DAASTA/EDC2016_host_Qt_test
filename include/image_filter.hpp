// Created by wxk14, 2016.8
//   一些常用的处理二维图片的静态方法。

#pragma once

#include <opencv2/opencv.hpp>

class ImageFilter {

public:

static void DilateImage(cv::Mat& src_img, cv::Mat& dst_img, int dilate_size, int kernel_type = cv::MORPH_RECT) {
    if (dilate_size>0) {
        cv::Mat dilate_kernel = cv::getStructuringElement(
            kernel_type,
            cv::Size(2*dilate_size+1, 2*dilate_size+1),
            cv::Point(dilate_size, dilate_size));
        cv::dilate(src_img, dst_img, dilate_kernel);
    }
}
static void ErodeImage(cv::Mat& src_img, cv::Mat& dst_img, int erode_size, int kernel_type = cv::MORPH_RECT) {
    if (erode_size>0) {
        cv::Mat erode_kernel = cv::getStructuringElement(
            kernel_type, 
            cv::Size(2*erode_size+1, 2*erode_size+1),
            cv::Point(erode_size, erode_size));
        cv::erode(src_img, dst_img, erode_kernel);
    }
}
static void OpenImage(cv::Mat& img, int refill_size=0, int erode_size=0, int dilate_size=0, int kernel_type = cv::MORPH_ELLIPSE) {
    // MORPH_RECT, MORPH_CROSS, MORPH_ELLIPSE
    DilateImage(img, img, refill_size, kernel_type);
    ErodeImage(img, img, erode_size, kernel_type);
    DilateImage(img, img, dilate_size, kernel_type);
}

// clear borders 清理边缘
static void ClearBorder(cv::Mat& img)
{
    if (img.type() != CV_8UC1) return;

    for (int i = 0; i < img.cols; ++i) {
        img.at<uchar>(0, i) = 0;
        img.at<uchar>(img.rows - 1, i) = 0;
    }
    for (int i = 0; i < img.rows; ++i) {
        img.at<uchar>(i, 0) = 0;
        img.at<uchar>(i, img.cols - 1) = 0;
    }
}
// get contours without `cv::findContours()` ; OpenCV2410版本中的`cv::findContours()`可能和VS2015不兼容；以下是一种简洁的实现。
static std::vector<std::vector<cv::Point> > GetContours(cv::Mat& img)
{
    int dx[8] = { 1, 1, 0, -1, -1, -1,  0,  1 };
    int dy[8] = { 0, 1, 1,  1,  0, -1, -1, -1 };
    auto outOfRange = [&](cv::Point p) {
        return p.x < 0 || p.x >= img.cols
            || p.y < 0 || p.y >= img.rows;
    };

    std::vector<std::vector<cv::Point> > contours;

    ClearBorder(img);
    cv::Mat lines = img.clone();
    OpenImage(lines, 0, 1, 0);
    lines = (~lines) & img;
    //cv::imshow("cont", lines);
    //cv::waitKey();

    bool next = true;
    cv::Point curr_p, next_p;

    for (int y = 0; y < lines.rows; ++y)
        for (int x = 0; x < lines.cols; ++x) {
            if (!lines.at<uchar>(y, x)) continue;
            
            // Search from this point
            std::vector<cv::Point> point_list;
            next = true;
            curr_p.x = x; curr_p.y = y;
            while (next) {
                next = false;
                lines.at<uchar>(curr_p) = 0;
                point_list.push_back(curr_p);

                for (int i = 0; i < 8; ++i) {
                    next_p.x = curr_p.x + dx[i]; next_p.y = curr_p.y + dy[i];
                    if (!outOfRange(next_p) && lines.at<uchar>(next_p)) {
                        next = true;
                        curr_p = next_p;
                        break;
                    }
                }
            }
            contours.push_back(point_list);
        }

    return contours;
}

// input: img cv::Mat(CV_8UC1);   return: cv::Mat(CV_8UC1), the contour of `img` would be 255
static cv::Mat GetContourLines(cv::Mat& img)     // todo: to be confirmed 
{
    std::vector<std::vector<cv::Point> > contours;
    cv::Mat result = cv::Mat::zeros(img.size(), CV_8UC1);

    cv::findContours(img, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    cv::drawContours(result, contours, -1, cv::Scalar(255), 1);

    return result;
}

// input: img  cv::Mat(CV_8UC3), mask cv::Mat(CV_8UC1)
static cv::Mat applyMask(cv::Mat img, cv::Mat mask, uchar cc = 0) {
    cv::Mat out = img.clone();
    
    if (mask.type() != CV_8UC1 || mask.cols != img.cols || mask.rows != img.rows
        || img.type() != CV_8UC3)
    {
        std::cout << "[Warning] (applyMask) the mask and the image do not correspond." << std::endl;
        return out;
    }

    for (int i = 0; i < out.rows; ++i)
        for (int j = 0; j < out.cols; ++j) 
            if (mask.at<uchar>(i, j) == 0) {
                cv::Vec3b & point = out.at<cv::Vec3b>(i, j);
                point(0) = point(1) = point(2) = cc;
            }
    return out;
}

};