// Created by sugar10w, 2016.8
//   Logic of the game

#include "car.hpp"


enum GameStatus {
    Pause,
    Running,
    GameOver,
    Error
};

class Game {
    
public:
    Game();

    inline GameStatus getStatus() { return status_; };

private:
    GameStatus status_;

};