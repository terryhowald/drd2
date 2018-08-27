#ifndef ENEMY_H
#define ENEMY_H

#include "sprite.h"

class Enemy : public Sprite
{
public:
    Enemy(const char* fileName, int xpos, int ypos);
    ~Enemy();

    void Update();
    void Move();

private:

};


#endif // ENEMY_H