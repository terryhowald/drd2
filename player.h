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
    void Set_Angle(double dAngle);

private:
    int m_iXSpeed;
    int m_iHorizontal;
    int m_iYSpeed;
    int m_iVertical;
};

#endif // PLAYER_H