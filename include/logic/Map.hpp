/*
 * 地图的行为
 */

#pragma once

#include"logic/Common.h"
#include"logic/Target.hpp"
#include"logic/Random.hpp"

#include<iostream>
#include<fstream>
#include<vector>

class Map {
public:
    //constructor
    Map(std::string Filename) {
        // load file
        std::ifstream in(Filename);
        if (!in) {
            std::cout << "[Error] Cannot load file " << Filename << std::endl;
            system("pause");
            exit(1);
        }

        in >> _map_size;

        for (int i = 0; i < _map_size; ++i)
            for (int j = 0; j < _map_size; ++j)
                _map[i][j] = getNum(in);

        in >> _targets_size;

        _targets.resize(_targets_size);
        for (int i = 0; i < _targets_size; ++i) {
            _targets[i].x = getNum(in); 
            _targets[i].y = getNum(in);
        }

        in.close();

        // init
        _tid = _random.Rand() % (_targets_size);

        //RefreshProp();
    }

    //更换target的位置tid，更新_target的状态，reverse代表是否因为BW道具强制换色
    void GenerateTarget(bool reverse = false) {  
		
		// _target
		_target.Reset();
		
		// _tid
		if (reverse)
		{
			// select another target with a different color
			std::vector<int> temp;
			unsigned char color = GetPointColor(_targets[_tid]);//the color of current tid
			for (int i = 0; i < _targets_size; ++i)
				if (i != _tid && GetPointColor(_targets[i]) != color) temp.push_back(i);
			_tid = temp[_random.Rand() % temp.size()];
		}
		else { 
			// select another target         
			int next_tid = _random.Rand() % (_targets_size - 1);
			if (next_tid < _tid) _tid = next_tid;
			else _tid = next_tid + 1;
		}
    }

	//更新target计时器 
	bool RefreshTarget()
	{
	    return _target.Refresh();
	}

	//Target是否存在
	bool HaveTarget() {
		return _target.isAlive();
	}


    //塔近攻
    void ShortAttack() {
        _target.ShortAttack();
    }
    //塔远攻
    void LongAttack() {
        _target.LongAttack();
    }
    //塔攻击(远/近)
    void Attack(bool critical) {
        if (critical) ShortAttack(); else LongAttack();
    }

    inline unsigned char GetPointColor(const Point& p) const { return _map[int(p.x)][int(p.y)]; }//return 0 or 255 in the map
    inline Point GetTargetPoint() const { return _targets[_tid]; }//return _tid
    inline double GetTargetHealth() const { return _target.GetHealth(); }//return 塔的血量
	inline int GetMapSize() const { return _map_size; }
	
private:
    //读数字
    unsigned char getNum(std::ifstream& in) {
        int v;
        in >> v;
        return v;
    }



    int _map_size;
    unsigned char _map[256][256];

    std::vector<Point> _targets; //目标点
    int _targets_size; //目标点总数
    int _tid; //当前目标点
    Target _target;//塔

    Random _random;
};
