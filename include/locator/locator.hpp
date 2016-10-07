// Created by wxk14, 2016.9
//  - Locator 色块定位

#pragma once

#include <opencv2\opencv.hpp>

#include "locator/contour.hpp"
#include "locator/image_filter.hpp"

class Locator {
public:

    // 一些需要实测才能确定的 Hoe 值
    enum HsvColor {
        Green = 60,
        Blue = 115,
        Red = 175
    };

    Locator()
        : threshold_(10)
    { }

    inline void setTargetColors(std::vector<double> colors) { target_color_ = colors; }
    inline void addTargetColor(double color) { target_color_.push_back(color); }
    inline std::vector<cv::Point> getTargetPoints() const { return target_point_; }

    void Refresh(cv::Mat& mat) {
        cv::Mat hsv_img;

        std::vector<cv::Mat> mask_list(target_color_.size());
        for (int k = 0; k < target_color_.size(); ++k)
            mask_list[k] = cv::Mat::zeros(mat.size(), CV_8UC1);

        // 从 BGR 转到 HSV 
        cvtColor(mat, hsv_img, CV_BGR2HSV);
        for (int i = 0; i<mat.rows; ++i)
            for (int j = 0; j < mat.cols; ++j) {
                cv::Vec3b& point = hsv_img.at<cv::Vec3b>(i, j);
                uchar& h = point[0];
                uchar& s = point[1];
                uchar& v = point[2];

                // 饱和度 > 100; 且色彩区域正确
                if (s > 100) {
                    for (int k = 0; k < target_color_.size(); ++k)
                        mask_list[k].at<uchar>(i, j) = isSimilarHoe(target_color_[k], h) ? 255 : 0;
                }

            }

        if (target_point_.size() < target_color_.size()) target_point_.resize(target_color_.size());

        // 轮廓提取，确定目标位置
        for (int k = 0; k < target_color_.size(); ++k) {
            ImageFilter::OpenImage(mask_list[k], 0, 5, 5);

            ContourList cl;
            cl.SetImage(mask_list[k]);
            target_point_[k] = cl.GetBestContour(target_point_[k]).GetCenterPoint();
        }
    }

private:

    inline bool isSimilarHoe(double std, double h)
    { 
        return abs(std - h) < THRESHOLD
            || abs(std - h + 180) < THRESHOLD
            || abs(std - h - 180) < THRESHOLD;
    }

    std::vector<cv::Point> target_point_;
    std::vector<double> target_color_;
    double threshold_;

    const static int RED = 175;
    const static int BLUE = 115;
    const static int GREEN = 60;
    const static int THRESHOLD = 10;
};