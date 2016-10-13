#pragma once

#include <string>

#include "logic/Common.h"

const static char* const format_car =  "     %4.1lf %3d %3d   %1d   %3d %3d   %1d %1d    %1d %3d \r\n";
const static char* const format_car_in = "%lf%d%d%d%d%d%d%d%d%d";

const static char* const format_game = "%3d  %3d %3d %4.1lf   %3d %3d %1d    %3d %3d %1d  \r\n";
const static char* const format_game_in = "%d%d%d%lf%d%d%d%d%d%d";

struct CarData
{
    

    //小车信息
    double health;//血量
    Point pos;//位置
    unsigned char color;//小车所在点的颜色，即黑白
    bool long_attack_map;//是否被地图小攻击
    bool short_attack_map;//是否被地图大攻击
    bool attack_plane;//是否被飞机攻击
    bool heal_plane;//是否被飞机加血
    bool air_command;//是否有制空权
    int count_air_command;//记录制空权回合数

    bool out_of_range; //是否出界

    std::string getString() {
        char buffer[512];
        

        sprintf(buffer, format_car,
            health, pos.x, pos.y, color, 
            long_attack_map, short_attack_map, 
            attack_plane, heal_plane, 
            air_command, count_air_command);
        return buffer;
        // return all data
    }

    void loadString(char* buffer) {
        
        sscanf(buffer, format_car_in,
            &health, &pos.x, &pos.y, &color,
            &long_attack_map, &short_attack_map,
            &attack_plane, &heal_plane,
            &air_command, &count_air_command);
    }
};


struct GameData
{
    //回合数
    int round;
    //小车信息
    CarData carData[2];//对象采用camel命名

    //飞机信息
    Point planePoint;
    PlaneStatus planeStatus;


    //目标点信息
    double targetHealth;//生命值
    Point targetPoint; //目标点位置

    //地图动态信息
    Point propPoint;//道具所在位置
    PropType propType; // 道具类型

    std::string getString() {
        char buffer[512];
        
        sprintf(buffer, format_game,
            round,
            targetPoint.x, targetPoint.y, targetHealth,
            planePoint.x, planePoint.y, planeStatus,
            propPoint.x, propPoint.y, propType);
        return (buffer + carData[Red].getString() + carData[Blue].getString());
    }

    void loadString(char* buffer) {

        sscanf(buffer, format_game_in,
            &round,
            &(targetPoint.x), &(targetPoint.y), &targetHealth,
            &(planePoint.x), &(planePoint.y), &planeStatus,
            &(propPoint.x), &(propPoint.y), &propType);
    }

};

