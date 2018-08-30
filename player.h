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
    void Set_Speed(int iSpeed);

private:
    int m_iXSpeed;
    int m_iHorizontal;
    int m_iYSpeed;
    int m_iVertical;
    int m_iOrientation;
    int m_iCount;
};

#endif // PLAYER_H