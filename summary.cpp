#include "summary.h"
#include "game.h"

Summary::Summary()
{}

Summary::~Summary()
{}

void Summary::Show(int iScreen)
{
    // Determine which screen to display    
    std::string strScreen;
    std::string strWave;
    if (SHOW_SG_SCREEN == iScreen) 
    {
        strScreen = "/home/terry/repos/drd2/img/sg.png";
        strWave = "/home/terry/repos/drd2/snd/tos_sickbay_venus_drug.wav";
    }
    else 
    {
        strScreen = "/home/terry/repos/drd2/img/go.png";
        strWave = "/home/terry/repos/drd2/snd/tos_sickbayscannerheartbeats.wav";        
    }  

    // Load image
    SDL_Texture *texture = IMG_LoadTexture(Game::m_pRenderer, strScreen.c_str());

    // Set offscreen to black  
    SDL_SetRenderDrawColor(Game::m_pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  
    SDL_RenderClear(Game::m_pRenderer);  

    // Copy image to offscreen 
    SDL_RenderCopy(Game::m_pRenderer, texture, NULL, NULL);   
           
    // Blit offscreen to display
    SDL_RenderPresent(Game::m_pRenderer);    

    // Load wav file
    Mix_Chunk *wave = Mix_LoadWAV(strWave.c_str());
    if(NULL != wave) 
    {
        int iChannel = Mix_PlayChannel(-1, wave, -1);
     
        Mix_Volume(iChannel, MIX_MAX_VOLUME/4);
    }

    // Watch for events to exit screen
    bool bQuit = false;
    while(true != bQuit) 
    {
        if (SDL_PollEvent(&Game::m_Event)) 
        {
            switch(Game::m_Event.type) 
            {
                case SDL_MOUSEBUTTONDOWN:
                case SDL_KEYDOWN:
                    bQuit = true;
                    break;
                case SDL_QUIT:
                    bQuit = true;
                    Game::m_bRunning = false;
                    break;
            }
        }  
    }

    // Free wav file
    if (NULL != wave)
        Mix_FreeChunk(wave);

    // Free image
    if (NULL != texture)
        SDL_DestroyTexture(texture);

   return;        
}