/*
 * С������Ϊ
 */

#pragma once

#include"logic/Common.h"
class Car {
public:

    Car() {
        ResetCar();
    }

    //�޸ĵ�ǰλ�ã�����Ѫ����ص��ĸ�������״̬��Ϊfalse������ƿ�Ȩ�غ���
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

    //��ʼ��С�����������ƿջغϣ����䣩
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

    //�Լ�
    void CheckHP() {
        if (_health > HP_INITIAL) _health = HP_INITIAL;
    }

    ////////////////////////////////����

    //��ͼС��������Ѫ+��״̬
    void LongAttackedByMap()
    {
        _health -= MAP_LONG_ATTACK;
        _long_attack_map = true;
    }
    //��ͼ�󹥻�
    void ShortAttackedByMap()
    {
        _health -= MAP_SHORT_ATTACK;
        _short_attack_map = true;
    }
    //��ͼС/�󹥻�
    void AttackedByMap(bool critical)
    {
        if (critical) ShortAttackedByMap(); else LongAttackedByMap();
    }
    //�ɻ�����
    void AttackedByPlane()
    {
        _health -= PLANE_ATTACK;
        _attack_plane = true;
    }
    
    ////////////////////////////////��Ѫ

    //�ɻ���Ѫ
    void HealedByPlane()
    {
        _health += PLANE_HEAL;
        _heal_plane = true;
    }
    //���߼�Ѫ
    void HealByProp()
    {
        _health += HEALTH_PROP;
    }

    ////////////////////////////////�ƿ�Ȩ

    //����ƿ�Ȩ������һ���غ���
    void AcquireAirCommand()
    {
        _air_command = true;
        _count_air_command = 0;
    }
    //����ƿ�Ȩ�����count
    void DismissAirCommand()
    {
        _air_command = false;
        _count_air_command = 0;
    }

    inline bool IsAlive() { return (_health >= HP_DEATH); } //�Ƿ���
    inline bool CommandAir() const { return _air_command; }//�Ƿ����ƿ�Ȩ
    inline Point GetPoint() const { return this->_pos; }//����С��λ��
    inline double GetHealth() const { return _health; }//����Ѫ��(���䣩
    inline bool GetLongAttackMap() const { return _long_attack_map; }//�����Ƿ񱻵�ͼС����(���䣩
    inline bool GetShortAttackMap() const { return _short_attack_map; }//�����Ƿ񱻵�ͼС����(���䣩
    inline bool GetAttackMap() const {  return _long_attack_map || _short_attack_map; } //�����Ƿ񱻵�ͼ����
    inline bool GetAttackPlane() const { return _attack_plane; }//�����Ƿ񱻷ɻ�����(���䣩
    inline bool GetHealPlane() const { return _heal_plane; }//�����Ƿ񱻷ɻ���Ѫ(���䣩
	inline int GetCountAirCommand() const { return _count_air_command; }//�����ƿ�Ȩ�غ���
private:
    double _health;//Ѫ��
    Point _pos;//λ��
    unsigned char _color;//С�����ڵ����ɫ�����ڰ�

    bool _long_attack_map;//�Ƿ񱻵�ͼС����
    bool _short_attack_map;//�Ƿ񱻵�ͼ�󹥻�
    bool _attack_plane;//�Ƿ񱻷ɻ�����
    bool _heal_plane;//�Ƿ񱻷ɻ���Ѫ

    bool _air_command;//�Ƿ����ƿ�Ȩ
    int _count_air_command;//��¼�ƿ�Ȩ�غ���

};
