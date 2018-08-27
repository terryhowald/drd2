#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

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
    MAP_HEIGHT = 20
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

    void LoadMap();
    void Run();
    void Events();
    void Update();
    void Draw();         
};


#endif // GAME_H