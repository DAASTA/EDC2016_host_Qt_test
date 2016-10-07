/*
 * ��ͼ����Ϊ
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

    //����target��λ��tid������_target��״̬��reverse�����Ƿ���ΪBW����ǿ�ƻ�ɫ
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

	//����target��ʱ�� 
	bool RefreshTarget()
	{
	    return _target.Refresh();
	}

	//Target�Ƿ����
	bool HaveTarget() {
		return _target.isAlive();
	}


    //������
    void ShortAttack() {
        _target.ShortAttack();
    }
    //��Զ��
    void LongAttack() {
        _target.LongAttack();
    }
    //������(Զ/��)
    void Attack(bool critical) {
        if (critical) ShortAttack(); else LongAttack();
    }

    inline unsigned char GetPointColor(const Point& p) const { return _map[int(p.x)][int(p.y)]; }//return 0 or 255 in the map
    inline Point GetTargetPoint() const { return _targets[_tid]; }//return _tid
    inline double GetTargetHealth() const { return _target.GetHealth(); }//return ����Ѫ��
	inline int GetMapSize() const { return _map_size; }
	
private:
    //������
    unsigned char getNum(std::ifstream& in) {
        int v;
        in >> v;
        return v;
    }



    int _map_size;
    unsigned char _map[256][256];

    std::vector<Point> _targets; //Ŀ���
    int _targets_size; //Ŀ�������
    int _tid; //��ǰĿ���
    Target _target;//��

    Random _random;
};
