// Created by sugar10w, 2016.8
//   Car of the players

struct Position2d
{
    int x, y;
    int deg;
};

class Car {
public:
    Car() {};

    bool setPosition(Position2d pos) { pos_ = pos; }

private:
    Position2d pos_;

};
