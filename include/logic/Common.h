/*
 *  公共的头文件
 */

#pragma once

#include<string>
#include<vector>
#include<cmath>
#include<ctime>

// 时间控制
const int FREQ = 10;        // 逻辑频率
const int MAX_ROUND = 180 * FREQ; // 至多进行回合（限制比赛时间）


// Car 参数
const double HP_DEATH = 0.;       // 最低血量
const double HP_INITIAL = 200.;   // 初始血量
const double HEALTH_PROP = 40.;     // 道具瞬间回复血量

// Plane 参数
const double RADIUS_PLANE_ATTACK = 37.5;    // 攻击半径
const double RADUIS_PLANE_HEAL = 60.;     // 治疗半径
const int AIR_COMMAND_TIME = 10 * FREQ; // 制空权维持回合数
const double PLANE_ATTACK = 8. / FREQ;     // 飞机一回合攻击值
const double PLANE_HEAL = 4. / FREQ;       // 飞机一回合回复值

// Target 参数
const double HP_TARGET = 40.;      // 目标点HP
const double MAP_SHORT_ATTACK = 8. / FREQ; // 地图暴击一回合伤害值
const double MAP_LONG_ATTACK = 2. / FREQ;  // 地图普攻一回合伤害值
const double RADIUS_CRITICAL = 20.;      // 远/近攻击（暴击/普攻）的临界距离
const int TARGET_CD = 3 * FREQ;            // 目标点冷却时间

// Prop 参数
const double RADIUS_PROP = 20.;
const double PROP_AVOID = 90.;
const double PROP_MARGIN = 20.;
const int PROP_CD = 3 * FREQ;           // 道具冷却时间
const int PROP_ABANDON = 15 * FREQ;     // 道具没有被吃掉自动刷新
const int PROP_SIZE = 4;    

//parameters of Game
enum PropType { PropET = 0, PropHP = 1, PropBW = 2, PropAC = 3 }; //(EmpTy)空，（Black-White）黑白颠倒，(+HP)加生命，(Air Command)制空权
enum GameStatus { Running = 0, RedWin = 1, BlueWin = 2, Tie = 3 };
enum CarName { Red = 0, Blue = 1 };
enum PlaneStatus { PlaneHeal = 0, PlaneAttack = 1, PlaneNone = 10 };

//点的颜色：0代表黑，255代表白
struct Point {
    Point() : x(0), y(0) {}
    Point(unsigned char _x, unsigned char _y) : x(_x), y(_y) { }

    unsigned char x, y;

    double getDistance(const Point& a) const {
        return sqrt((a.x - x)*(a.x - x) + (a.y - y)*(a.y - y));
    }

    bool operator ==(const Point& p) const { return (x == p.x && y == p.y); }
};

