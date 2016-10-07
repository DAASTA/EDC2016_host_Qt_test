#pragma once

#include <string>

#include "logic/Common.h"

struct CarData
{
    //С����Ϣ
    double health;//Ѫ��
    Point pos;//λ��
    unsigned char color;//С�����ڵ����ɫ�����ڰ�
    bool long_attack_map;//�Ƿ񱻵�ͼС����
    bool short_attack_map;//�Ƿ񱻵�ͼ�󹥻�
    bool attack_plane;//�Ƿ񱻷ɻ�����
    bool heal_plane;//�Ƿ񱻷ɻ���Ѫ
    bool air_command;//�Ƿ����ƿ�Ȩ
    int count_air_command;//��¼�ƿ�Ȩ�غ���

    std::string getString() {
        char buffer[512];
        sprintf(buffer, " [ HP %.1lf pos (%d,%d) color %d A L%d S%d PA%d PH%d AC%d(%d) ] ",
            health, pos.x, pos.y, color, 
            long_attack_map, short_attack_map, 
            attack_plane, heal_plane, 
            air_command, count_air_command);
        return buffer;
        // return all data
    }

};


struct GameData
{
    //�غ���
    int round;
    //С����Ϣ
    CarData carData[2];//�������camel����

    //�ɻ���Ϣ
    Point planePoint;
    PlaneStatus planeStatus;


    //Ŀ�����Ϣ
    double targetHealth;//����ֵ
    Point targetPoint; //Ŀ���λ��

    //��ͼ��̬��Ϣ
    Point propPoint;//��������λ��
    PropType propType; // ��������

    std::string getString() {

        char buffer[512];
        sprintf(buffer, " round %d : target (%d,%d) %.1lf ; plane (%d,%d) %d ; prop (%d,%d) %d ; ",
            round,
            targetPoint.x, targetPoint.y, targetHealth,
            planePoint.x, planePoint.y, planeStatus,
            propPoint.x, propPoint.y, propType);
        return (buffer + carData[Red].getString() + carData[Blue].getString());
        // ( call carData.getString()) and return all data 
    }

};

