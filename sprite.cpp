#include "sprite.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game.h"

Sprite::Sprite(const char* fileName, int xpos, int ypos) :
    m_pTexture(nullptr), m_iXPos(xpos), m_iYPos(ypos)
{
    if(nullptr != fileName)
    {
        m_pTexture = LoadTexture(fileName);
    }

    m_Rect.x = m_Rect.y = 0;
    m_Rect.w = m_Rect.h = 32;
}

Sprite::~Sprite()
{
    if(nullptr != m_pTexture)
    {
        SDL_DestroyTexture(m_pTexture);
        m_pTexture = nullptr;
    }
}

void Sprite::Update()
{
    // Override
}

void Sprite::Draw()
{
    // Define src rect
    SDL_Rect srcRect;
    srcRect.x = srcRect.y = 0;
    srcRect.w = srcRect.h = TILE_SIZE;

    // Define dest rect
    SDL_Rect destRect;
    destRect.x = m_iXPos*TILE_SIZE;
    destRect.y = m_iYPos*TILE_SIZE;
    destRect.w = destRect.h = TILE_SIZE;    

    SDL_RenderCopy(Game::m_pRenderer, m_pTexture, &srcRect, &destRect);
}

SDL_Texture* Sprite::LoadTexture(const char* fileName)
{
    SDL_Surface* tempSurface = IMG_Load(fileName);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::m_pRenderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;    
}

SDL_Texture* Sprite::Get_Image() const
{
    return m_pTexture;
}

bool Sprite::operator==(const Sprite &other) const
{
    return(m_pTexture == other.Get_Image());
}