/*
 * �ɻ�����Ϊ
 */


#pragma once

#include "logic/Common.h"

class Plane {
public:
    //ˢ�·ɻ�λ��,����Tower����ɫ�������ɻ�״̬
    void Refresh(Point p, unsigned char color) {
        _pos.x = p.x;
        _pos.y = p.y;
        SetPlaneStatus(color);
    }

    //�ж�p�Ƿ��ڹ�����Χ��
    bool IsInAttackRange(Point p) {
        return(_pos.getDistance(p) <= RADIUS_PLANE_ATTACK);

    }
    
    //�ж�p�Ƿ������Ʒ�Χ��
    bool IsInHealRange(Point p) {
        return(_pos.getDistance(p) <= RADUIS_PLANE_HEAL);

    }
	//���طɻ�λ��
	Point GetPlanePos()
	{
		return _pos;
	}
	
	//���طɻ�״̬
    PlaneStatus GetPlaneStatus() {
        return _status;
    }

    //���÷ɻ�״̬
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