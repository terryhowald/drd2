#include "spritegroup.h"

SpriteGroup::SpriteGroup()
{}

SpriteGroup::~SpriteGroup()
{}

SpriteGroup SpriteGroup::Copy()
{
    SpriteGroup new_group;

    for(int i=0; i<sprites_size; i++)
    {
        new_group.Add(sprites[i]);
    }

    return(new_group);
}

void SpriteGroup::Add(Sprite *sprite)
{
    sprites.push_back(sprite);
    sprites_size = sprites.size();
}

void SpriteGroup::Remove(Sprite sprite_object)
{
    for(int i=0; i< sprites_size; i++)
    {
        if( *sprites[i] == sprite_object )
        {
            sprites.erase(sprites.begin() + i);
        }
    }
    sprites_size = sprites.size();
}

bool SpriteGroup::Has(Sprite sprite_object)
{
    for(int i=0; i< sprites_size; i++)
    {
        if( *sprites[i] == sprite_object )
            return true;
    }    

    return false;    
}

void SpriteGroup::Update()
{
    if(!sprites.empty())
    {
        for(int i=0; i< sprites_size; i++)
        {
            sprites[i]->Update();
        }   
    }   
} 

void SpriteGroup::Draw()
{
    if(!sprites.empty())
    {
        for(int i=0; i< sprites_size; i++)
        {
            sprites[i]->Draw();
        }   
    }   
}

void SpriteGroup::Empty()
{
    sprites.clear();
    sprites_size = sprites.size();
} 

int SpriteGroup::Size()
{
    return sprites_size;
} 

std::vector <Sprite*> SpriteGroup::Get_Sprites()
{
    return sprites;
}