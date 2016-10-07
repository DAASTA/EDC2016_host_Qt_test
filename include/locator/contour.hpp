// Created by sugar10w, 2016.9
// 提取二值图像的边缘。
//  - Contour 一个边缘
//  - ContourList 整个图像

#pragma once

#include <vector>
#include <limits>

#include <opencv2/opencv.hpp>

#include "locator/image_filter.hpp"

class Contour {

public:
    Contour()
        : center_point_(0,0)
        , size_(0)
    { }

    void SetPointList(std::vector<cv::Point> list) {
        point_list_ = list;
        size_ = cv::contourArea(point_list_);
        CalcCenterPoint();
    }

    double GetSize() const { return size_; }
    cv::Point GetCenterPoint() const { return center_point_; }

private:

    void CalcCenterPoint() {
        // 直接将轮廓线的重心当作中心点坐标
        float x = 0, y = 0;
        for (int i = 0; i < point_list_.size(); ++i) {
            x += point_list_[i].x;
            y += point_list_[i].y;
        }
        center_point_.x = (int)(x / point_list_.size());
        center_point_.y = (int)(y / point_list_.size());
    }

    std::vector<cv::Point> point_list_;
    double size_;
    cv::Point center_point_;
};

class ContourList {

public:
    ContourList() { }

    bool SetImage(cv::Mat & img) {
        valid_ = false;
        if (img.type() != CV_8UC1) return false;

        cv::Mat mask = img.clone();
        std::vector<std::vector<cv::Point> > contours = ImageFilter::GetContours(mask);
        contour_list_.resize(contours.size());
        for (int i = 0; i < contours.size(); ++i) contour_list_[i].SetPointList(contours[i]);

        valid_ = true;
        return true;
    }

    Contour GetMaxContour() {
        if (!valid_) return Contour();

        double bestSize = -1;
        int bestIdx = -1;
        for (int i=0; i<contour_list_.size(); ++i)
            if (contour_list_[i].GetSize() > bestSize) {
                bestSize = contour_list_[i].GetSize();
                bestIdx = i;
            }

        if (bestIdx == -1) return Contour();
        return contour_list_[bestIdx];
    }

    Contour GetBestContour(cv::Point point) {
        if (!valid_) return Contour();

        double bestDistance = std::numeric_limits<double>::max();
        int bestIdx = -1;
        for (int i=0; i<contour_list_.size(); ++i){
            // 只有面积大于500才会被认可 TODO
            if (contour_list_[i].GetSize() < 500) continue;
            cv::Point center = contour_list_[i].GetCenterPoint();
            double square_distance = (center.x - point.x)*(center.x - point.x) + (center.y - point.y)*(center.y - point.y);
            if (square_distance < bestDistance) {
                bestDistance = square_distance;
                bestIdx = i;
            }
        }
        
        if (bestIdx == -1) return Contour();
        return contour_list_[bestIdx];
    }

    bool isValid() const { return valid_; }

private:
    bool valid_;
    std::vector<Contour> contour_list_;


};