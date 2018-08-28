#include "game.h"
#include "summary.h"
#include "player.h"
#include "enemy.h"
#include "tile.h"
#include "egg.h"
#include "spritegroup.h"
#include <cstdlib>
#include <ctime>

SDL_Renderer *Game::m_pRenderer = nullptr;
SDL_Event Game::m_Event;
bool Game::m_bRunning = false;

Player* player = nullptr;
Enemy* enemy = nullptr;
Egg* egg = nullptr;

SpriteGroup* tilegroup = nullptr;

Game::Game()
    : m_bPlaying(false), m_pWindow(NULL), m_pController(NULL)
{
    // Initialize SDL2 library
    if (0 == SDL_Init(SDL_INIT_EVERYTHING))
    {
        // Create a window and default renderer
        int flags = SDL_WINDOW_SHOWN;
        SDL_CreateWindowAndRenderer(DISPLAY_WIDTH, DISPLAY_HEIGHT, flags, &m_pWindow, &m_pRenderer);

        // Set window title
        if (NULL != m_pWindow)
        {
            SDL_SetWindowTitle(m_pWindow, "Die, Redshirt, Die!");
        }

        // Initialize SDL2 Image library
        IMG_Init(IMG_INIT_PNG);

        // Initialize SDL2 Mixer library
        Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096);

        // Initialize SDL2 game controller
        for (int i = 0; i < SDL_NumJoysticks(); ++i)
        {
            if (SDL_IsGameController(i))
            {
                m_pController = SDL_GameControllerOpen(i);
                break;
            }
        }

        // Set initial running flag
        m_bRunning = true;

        // Seed random number generator
        srand((unsigned)time(0)); 
    }
}

Game::~Game()
{
    // Destory enemy
    if(nullptr != enemy)
    {
        delete enemy;
        enemy = nullptr;
    }

    // Destory player
    if(nullptr != player)
    {
        delete player;
        player = nullptr;
    }

    // Destory renderer
    if (NULL != m_pRenderer)
    {
        SDL_DestroyRenderer(m_pRenderer);
        m_pRenderer = NULL;
    }

    // Destory window
    if (NULL != m_pWindow)
    {
        SDL_DestroyWindow(m_pWindow);
        m_pWindow = NULL;
    }

    // Shutdown SDL2 Image
    IMG_Quit();

    // Shutdown SDL2 Mixer
    Mix_CloseAudio();

    // Shutdown SDL2 game controller
    if (NULL != m_pController)
    {
        SDL_GameControllerClose(m_pController);
        m_pController = NULL;
    }

    // Shutdown SDL2
    SDL_Quit();
}

void Game::ShowScreen(int iScreen)
{
    // Check for quit
    if (false == m_bRunning)
        return;

    // Display summary screen
    Summary *pSummary = new Summary();
    if (pSummary != nullptr)
    {
        pSummary->Show(iScreen);
        delete pSummary;
    }
}

void Game::New()
{
    // Setup background sound
    Mix_Music *music = Mix_LoadMUS("/home/terry/repos/drd2/snd/tos_planet_3.wav");
    if (NULL != music)
    {
        Mix_PlayMusic(music, -1);
        Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
    }

    // Setup tile map
    tilegroup = new SpriteGroup();
    LoadMap();

    // Create player
    player = new Player("/home/terry/repos/drd2/img/horta.png", 5, 5);
 
    // Let 'er rip!
    Run();

    // Free music
    if (nullptr != music)
    {
        Mix_FreeMusic(music);
        music = nullptr;
    }
}

void Game::Run()
{
    // Game loop
    int iTicks = 0;
    int iMaxTicks = 1000 / FPS;
    m_bPlaying = true;
    while (true == m_bPlaying)
    {
        // Get current tick count
        iTicks = SDL_GetTicks();

        // Process events
        Events();

        // Update game parameters
        Update();

        // Draw new frame
        Draw();

        // Delay if necessary for next frame
        iTicks = SDL_GetTicks() - iTicks;
        if (iTicks < iMaxTicks)
            SDL_Delay(iMaxTicks - iTicks);
    }
}

void Game::Events()
{
    // Process event queue
    while (SDL_PollEvent(&m_Event))
    {
        switch (m_Event.type)
        {
        case SDL_MOUSEBUTTONDOWN:  
            m_bPlaying = false; 
            break;            
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            //player->Move();
            break;         
        case SDL_QUIT:
        case SDL_CONTROLLERBUTTONDOWN:
            m_bPlaying = false;
            m_bRunning = false;
            break;
        default:
            break;
        }
    }
}

void Game::Update()
{
    player->Update();
}

void Game::Draw()
{
    // Clear offscreen to black
    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_pRenderer);

    // Draw map
    tilegroup->Draw();

    // Draw player
    player->Draw();

    // Blit offscreen to display
    SDL_RenderPresent(m_pRenderer);
}

bool Game::IsRunning()
{
    return m_bRunning;
}

void Game::LoadMap()
{
    // Initialize tilemap
    for(int row = 0; row < MAP_HEIGHT; row++)
    {
        for(int column = 0; column < MAP_WIDTH; column++)
        {
            tilemap[row][column] = 1; 
        }
    }  

    // Generate tunnels
    for(int tunn = 0; tunn < NUM_TUNNELS; tunn++)
    {
        // Compute random x and y starting points
        int iXPos = 2*(rand()%(MAP_HEIGHT/2)); 
        int iYPos = 2*(rand()%(MAP_WIDTH/2));
        int iDir = (rand()%HEADS_OR_TAILS);
        //std::cout << iXPos << "," << iYPos << "," << iDir << std::endl;

        // Determine which direction to draw the tunnel
        if(DIR_HORIZONTAL == iDir)
        {
            // Create horizontal tunnel
            int row = iXPos;        
            if(iYPos > (MAP_WIDTH)/2)
            {
                for(int column = 0; column < iYPos; column++)
                {
                    tilemap[row][column] = 0;
                }
            }
            else
            {
                for(int column = iYPos; column < MAP_WIDTH; column++)
                {
                    tilemap[row][column] = 0;
                }
            }
        }
        else // DIR_VERTICAL == iDir
        {
            // Create vertical tunnel
            int column = iYPos;
            if(iXPos > (MAP_HEIGHT)/2)
            {
                for(int row = 0; row < iXPos; row++)
                {
                    tilemap[row][column] = 0;
                }
            }
            else
            {
                for(int row = iXPos; row < MAP_HEIGHT; row++)
                {
                    tilemap[row][column] = 0;
                }
            }
        }
    }

    // Setup tiles
    for(int row = 0; row < MAP_HEIGHT; row++)
    {
        for(int column = 0; column < MAP_WIDTH; column++)
        {
            int iTile = tilemap[row][column];
            if(iTile)
            {
                Tile *tile = new Tile("/home/terry/repos/drd2/img/rock_tile.png", column, row); 
                tilegroup->Add(tile);
            }
        }
    }    


}
