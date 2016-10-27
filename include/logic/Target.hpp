

#pragma once

#include"logic/Common.h"

class Target {
public:

    Target()
        : _health(0)
        , _waiting_count(0)
        , _alive(false)
    { }

    void Reset() { //����ֵ����
		_health = HP_TARGET; 
		_waiting_count = 0;
		_alive = true;
	}

	bool Refresh() { 

		if (!_alive) {
			++_waiting_count;
			if (_waiting_count >= TARGET_CD) {
				_waiting_count = 0;
				return true;
			}
		}
		return false;
	} //refresh

    void ShortAttack() { //��ս��Ѫ
        if (!TARGET_NONEXISTENT) _health -= MAP_SHORT_ATTACK;
        _alive = _health > 0; 
    }
    void LongAttack() { //Զս��Ѫ
        if (!TARGET_NONEXISTENT) _health -= MAP_LONG_ATTACK; 
        _alive = _health > 0; 
    }
    void Attack(bool critical) { //Զ/����Ѫ
        if (critical) ShortAttack(); 
        else LongAttack(); 
    } 

	inline double GetHealth() const { return _health; }//����Ѫ��
	inline bool isAlive() const { return _alive;  }

private:

	
    double _health;//����ֵ
	bool _alive;

	int _waiting_count;

};