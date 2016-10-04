#pragma once

/*
 *  公共的头文件
 */

#include<string>
#include<vector>
#include<cmath>
#include<ctime>

#include"Random.hpp"

 // const int MAP_SIZE = 256;//地图尺寸，已改为从文件读取
const int MAX_ROUND = 3000;//至多进行3000回合（限制比赛时间）
const int TOWER_SUSPEND = 40; //多少回合不会出现塔

// Car 参数
const double DEATH_BlOOD = 0;       // 最低血量
const double INITIAL_BlOOD = 200;   // 初始血量
const double PLANE_ATTACK = 50;     // 飞机一回合攻击值
const double PLANE_HEAL = 20;       // 飞机一回合回复值
const double MAP_SHORT_ATTACK = 10; // 地图暴击一回合伤害值
const double MAP_LONG_ATTACK = 2;   // 地图普攻一回合伤害值
const double AIR_COMMAND_TIME = 20; // 制空权维持回合数
const double HEALTH_PROP = 100;     // 道具瞬间回复血量

// Plane 参数
const double ATTACK_RADIUS = 50;    // 攻击半径
const double HEAL_RADUIS = 100;     // 治疗半径
enum PlaneStatus { PlaneNone, PlaneAttack, PlaneHeal };

// Tower 参数
extern bool IsTower; //塔是否存在
extern int TowerSuspend; //当前已经有多少回合没有出现塔
const double TOWER_HEALTH = 100;    // 目标点点数
const double SHORT_ATTACK = 10;     // 目标点暴击消耗点数
const double LONG_ATTACK = 2;       // 目标点普攻消耗点数
const double CRITICAL_DISTANCE = 50;// 远/近攻击（暴击/普攻）的临界距离

//parameters of Game
const int PROP_SIZE = 4;
enum Prop { PropET, PropHP, PropBW, PropAC }; //(EmpTy)空，（Black-White）黑白颠倒，(+HP)加生命，(Air Command)制空权
enum GameStatus { Running, RedWin, BlueWin, Tie };
enum CarName { Red = 0, Blue = 1 };

//点的颜色：0代表黑，255代表白
struct Point {
    Point() : x(0), y(0) {}
    Point(unsigned char _x, unsigned char _y) : x(_x), y(_y) {}

    unsigned char x, y;

    double getDistance(const Point& a) const
    {
        return sqrt((a.x - x)*(a.x - x) + (a.y - y)*(a.y - y));
    }

    bool operator ==(const Point& p) const { return (x == p.x && y == p.y); }
};
