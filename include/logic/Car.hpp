/*
 * 小车的行为
 */

#pragma once

#include"logic/Common.h"
class Car {
public:

    Car() {
        ResetCar();
    }

    //修改当前位置；将和血量相关的四个布尔型状态设为false；检查制空权回合数
    void Refresh(Point p, unsigned char c)
    {
        _pos = p;
        _color = c;
        _long_attack_map = false;
        _short_attack_map = false;
        _attack_plane = false;
        _heal_plane = false;

        if (_count_air_command >= AIR_COMMAND_TIME) {
            DismissAirCommand();
            _count_air_command = 0;
        }
        if (_air_command) _count_air_command++;
    }

    //初始化小车：生命、制空回合（补充）
    void ResetCar()
    {
        _health = HP_INITIAL;
        _count_air_command = 0;
        _air_command = false;
        _long_attack_map = false;
        _short_attack_map = false;
        _attack_plane = false;
        _heal_plane = false;
    }

    //自检
    void CheckHP() {
        if (_health > HP_INITIAL) _health = HP_INITIAL;
    }

    ////////////////////////////////攻击

    //地图小攻击，减血+改状态
    void LongAttackedByMap()
    {
        _health -= MAP_LONG_ATTACK;
        _long_attack_map = true;
    }
    //地图大攻击
    void ShortAttackedByMap()
    {
        _health -= MAP_SHORT_ATTACK;
        _short_attack_map = true;
    }
    //地图小/大攻击
    void AttackedByMap(bool critical)
    {
        if (critical) ShortAttackedByMap(); else LongAttackedByMap();
    }
    //飞机攻击
    void AttackedByPlane()
    {
        _health -= PLANE_ATTACK;
        _attack_plane = true;
    }
    
    ////////////////////////////////加血

    //飞机加血
    void HealedByPlane()
    {
        _health += PLANE_HEAL;
        _heal_plane = true;
    }
    //道具加血
    void HealByProp()
    {
        _health += HEALTH_PROP;
    }

    ////////////////////////////////制空权

    //获得制空权，持续一定回合数
    void AcquireAirCommand()
    {
        _air_command = true;
        _count_air_command = 0;
    }
    //解除制空权，清空count
    void DismissAirCommand()
    {
        _air_command = false;
        _count_air_command = 0;
    }

    inline bool IsAlive() { return (_health >= HP_DEATH); } //是否存活
    inline bool CommandAir() const { return _air_command; }//是否有制空权
    inline Point GetPoint() const { return this->_pos; }//返回小车位置
    inline double GetHealth() const { return _health; }//返回血量(补充）
    inline bool GetLongAttackMap() const { return _long_attack_map; }//返回是否被地图小攻击(补充）
    inline bool GetShortAttackMap() const { return _short_attack_map; }//返回是否被地图小攻击(补充）
    inline bool GetAttackMap() const {  return _long_attack_map || _short_attack_map; } //返回是否被地图攻击
    inline bool GetAttackPlane() const { return _attack_plane; }//返回是否被飞机攻击(补充）
    inline bool GetHealPlane() const { return _heal_plane; }//返回是否被飞机加血(补充）
	inline int GetCountAirCommand() const { return _count_air_command; }//返回制空权回合数
private:
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
