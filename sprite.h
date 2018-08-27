#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>

class Sprite 
{
public:
    Sprite(const char* fileName, int xpos, int ypos);
    ~Sprite();

    void Update();
    void Draw();
    SDL_Texture* Get_Image() const;  
    bool operator==(const Sprite &other) const; 

protected:
    SDL_Texture* m_pTexture;
    SDL_Rect m_Rect;
    int m_iXPos;
    int m_iYPos;

private:
    SDL_Texture* LoadTexture(const char* fileName);
    void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
};

#endif // SPRITE_H