#include "player.h"
#include "game.h"

Player::Player(const char *fileName, int xpos, int ypos)
    : Sprite(fileName, xpos, ypos), m_iXSpeed(0), m_iYSpeed(0)
{
}

Player::~Player()
{
}

void Player::Update()
{
    m_iXPos += m_iXSpeed;
    m_iYPos += m_iYSpeed;
}

void Player::Move()
{
    switch (Game::m_Event.type)
    {
    case SDL_KEYDOWN:
        switch (Game::m_Event.key.keysym.sym)
        {
        case SDLK_UP:
            m_iYSpeed = -TUNNEL_SPEED;
            Set_Angle(0.0);
            break;
        case SDLK_LEFT:
            m_iXSpeed = -TUNNEL_SPEED;
            Set_Angle(270.0);
            break;
        case SDLK_RIGHT:
            m_iXSpeed = TUNNEL_SPEED;
            Set_Angle(90.0);
            break;
        case SDLK_DOWN:
            m_iYSpeed = TUNNEL_SPEED;
            Set_Angle(180.0);
            break;
        default:
            break;
        }
        break;
    case SDL_KEYUP:
        switch (Game::m_Event.key.keysym.sym)
        {
        case SDLK_UP:
            if (m_iYSpeed < 0)
                m_iYSpeed = 0;
            break;
        case SDLK_LEFT:
            if (m_iXSpeed < 0)
                m_iXSpeed = 0;
            break;
        case SDLK_RIGHT:
            if (m_iXSpeed > 0)
                m_iXSpeed = 0;
            break;
        case SDLK_DOWN:
            if (m_iYSpeed > 0)
                m_iYSpeed = 0;
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
