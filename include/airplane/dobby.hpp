#pragma once

#include<Windows.h>

#include"logic/Common.h"
#include"logic/Random.hpp"

class Dobby {

public:        

    typedef struct {
        double x, y, z;   // µ¥Î»Îª cm 
        int writeflag;
    } pos_xyz_info;

    Dobby()
        : pos_(128, 128)
        , target_(128, 128)
        , valid_(false)
        , true_size_256(2.05)
        , true_size_cross(2.28)
        , high_z(150)
        , low_z(75)
    {
        map_to_true = true_size_256 / 256;

        SetTarget(target_);
    }

    inline void SetTarget(Point p, bool high = true) {
        target_ = p; 

        // set the memory
        LPCWSTR strMapName_pos = L"ZeroTechDobbyTarget";
        HANDLE hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, strMapName_pos);
        LPVOID pBuffer;
        pos_xyz_info* p_xyz;

        if (NULL == hMap) {
            hMap = CreateFileMapping(
                INVALID_HANDLE_VALUE,
                NULL,
                PAGE_READWRITE,
                0,
                100,
                strMapName_pos
            );
            pBuffer = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
            p_xyz = (pos_xyz_info*)pBuffer;

            // init
            p_xyz->x = (double)128*map_to_true * 100;
            p_xyz->y = (double)128*map_to_true * 100;
            p_xyz->z = high_z;
            p_xyz->writeflag = 1;
            return;
        }

        pBuffer = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        p_xyz = (pos_xyz_info*)pBuffer;

        p_xyz->x = (double)p.x*map_to_true * 100;
        p_xyz->y = (double)p.y*map_to_true * 100;
        p_xyz->z = high ? high_z : low_z;
        p_xyz->writeflag = 1;
        return;
    }
    
    

    inline Point GetPos() {
        /*int dx = random.Rand(2);
        int dy = random.Rand(2);
        if (pos_.x > target_.x) dx = -dx;
        if (pos_.y > target_.y) dy = -dy;
        int x = dx + pos_.x;
        int y = dy + pos_.y;
        if (x < 0) x = 0; 
        if (x > 255) x = 255;
        if (y < 0) y = 0;
        if (y > 255) y = 255;        
        pos_.x = x;
        pos_.y = y;*/

        // read memory // TODO mapper translater
        LPCWSTR strMapName_pos = L"ZeroTechDobbyPos";
        HANDLE hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, strMapName_pos);
        if (NULL == hMap) {
            valid_ = false;
            return pos_;
        }
        else {
            LPVOID pBuffer = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
            pos_xyz_info* p_xyz = (pos_xyz_info*)pBuffer;
            
            if (0 == p_xyz->writeflag) {
                valid_ = false;
                return pos_;
            }

            pos_.x = p_xyz->x / map_to_true;
            pos_.y = p_xyz->y / map_to_true;
            p_xyz->writeflag = 0;
        }

        return pos_;
    }

    inline bool isValid() { return valid_; }

private:

    bool valid_;

    double true_size_256;
    double true_size_cross;
    double map_to_true;

    double high_z, low_z;

    Point pos_;
    Point target_;

    Random random;

};