#include "player.h"
#include "game.h"

Player::Player(const char *fileName, int xpos, int ypos)
    : Sprite(fileName, xpos, ypos), m_iXSpeed(3), m_iYSpeed(3), 
    m_iHorizontal(0), m_iVertical(0), m_iOrientation(DIR_VERTICAL), m_iCount(0)
{
}

Player::~Player()
{
}

void Player::Update()
{
    // Update x position
    m_Rect.x += m_iHorizontal * m_iXSpeed;

    // Check x bounds
    if(m_Rect.x < 0)
        m_Rect.x = 0;
    else if (m_Rect.x > (DISPLAY_WIDTH - TILE_SIZE))
        m_Rect.x = DISPLAY_WIDTH - TILE_SIZE;

    // Update y position
    m_Rect.y += m_iVertical * m_iYSpeed;

    // Check y bounds
    if(m_Rect.y < 0)
        m_Rect.y = 0;
    else if (m_Rect.y > (DISPLAY_HEIGHT - TILE_SIZE))
        m_Rect.y = DISPLAY_HEIGHT - TILE_SIZE;  

    // Check slow speed countdown
    if(m_iCount--)
    {
        // If count has expires, set normal speed
        if(!m_iCount)
        {
            Set_Speed(3);
        }
    }  
}

void Player::Move()
{
    // Process key event
    switch (Game::m_Event.type)
    {
    case SDL_KEYDOWN:
        switch (Game::m_Event.key.keysym.sym)
        {
        case SDLK_UP:
            m_iVertical = -1;         
            Set_Angle(0.0);
            break;
        case SDLK_LEFT:
            m_iHorizontal = -1;          
            Set_Angle(270.0);           
            break;
        case SDLK_RIGHT:
            m_iHorizontal = 1;            
            Set_Angle(90.0);           
            break;
        case SDLK_DOWN:
            m_iVertical = 1;           
            Set_Angle(180.0);           
            break;
        default:
            break;
        }
        break;
    case SDL_KEYUP:
        switch (Game::m_Event.key.keysym.sym)
        {
        case SDLK_LEFT:
            m_iHorizontal = 0;
            break;
        case SDLK_RIGHT:       
            m_iHorizontal = 0;
            break;
        case SDLK_UP:  
            m_iVertical = 0;
            break;                
        case SDLK_DOWN:         
            m_iVertical = 0;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void Player::Set_Angle(double dAngle)
{
    // Set player angle
    m_dAngle = dAngle;

    // Determine orientation
    int iOrientation = DIR_HORIZONTAL;
    if(m_dAngle == 0.0 || m_dAngle == 180.0)
    {
        iOrientation = DIR_VERTICAL;
    }
    else if(m_dAngle == 90.0 || m_dAngle == 270.0)
    {
        iOrientation = DIR_HORIZONTAL;
    }

    // Check for orientation change
    if(m_iOrientation != iOrientation)
    {
        m_iOrientation = iOrientation;

        // Adjust player position to align with rock tiles
        m_Rect.x = ((int)round((double)m_Rect.x/(double)TILE_SIZE))*TILE_SIZE;      
        m_Rect.y = ((int)round((double)m_Rect.y/(double)TILE_SIZE))*TILE_SIZE;      
    }
}

void Player::Set_Speed(int iSpeed)
{
    m_iXSpeed = iSpeed;
    m_iYSpeed = iSpeed;

    // If player speed is set slow, start reset countdown
    if(iSpeed == 1)
    {
        m_iCount = 30;
    }
}
