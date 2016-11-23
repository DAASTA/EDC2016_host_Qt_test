#pragma once

#include <string>

#include "logic/Common.h"

const static char* const format_car =  "     %4.1lf %3d %3d   %1d   %3d %3d   %1d %1d    %1d %3d \r\n";
const static char* const format_car_in = "%lf%d%d%d%d%d%d%d%d%d";

const static char* const format_game = "%3d  %3d %3d %4.1lf   %3d %3d %1d    %3d %3d %1d  \r\n";
const static char* const format_game_in = "%d%d%d%lf%d%d%d%d%d%d";

class CarData
{
public:
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

    bool out_of_range() {//�Ƿ����
        return (pos.x <= 0 || pos.x >= 255 || pos.y <= 0 || pos.y >= 255);
    } 

    std::string getString() {
        char buffer[512];

        sprintf(buffer, format_car,
            health, pos.x, pos.y, color, 
            long_attack_map, short_attack_map, 
            attack_plane, heal_plane, 
            air_command, count_air_command);
        return buffer;
        // return all data
    }

    void loadString(char* buffer) {
        
        sscanf(buffer, format_car_in,
            &health, &pos.x, &pos.y, &color,
            &long_attack_map, &short_attack_map,
            &attack_plane, &heal_plane,
            &air_command, &count_air_command);
    }
};


class GameData
{
public:
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
        
        sprintf(buffer, format_game,
            round,
            targetPoint.x, targetPoint.y, targetHealth,
            planePoint.x, planePoint.y, planeStatus,
            propPoint.x, propPoint.y, propType);
        return (buffer + carData[Red].getString() + carData[Blue].getString());
    }

    void loadString(char* buffer) {

        sscanf(buffer, format_game_in,
            &round,
            &(targetPoint.x), &(targetPoint.y), &targetHealth,
            &(planePoint.x), &(planePoint.y), &planeStatus,
            &(propPoint.x), &(propPoint.y), &propType);
    }

};

