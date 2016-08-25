// Created by sugar10w, 2016.8
//   Plane

struct Position3d
{
    int x, y, z;
    // TODO
};

class Plane {

public:
    Plane() {}

    bool setPosition(Position3d pos) { pos_ = pos; }
    

private:
    Position3d pos_;
};


class Dobby : public Plane {
    // todo (comm) 
};