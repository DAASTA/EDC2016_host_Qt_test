
#pragma once

#include<opencv2/opencv.hpp>

#include<string>
#include<iostream>
#include<fstream>
#include<vector>

#include"logic\GameData.h"

class LogReader {

public:

    LogReader()
        : map_(256, 256, CV_8UC3)
    { }

    void LoadMap(std::string filename) {
        std::ifstream in(filename);
        in >> size_;
        map_ = cv::Mat::zeros(size_, size_, CV_8UC3);
        for (int i = 0; i < size_; ++i)
            for (int j = 0; j < size_; ++j) {
                int k; in >> k;
                if (0 == k) map_.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);
                else map_.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 255);
            }
        in.close();
    }
    void LoadLog(std::string filename) {
        std::ifstream in(filename);
        std::string s;
        char buffer[512];

        gamedata_list_.clear();

        while (!in.eof()) {
            GameData gamedata;
            std::getline(in, s);
            if (s.empty()) break;
            strcpy(buffer, s.c_str());
            gamedata.loadString(buffer);

            std::getline(in, s);
            if (s.empty()) break;
            strcpy(buffer, s.c_str());
            gamedata.carData[0].loadString(buffer);

            std::getline(in, s);
            if (s.empty()) break;
            strcpy(buffer, s.c_str());
            gamedata.carData[1].loadString(buffer);

            std::getline(in, s);

            gamedata_list_.push_back(gamedata);

        }

        in.close();
    }

    cv::Mat TranslateGameData(GameData& gamedata) {
        cv::Mat display;

        Point t;
        int radius = 9;
        char buffer[512];

        display = map_.clone();
        t = gamedata.targetPoint;
        if (gamedata.targetHealth > 0) {
            cv::circle(display, cv::Point(t.y, t.x), radius, cv::Scalar(0, 255, 0), -1);
            cv::circle(display, cv::Point(t.y, t.x), RADIUS_CRITICAL, cv::Scalar(0, 255, 0), 1);
            cv::rectangle(display, cv::Rect(t.y - 20, t.x - 15, gamedata.targetHealth, 3), cv::Scalar(0, 255, 0), -1);
        }

        t = gamedata.carData[Red].pos;
        cv::circle(display, cv::Point(t.y, t.x), radius, cv::Scalar(0, 0, 255), -1);
        if (gamedata.carData[Red].attack_plane || gamedata.carData[Red].heal_plane)
            cv::circle(display, cv::Point(t.y, t.x), 4, cv::Scalar(255, 255, 0), 1);
        if (gamedata.carData[Red].long_attack_map || gamedata.carData[Red].short_attack_map)
            cv::circle(display, cv::Point(t.y, t.x), 7, cv::Scalar(0, 255, 0), 1);


        t = gamedata.carData[Blue].pos;
        cv::circle(display, cv::Point(t.y, t.x), radius, cv::Scalar(255, 0, 0), -1);
        if (gamedata.carData[Blue].attack_plane || gamedata.carData[Blue].heal_plane)
            cv::circle(display, cv::Point(t.y, t.x), 4, cv::Scalar(255, 255, 0), 1);
        if (gamedata.carData[Blue].long_attack_map || gamedata.carData[Blue].short_attack_map)
            cv::circle(display, cv::Point(t.y, t.x), 7, cv::Scalar(0, 255, 0), 1);

        t = gamedata.planePoint;
        cv::circle(display, cv::Point(t.y, t.x), radius, cv::Scalar(255, 255, 0), -1);
        if (PlaneAttack == gamedata.planeStatus)
            cv::circle(display, cv::Point(t.y, t.x), RADIUS_PLANE_ATTACK, cv::Scalar(255, 255, 0), 1);
        else cv::circle(display, cv::Point(t.y, t.x), RADUIS_PLANE_HEAL, cv::Scalar(255, 255, 0), 1);
        if (gamedata.carData[Red].air_command)
            cv::circle(display, cv::Point(t.y, t.x), 4, cv::Scalar(0, 0, 255), 1);
        if (gamedata.carData[Blue].air_command)
            cv::circle(display, cv::Point(t.y, t.x), 4, cv::Scalar(255, 0, 0), 1);

        t = gamedata.propPoint;
        if (gamedata.propType != PropET) {
            cv::circle(display, cv::Point(t.y, t.x), radius, cv::Scalar(255, 0, 255), -1);
            sprintf(buffer, "%d", gamedata.propType);
            cv::putText(display, buffer, cv::Point(t.y - 3, t.x + 3), 1, 1, cv::Scalar(255, 255, 255));
            cv::circle(display, cv::Point(t.y, t.x), RADIUS_PROP, cv::Scalar(255, 0, 255), 1);
        }

        cv::rectangle(display, cv::Rect(20, 252, gamedata.carData[Red].health, 3), cv::Scalar(0, 0, 255), -1);
        cv::rectangle(display, cv::Rect(20, 249, gamedata.carData[Blue].health, 3), cv::Scalar(255, 0, 0), -1);

        return display;
    }

    cv::Mat TranslateGameData(int idx) {
        if (gamedata_list_.size() == 0)  return map_;
        if (idx < gamedata_list_.size())
            return TranslateGameData(gamedata_list_[idx]);
        else return TranslateGameData(gamedata_list_[gamedata_list_.size() - 1]);
    }

    GameData getGameData(int idx) {
        if (gamedata_list_.size() == 0)  return GameData();
        if (idx < gamedata_list_.size())
            return gamedata_list_[idx];
        else return gamedata_list_[gamedata_list_.size() - 1];
    }

    inline int getMaxRound() const { return gamedata_list_.size(); }

private:

    int size_;
    cv::Mat map_;
    std::vector<GameData> gamedata_list_;

};