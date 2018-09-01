#ifndef EGG_H
#define EGG_H

#include "sprite.h"

class Egg : public Sprite
{
public:
    Egg(const char* fileName, int xpos, int ypos);
    ~Egg();

    void Update() override;
    void Move();

private:

};


#endif // EGG_H