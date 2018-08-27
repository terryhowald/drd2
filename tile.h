#ifndef TILE_H
#define TILE_H

#include "sprite.h"

class Tile : public Sprite
{
public:
    Tile(const char* fileName, int xpos, int ypos);
    ~Tile();

    void Update();
    void Move();

private:

};

#endif // TILE_H