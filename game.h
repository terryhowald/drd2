#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "spritegroup.h"
#include <string>

enum 
{
    DISPLAY_WIDTH  = 800, 
    DISPLAY_HEIGHT = 640,
    TILE_SIZE = 32,
    SHOW_SG_SCREEN = 0,
    SHOW_GO_SCREEN = 1,
    FPS = 60,
    UPDATE_INTERVAL = 1000/60,
    PLAYER_SPEED = 2,
    MAP_WIDTH = 25,
    MAP_HEIGHT = 20,
    NUM_TUNNELS = 6,
    DIR_HORIZONTAL = 0,
    DIR_VERTICAL = 1,
    HEADS_OR_TAILS = 2,
    TUNNEL_SPEED = 3,
    ROCK_SPEED = 1,
    DIR_LEFT  = 0, 
    DIR_RIGHT = 1,
    DIR_UP = 2,
    DIR_DOWN = 3,
    TURN_ODDS = 4,
    MAX_SCREAMS = 8    
};  

class Game 
{

public:
    Game();
    ~Game();
    bool IsRunning();
    void ShowScreen(int Screen);
    void New();

    static SDL_Renderer *m_pRenderer;
    static SDL_Event m_Event;   
    static bool m_bRunning;

private:
    bool m_bPlaying;
    SDL_Window* m_pWindow;  
    SDL_GameController *m_pController;   
    int tilemap[MAP_WIDTH][MAP_HEIGHT];
    int m_iEnemyNum;
    bool m_bFiring;
    bool m_bFirePhaser;
    bool m_bCeaseFire;
    int m_iFireIndex;
    int m_iPhaserCh;
    int m_iScream;
    int m_iPlayerAlive;
    int m_iPhaserPower;
    int m_iEnemyDead;
    int m_iEggsSaved;
    int m_iTotalScore;
    std::string m_sBasePath;

    Mix_Chunk *m_pSquashWav;
    Mix_Chunk *m_pPhaserWav;    
    Mix_Chunk *m_pScreamWavs[MAX_SCREAMS];

    struct EnemyInit
    {
        int xpos;
        int ypos;
        int dir;
    };

    struct EnemyFire
    {
        int Enemy_Num;
        int e_xpos;
        int e_ypos;
        int p_xpos;
        int p_ypos;
    };

    EnemyInit* m_pEnemyInit;
    EnemyFire m_EnemyFire;

    void LoadMap();
    void Run();
    void Events();
    void Update();
    void Draw();    
    bool SpriteCollide(Sprite *pSprite, SpriteGroup *pSpritegroup, bool bRemove); 
    void LoadEggs();
    bool TurnCheck(Sprite *pSprite); 
    bool PhaserCheck(Sprite *pSprite);   
    void LoadWavs();
};


#endif // GAME_H