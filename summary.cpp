#include "summary.h"
#include "game.h"

Summary::Summary() : m_pFont(nullptr)
{
    m_pFont = TTF_OpenFont("/home/terry/repos/drd2/ttf/TOS_Title.ttf", 64);
}

Summary::~Summary()
{
    if(nullptr != m_pFont)
    {
        TTF_CloseFont(m_pFont);
    }
}

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

    // Display text if GO screen
    if(SHOW_GO_SCREEN == iScreen)
    {
        SDL_Rect rect;

        // Display enemy dead count
        rect.x = 580;
        rect.y = 238;
        std::string sEnemyDead = std::to_string(m_iEnemyDead);
        DisplayText(sEnemyDead, rect);

        // Display eggs saved count
        rect.x = 580;
        rect.y = 344;
        std::string sEggsSaved = std::to_string(m_iEggsSaved);
        DisplayText(sEggsSaved, rect);    

        // Display total score
        rect.x = 580;
        rect.y = 450;
        std::string sTotalScore = std::to_string(m_iTotalScore);
        DisplayText(sTotalScore, rect);              
    }
           
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
                    bQuit = true;
                    break;                
                case SDL_KEYDOWN:
                    if(SDLK_RETURN == Game::m_Event.key.keysym.sym)
                    {              
                        bQuit = true;
                    }
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

void Summary::SetEnemyDead(int iVal)
{
    m_iEnemyDead = iVal;
}

void Summary::SetEggsSaved(int iVal)
{
    m_iEggsSaved = iVal;
}

void Summary::SetTotalScore(int iVal)
{
    m_iTotalScore = iVal;
}

void Summary::DisplayText(std::string text, SDL_Rect rect)
{
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surf = TTF_RenderText_Blended(m_pFont, text.c_str(), white);
    SDL_Texture* pLabelTexture = SDL_CreateTextureFromSurface(Game::m_pRenderer, surf);
    SDL_FreeSurface(surf);

    SDL_QueryTexture(pLabelTexture, nullptr, nullptr, &rect.w, &rect.h);
    SDL_RenderCopy(Game::m_pRenderer, pLabelTexture, nullptr, &rect);
}
