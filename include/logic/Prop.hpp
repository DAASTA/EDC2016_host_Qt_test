
#pragma once


#include <vector>

#include "logic/Common.h"

class Prop {

public:

	Prop()
		: _waiting_count(0)
        , _waiting_count_waste(0)
        , _prop(PropET)
	{
	}

	// ���������Ҫˢ�£�����true�����򷵻�false
	bool Refresh() {


		if (PropET == _prop) {
			++_waiting_count;
			if (_waiting_count >= PROP_CD) {
				_waiting_count = 0;
				return true;
			}
		}
        else {
            ++_waiting_count_waste;
            if (_waiting_count_waste >= PROP_ABANDON) {
                _waiting_count_waste = 0;
                return true;
            }
        }

		return false;
	}

    void Consume() {
        _waiting_count = 0;
        _prop = PropET;
    }

	void Generate(std::vector<Point> avoid_point_list, int _map_size, double distance = 50) {
		// ����һ�����ߣ����Ҳ��ܷǳ������������ļ�����(С��,Ŀ���ʲô��)

        // ��һ��ʱ�����ֵ���
        _prop = (PropType)(_random.Rand() % (PROP_SIZE - 1) + 1); // ����0��NULL����

        if (_prop != PropBW) {
            for (int i = 0; i < 10; i++)
            {
                _point.x = _random.Rand() % _map_size;
                _point.y = _random.Rand() % _map_size;

                if (_point.x<PROP_MARGIN || _point.x>_map_size - PROP_MARGIN) continue;
                if (_point.y<PROP_MARGIN || _point.y>_map_size - PROP_MARGIN) continue;

                bool flag = true;
                for (int k = 0; k < avoid_point_list.size(); ++k)
                    if (_point.getDistance(avoid_point_list[k]) < PROP_AVOID) {
                        flag = false;
                        break;
                    }
                if (flag) break;
            }
        }
        else {
            for (int i = 0; i < 10; i++)
            {
                _point.x = _random.Rand() % _map_size;
                _point.y = _random.Rand() % _map_size;

                if (_point.x<PROP_MARGIN || _point.x>_map_size - PROP_MARGIN) continue;
                if (_point.y<PROP_MARGIN || _point.y>_map_size - PROP_MARGIN) continue;

                bool flag = false;
                for (int k = 0; k < avoid_point_list.size(); ++k)
                    if (_point.getDistance(avoid_point_list[k]) < PROP_AVOID) {
                        flag = true;
                        break; 
                    }
                if (flag) break;
            }
        }

	
	}

	bool CheckPoint(Point p) { //���˵��Ƿ���� _point �㹻��

        return PropET != _prop && p.getDistance(_point) < PROP_DISTANCE;
	}

	inline Point getPoint() const { return _point; }
	inline PropType getPropType() const { return _prop; }

private:

	Point _point;
	PropType _prop;

	Random _random;

	int _waiting_count;
    int _waiting_count_waste;
};