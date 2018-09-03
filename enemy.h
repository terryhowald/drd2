#ifndef ENEMY_H
#define ENEMY_H

#include "sprite.h"

class Enemy : public Sprite
{
  public:
    Enemy(const char *fileName, int xpos, int ypos, int dir);
    ~Enemy();

    void Update() override;
    void Move();
    int GetDir();
    void SetDir(int dir);
    void Maneuver();

  private:
    int m_iSpeed;
    int m_iHorizontal;
    int m_iVertical;
    int m_iDir;
    bool m_bDirChange;
};

#endif // ENEMY_H