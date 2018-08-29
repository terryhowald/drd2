#include "player.h"
#include "game.h"

Player::Player(const char *fileName, int xpos, int ypos)
    : Sprite(fileName, xpos, ypos), m_iXSpeed(3), m_iYSpeed(3)
{
}

Player::~Player()
{
}

void Player::Update()
{
    // Update x position
    m_iXPos += m_iHorizontal * m_iXSpeed;

    // Check x bounds
    if(m_iXPos < 0)
        m_iXPos = 0;
    else if (m_iXPos > (DISPLAY_WIDTH - TILE_SIZE))
        m_iXPos = DISPLAY_WIDTH - TILE_SIZE;

    // Update y position
    m_iYPos += m_iVertical * m_iYSpeed;

    // Check y bounds
    if(m_iYPos < 0)
        m_iYPos = 0;
    else if (m_iYPos > (DISPLAY_HEIGHT - TILE_SIZE))
        m_iYPos = DISPLAY_HEIGHT - TILE_SIZE;    
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
        case SDLK_RIGHT:
            m_iHorizontal = 0;
            break;
        case SDLK_UP:            
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
    m_dAngle = dAngle;
}
