#include "enemy.h"
#include "game.h"
#include <cstdlib>

Enemy::Enemy(const char* fileName, int xpos, int ypos, int dir)
    : Sprite(fileName, xpos, ypos), m_iSpeed(1), m_bDirChange(false)
{
    // Set initial direction
    m_iHorizontal = 0;
    m_iVertical = 0;  
    m_iDir = dir;
    switch(dir)
    {
        case DIR_UP:
            m_iVertical = -1;
            m_Flip = SDL_FLIP_NONE; 
            break;
        case DIR_DOWN:
            m_iVertical = 1;
            m_Flip = SDL_FLIP_HORIZONTAL; 
            break;
        case DIR_LEFT:
            m_iHorizontal = -1;
            m_Flip = SDL_FLIP_NONE;            
            break;
        case DIR_RIGHT:
            m_iHorizontal = 1;
            m_Flip = SDL_FLIP_HORIZONTAL;
            break;
        default:
            break;
    }
}

Enemy::~Enemy()
{}

void Enemy::Update()
{
    // Left or right move
    if(m_iDir == DIR_LEFT || m_iDir == DIR_RIGHT)
    {
        // Update x position
        m_Rect.x += m_iHorizontal * m_iSpeed;

        // Check x bounds
        if(m_Rect.x < 0)
        {
            m_Rect.x = 0;
            m_iDir = DIR_RIGHT;
            m_iHorizontal = 1;
            m_Flip = SDL_FLIP_HORIZONTAL;
        }
        else if (m_Rect.x > (DISPLAY_WIDTH - TILE_SIZE))
        {
            m_Rect.x = DISPLAY_WIDTH - TILE_SIZE;
            m_iDir = DIR_LEFT;
            m_iHorizontal = -1;
            m_Flip = SDL_FLIP_NONE;
        }
    }
    else // Up or down move
    {
        // Update y position
        m_Rect.y += m_iVertical * m_iSpeed;

        // Check y bounds
        if(m_Rect.y < 0)
        {
            m_Rect.y = 0;
            m_iDir = DIR_DOWN;
            m_iVertical = 1;
        }
        else if (m_Rect.y > (DISPLAY_HEIGHT - TILE_SIZE))
        {
            m_Rect.y = DISPLAY_HEIGHT - TILE_SIZE; 
            m_iDir = DIR_UP;    
            m_iVertical = -1;        
        } 
    } 
}

void Enemy::Move()
{
}

void Enemy::Maneuver()
{
    // Back off position
    if(m_iDir == DIR_LEFT || m_iDir == DIR_RIGHT)
    {
        m_Rect.x -= m_iHorizontal * m_iSpeed;
    }
    else // m_iDir == DIR_VERTICAL
    {
        m_Rect.y -= m_iVertical * m_iSpeed;
    }

    bool bHeads = (rand() % HEADS_OR_TAILS);    

    // Change direction
    if(m_iDir == DIR_LEFT || m_iDir == DIR_RIGHT)
    {
        if(m_iDir == DIR_LEFT)
        {
            m_iDir = DIR_RIGHT;
            m_iHorizontal = 1;
            m_Flip = SDL_FLIP_HORIZONTAL;
        }
        else // m_iDir == DIR_RIGHT
        {
            m_iDir = DIR_LEFT;
            m_iHorizontal = -1;   
            m_Flip = SDL_FLIP_NONE;         
        }
    }
    else // m_iDir == VERTICAL
    {
        if(m_iDir == DIR_UP)
        {
            m_iDir = DIR_DOWN;
            m_iVertical = 1;
        }
        else // m_iDir == DIR_DOWN
        {
            m_iDir = DIR_UP;
            m_iVertical = -1;           
        }     
    }
}

int Enemy::GetDir()
{
    return(m_iDir);
}

void Enemy::SetDir(int dir)
{
    m_iDir = dir;
    m_bDirChange = true;
}

