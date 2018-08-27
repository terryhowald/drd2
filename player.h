#ifndef PLAYER_H
#define PLAYER_H

#include "sprite.h"

class Player : public Sprite
{
public:
    Player(const char* fileName, int xpos, int ypos);
    ~Player();

    void Update();
    void Move();

private:
};

#endif // PLAYER_H