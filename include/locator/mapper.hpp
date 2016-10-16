// Created by wxk14, 2016.10
//   将图片坐标(640*480)和地图坐标(256*256)

#pragma once

#include<fstream>
#include<string>
#include<vector>

#include<opencv2\opencv.hpp>


class Mapper {

public:
    explicit Mapper(std::string filename) {
        std::ifstream in(filename);        

        if (!in) {
            std::cout << "[Error] Invalid file " << filename << std::endl;
            return;
        }
        
        for (int i = 0; i < 4; ++i) {
            in >> p[i].x >> p[i].y;
        }
        
        in.close();

        cv::Mat list_map(4, 2, CV_32F);
        list_map.at<float>(0, 0) = INDEX_POS0; list_map.at<float>(0, 1) = INDEX_POS0;
        list_map.at<float>(1, 0) = INDEX_POS1; list_map.at<float>(1, 1) = INDEX_POS0;
        list_map.at<float>(2, 0) = INDEX_POS0; list_map.at<float>(2, 1) = INDEX_POS1;
        list_map.at<float>(3, 0) = INDEX_POS1; list_map.at<float>(3, 1) = INDEX_POS1;
        
        cv::Mat list_image(4, 2, CV_32F);
        for (int i = 0; i < 4; ++i) {
            list_image.at<float>(i, 0) = p[i].x;
            list_image.at<float>(i, 1) = p[i].y;
        }

        std::vector<uchar> m;
        m_image2map = cv::findHomography(list_image, list_map, CV_RANSAC, 1, m);
        m_map2image = cv::findHomography(list_map, list_image, CV_RANSAC, 1, m);

        /*         std::cout << m_image2map.type() << std::endl;

       for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                std::cout << m_image2map.at<double>(i, j) << "\t\t";
            }
            std::cout << std::endl;
        }

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                std::cout << m_map2image.at<double>(i, j) << "\t\t";
            }
            std::cout << std::endl;
        }
        */
    }

    cv::Point MapToImage(cv::Point p) {
        
        cv::Mat_<double> v_image;
        cv::Mat_<double> v_map(3, 1);
        cv::Point p_image;

        v_map << p.x, p.y, 1;

        v_image = m_map2image * v_map;

        p_image.x = v_image(0, 0) / v_image(2, 0);
        p_image.y = v_image(1, 0) / v_image(2, 0);

        return p_image;
    }

    cv::Point MapToImage(int x, int y) {
        return MapToImage(cv::Point(x, y));
    }

    cv::Point ImageToMap(cv::Point p) {

        cv::Mat_<double> v_image(3, 1);
        cv::Mat_<double> v_map;
        cv::Point p_map;

        v_image << p.x, p.y, 1;

        v_map = m_image2map * v_image;

        p_map.x = v_map(0, 0) / v_map(2, 0);
        p_map.y = v_map(1, 0) / v_map(2, 0);

        return p_map;
    }

    cv::Point ImageToMap(int x, int y) {
        return ImageToMap(cv::Point(x, y));
    }

private:

    const static int INDEX_POS0 = -14;
    const static int INDEX_POS1 = 256 - INDEX_POS0;

    cv::Point2f p[4];

    cv::Mat_<double> m_image2map;
    cv::Mat_<double> m_map2image;
};