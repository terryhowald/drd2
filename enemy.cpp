#include "enemy.h"
#include "game.h"
#include <cstdlib>

Enemy::Enemy(const char* fileName, int xpos, int ypos)
    : Sprite(fileName, xpos, ypos), m_iSpeed(1), m_iDir(DIR_HORIZONTAL)
{
    m_iHorizontal = 1;
    m_iVertical = 1;

    // Set initial direction
    m_iDir = (rand() % HEADS_OR_TAILS);
}

Enemy::~Enemy()
{}

void Enemy::Update()
{
    if(m_iDir == DIR_HORIZONTAL)
    {
        // Update x position
        m_Rect.x += m_iHorizontal * m_iSpeed;

        // Check x bounds
        if(m_Rect.x < 0)
        {
            m_Rect.x = 0;
            m_iHorizontal = 1;
        }
        else if (m_Rect.x > (DISPLAY_WIDTH - TILE_SIZE))
        {
            m_Rect.x = DISPLAY_WIDTH - TILE_SIZE;
            m_iHorizontal = -1;
        }
    }
    else // m_Dir == DIR_VERTICAL
    {
        // Update y position
        m_Rect.y += m_iVertical * m_iSpeed;

        // Check y bounds
        if(m_Rect.y < 0)
        {
            m_Rect.y = 0;
            m_iVertical = 1;
        }
        else if (m_Rect.y > (DISPLAY_HEIGHT - TILE_SIZE))
        {
            m_Rect.y = DISPLAY_HEIGHT - TILE_SIZE; 
            m_iVertical = -1;
        } 
    }  

}

void Enemy::Move()
{
    // Back off position
    if(m_iDir == DIR_HORIZONTAL)
    {
        m_Rect.x -= m_iHorizontal * m_iSpeed;
    }
    else // m_iDir == DIR_VERTICAL
    {
        m_Rect.y -= m_iVertical * m_iSpeed;
    }

    bool bHeads = (rand() % HEADS_OR_TAILS);    

    // Change direction
    if(m_iDir == DIR_HORIZONTAL)
    {
        m_iDir = DIR_VERTICAL;
        m_iHorizontal = 0;
        if(bHeads)
            m_iVertical = -1;
        else
            m_iVertical = 1;
    }
    else // m_iDir == VERTICAL
    {
        m_iDir = DIR_HORIZONTAL;
        m_iVertical = 0;
        if(bHeads)
            m_iHorizontal = -1;
        else
            m_iHorizontal = 1;        
    }

}

int Enemy::GetDir()
{
    return(m_iDir);
}
