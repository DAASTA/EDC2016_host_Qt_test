/*
 * 飞机的行为
 */


#pragma once

#include "logic/Common.h"

class Plane {
public:
    //刷新飞机位置,根据Tower的颜色来决定飞机状态
    void Refresh(Point p, unsigned char color) {
        _pos.x = p.x;
        _pos.y = p.y;
        SetPlaneStatus(color);
    }

    //判断p是否在攻击范围内
    bool IsInAttackRange(Point p) {
        return(_pos.getDistance(p) <= RADIUS_PLANE_ATTACK);

    }
    
    //判断p是否在治疗范围内
    bool IsInHealRange(Point p) {
        return(_pos.getDistance(p) <= RADUIS_PLANE_HEAL);

    }
	//返回飞机位置
	Point GetPlanePos()
	{
		return _pos;
	}
	
	//返回飞机状态
    PlaneStatus GetPlaneStatus() {
        return _status;
    }

    //设置飞机状态
    void Plane::SetPlaneStatus(unsigned char color) {
        switch (color)
        {
        case 0:
            _status = PlaneAttack;
            break;
        default:
            _status = PlaneHeal;
            break;
        } 
    }

private:
    Point _pos;
    PlaneStatus _status;
};