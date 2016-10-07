#include "logic/Game.h"
#include <iostream>
#include <vector>

using namespace std;

Game::Game(string filename)
    : _map(filename)
    , _game_status(Running)
    , _round_count(0)
{
	//打开文件
	out_file.open("log.txt");
}

void Game::Refresh(const Point & pcar1, const Point & pcar2, const Point & pplane)
{
    //刷新所有位置
    _car[Red].Refresh(pcar1, _map.GetPointColor(pcar1));
    _car[Blue].Refresh(pcar2, _map.GetPointColor(pcar2));
    _plane.Refresh(pplane, _map.GetPointColor(_map.GetTargetPoint()));

    //道具
    if (_car[Red].GetPoint().getDistance(_prop.getPoint()) < _car[Blue].GetPoint().getDistance(_prop.getPoint()))
        CheckProp(Red);
    else CheckProp(Blue);

    //血量计算
    SettleDamage();

    //更新 .. . ..
    if (_prop.Refresh()) GenerateProp();
    if (_map.RefreshTarget()) _map.GenerateTarget();
    
    //判断游戏是否结束
    Judge();	

    //回合信息写入文件
    out_file << getGameData().getString() << endl;
}

GameData Game::getGameData()
{
	GameData g;
	g.round=_round_count;
	g.planePoint = _plane.GetPlanePos();
	g.planeStatus = _plane.GetPlaneStatus();
	g.targetHealth = _map.GetTargetHealth();
    g.targetPoint = _map.GetTargetPoint();
	g.propPoint = _prop.getPoint();
	g.propType = _prop.getPropType();
	for (int i = 0; i < 2; i++)
	{
		g.carData[i].health = _car[i].GetHealth();
		g.carData[i].pos = _car[i].GetPoint();
		g.carData[i].color = _map.GetPointColor(_car[i].GetPoint());
		g.carData[i].short_attack_map = _car[i].GetShortAttackMap();
		g.carData[i].long_attack_map = _car[i].GetLongAttackMap();
		g.carData[i].attack_plane = _car[i].GetAttackPlane();
		g.carData[i].heal_plane = _car[i].GetHealPlane();
		g.carData[i].air_command = _car[i].CommandAir();
		g.carData[i].count_air_command = _car[i].GetCountAirCommand();
	}
	return g;
}

void Game::CheckProp(CarName name) 
{
    Car & c = _car[name];
    Car & nc = _car[1 - name]; // 另一个小车

    if (_prop.CheckPoint(c.GetPoint()))
    {
		switch (_prop.getPropType())
		{
		case PropET: //道具0： Empty
			return;
		case PropHP: //道具1： HP
			c.HealByProp();
			break;
		case PropBW: //道具2： BlackWhite
			_map.GenerateTarget(true);
			_plane.SetPlaneStatus(_map.GetPointColor(_map.GetTargetPoint()));
			break;
		case PropAC: //道具3： AC
			c.AcquireAirCommand();
            nc.DismissAirCommand();
			break;
		}
		
        _prop.Consume();
    }
}

void Game::Attack(CarName car_name, bool critical)
{
    _car[car_name].AttackedByMap(critical);
    _map.Attack(critical);
}

void Game::SettleDamage() {
    Point r_pos = _car[Red].GetPoint();
    Point b_pos = _car[Blue].GetPoint();

    if (_map.HaveTarget()) {

        Point tid_pos = _map.GetTargetPoint();
        unsigned char tower_color = _map.GetPointColor(tid_pos)
            , r_color = _map.GetPointColor(r_pos)
            , b_color = _map.GetPointColor(b_pos);
        double r_distance = r_pos.getDistance(tid_pos)
            , b_distance = b_pos.getDistance(tid_pos);
        bool critical = (r_color == tower_color && r_distance <= CRITICAL_DISTANCE)
            || (b_color == tower_color && b_distance <= CRITICAL_DISTANCE);

        if (r_color == tower_color && b_color == tower_color) { //都位于正确颜色
            if (r_distance == b_distance) { //都位于正确颜色，且两者距离相等
                Attack(Red, critical);
                Attack(Blue, critical);
            }
            else { //都位于正确颜色，且两者距离不相等
                if (r_distance < b_distance) Attack(Blue, critical);
                else  Attack(Red, critical);
            }
        }
        else if (r_color != tower_color && b_color != tower_color) { //都位于错误颜色
            Attack(Red, critical);
            Attack(Blue, critical);
        }
        else {
            if (r_color == tower_color) Attack(Blue, critical); //只有红方位于正确色区
            else Attack(Red, critical); //只有蓝方位于正确色区
        }
    }

    switch (_plane.GetPlaneStatus()) {
    case PlaneNone:
        break;
    case PlaneAttack:
        if (_plane.IsInAttackRange(r_pos)) _car[Red].AttackedByPlane();
        if (_plane.IsInAttackRange(b_pos)) _car[Blue].AttackedByPlane();
        break;
    case PlaneHeal:
        if (_plane.IsInHealRange(r_pos)) _car[Red].HealedByPlane();
        if (_plane.IsInHealRange(b_pos)) _car[Blue].HealedByPlane();
        break;
    default:
        cout << "[Error] Unknown Plane Status!" << _plane.GetPlaneStatus() << endl;
        system("pause");
    }

}

void Game::GenerateProp()
{
	vector<Point> point_list;
	point_list.push_back(_car[Red].GetPoint());
	point_list.push_back(_car[Blue].GetPoint());
	point_list.push_back(_map.GetTargetPoint());
	_prop.Generate(point_list, _map.GetMapSize()); 
}

void Game::Judge()
{
    _car[Red].CheckHP();
    _car[Blue].CheckHP();

    if (_round_count < MAX_ROUND) //当比赛还有剩余时间
    {
		
		

		if (_car[Red].IsAlive() && _car[Blue].IsAlive()) //比赛继续
        {
            _round_count++;
            _game_status = Running;
        }
		else if (!_car[Red].IsAlive() && !_car[Blue].IsAlive()) //同时死亡，平局
		{
			_game_status = Tie; 
			std::cout << "[Game] No survior!" << endl;
		}
        else if ( _car[Red].IsAlive() && !_car[Blue].IsAlive())
			_game_status = RedWin;
        else if (!_car[Red].IsAlive() &&  _car[Blue].IsAlive())
            _game_status = BlueWin;

    }
    else //比赛时间到
    {
		cout << "[Game] Time out!" << endl;

        if (_car[Red].GetHealth() == _car[Blue].GetHealth())
            _game_status = Tie;
        else if (_car[Red].GetHealth() > _car[Blue].GetHealth())
            _game_status = RedWin;
        else
            _game_status = BlueWin;
    }

    switch (_game_status)
    {
    case Tie:
        cout << "[Game] Game Over: Tie!" << endl;
        break;
    case RedWin:
        cout << "[Game] Game Over: Red Win!" << endl;
        break;
    case BlueWin:
        cout << "[Game] Game Over: Blue Win!" << endl;
        break;
    default:
        break;
    }
}



