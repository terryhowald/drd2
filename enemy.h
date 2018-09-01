#ifndef ENEMY_H
#define ENEMY_H

#include "sprite.h"

class Enemy : public Sprite
{
  public:
    Enemy(const char *fileName, int xpos, int ypos);
    ~Enemy();

    void Update() override;
    void Move();
    int GetDir();

  private:
    int m_iSpeed;
    int m_iHorizontal;
    int m_iVertical;
    int m_iDir;
};

#endif // ENEMY_H