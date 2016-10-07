

#pragma once

#include"logic/Common.h"

class Target {
public:

    Target()
        : _health(0)
        , _waiting_count(0)
        , _alive(false)
    { }

    void Reset() { //生命值置满
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

    void ShortAttack() { _health -= MAP_SHORT_ATTACK; _alive = _health > 0; }//近战减血
    void LongAttack() { _health -= MAP_LONG_ATTACK; _alive = _health > 0; }//远战减血
    void Attack(bool critical) { if (critical) ShortAttack(); else LongAttack(); } //远/近减血

	inline double GetHealth() const { return _health; }//返回血量
	inline bool isAlive() const { return _alive;  }

private:

	
    double _health;//生命值
	bool _alive;

	int _waiting_count;

};