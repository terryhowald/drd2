#ifndef SPRITEGROUP_H
#define SPRITEGROUP_H

#include "sprite.h"
#include <vector>

class SpriteGroup
{
public:
    SpriteGroup();
    ~SpriteGroup();

    SpriteGroup Copy();
    void Add(Sprite *sprite);
    void Remove(Sprite sprite_object);
    bool Has(Sprite sprite_object);
    void Update();
    void Draw();
    void Empty();    
    int Size();    
    std::vector <Sprite*> Get_Sprites();

private:
    std::vector <Sprite*> sprites;
    int sprites_size;
};

#endif // SPRITEGROUP_H