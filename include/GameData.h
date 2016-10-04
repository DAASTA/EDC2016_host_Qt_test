#ifndef GAMEDATA_H
#define GAMEDATA_H

#include "Common.h"

struct CarData
{
    //小车信息
    double _health;//血量
    Point _pos;//位置
    unsigned char _color;//小车所在点的颜色，即黑白

    bool _long_attack_map;//是否被地图小攻击
    bool _short_attack_map;//是否被地图大攻击
    bool _attack_plane;//是否被飞机攻击
    bool _heal_plane;//是否被飞机加血

    bool _air_command;//是否有制空权
    int _count_air_command;//记录制空权回合数
    
};


struct GameData
{
    //小车信息
    CarData carData[2];//对象采用camel命名
    
    
    //飞机信息
    Point planePos;
    PlaneStatus planeStatus;
    
    
    //塔信息
    double towerHealth;//生命值
    
    
    //地图动态信息
    Point _prop_pos;//道具所在位置
    Prop _prop; // 道具类型
} ;

struct GameData gameData;

#endif
