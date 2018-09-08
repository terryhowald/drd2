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

Player *player = nullptr;
Enemy *enemy = nullptr;
Egg *egg = nullptr;

SpriteGroup *pTileGroup = nullptr;
SpriteGroup *pEnemyGroup = nullptr;
SpriteGroup *pEggGroup = nullptr;

Game::Game()
    : m_bPlaying(false), m_pWindow(NULL), m_pController(NULL), m_iEnemyNum(0),
      m_pEnemyInit(nullptr), m_bFiring(false), m_bCeaseFire(false), m_bFirePhaser(false),
      m_iPhaserPower(0), m_iEnemyDead(0), m_iEggsSaved(0), m_iTotalScore(0)
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

        // Init enemy init array
        m_pEnemyInit = new EnemyInit[NUM_TUNNELS];

        // Load wave files
        LoadWavs();

        m_iScream = 0;
    }
}

Game::~Game()
{
    if (nullptr != m_pEnemyInit)
    {
        delete[] m_pEnemyInit;
        m_pEnemyInit = nullptr;
    }

    // Destory egggroup
    if (nullptr != pEggGroup)
    {
        pEggGroup->Empty();
        delete pEggGroup;
        pEggGroup = nullptr;
    }

    // Destory pTileGroup
    if (nullptr != pTileGroup)
    {
        pTileGroup->Empty();
        delete pTileGroup;
        pTileGroup = nullptr;
    }

    // Destory enemy
    if (nullptr != enemy)
    {
        delete enemy;
        enemy = nullptr;
    }

    // Destory player
    if (nullptr != player)
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

    // Accumlate data
    if(iScreen != SHOW_SG_SCREEN)
    {
        pSummary->SetEnemyDead(m_iEnemyDead);
        pSummary->SetEggsSaved(m_iEggsSaved);
        m_iTotalScore += (m_iEnemyDead*10 + m_iEggsSaved);
        pSummary->SetTotalScore(m_iTotalScore);
    }

    // Display summary
    pSummary->Show(iScreen);

    // Free when finished
    delete pSummary;
}

void Game::New()
{
    // Seed random number generator
    std::srand(std::time(0));

    // Reset score fields
    m_iEnemyDead = 0;
    m_iEggsSaved = 0;

    // Setup background sound
    Mix_Music *music = Mix_LoadMUS("/home/terry/repos/drd2/snd/tos_planet_3.wav");
    if (NULL != music)
    {
        Mix_PlayMusic(music, -1);
        Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
    }

    // Setup tile group
    pTileGroup = new SpriteGroup();
    LoadMap();

    // Create player
    int iRandX = (rand() % MAP_WIDTH) * TILE_SIZE;
    int iRandY = (rand() % MAP_HEIGHT) * TILE_SIZE;
    player = new Player("/home/terry/repos/drd2/img/horta.png", iRandX, iRandY);
    m_iPlayerAlive = 255;
    m_iPhaserPower++;

    // Create enemy group
    pEnemyGroup = new SpriteGroup();

    // Create enemy(s)
    m_iEnemyNum++;
    for (int i = 0; i < m_iEnemyNum; i++)
    {
        // Get tunnel to place enemy
        int iTunn = i % NUM_TUNNELS;

        // Setup enemy
        int xpos = m_pEnemyInit[iTunn].xpos;
        int ypos = m_pEnemyInit[iTunn].ypos;
        int dir = m_pEnemyInit[iTunn].dir;
        if (dir == DIR_LEFT)
            xpos -= (50 + std::rand() % DISPLAY_WIDTH / 2);
        if (dir == DIR_RIGHT)
            xpos += (50 + std::rand() % DISPLAY_WIDTH / 2);
        if (dir == DIR_UP)
            ypos -= (50 + std::rand() % DISPLAY_HEIGHT / 2);
        if (dir == DIR_DOWN)
            ypos += (50 + std::rand() % DISPLAY_HEIGHT / 2);

        // Create enemy
        enemy = new Enemy("/home/terry/repos/drd2/img/redshirt.png", xpos, ypos, dir);
        pEnemyGroup->Add(enemy);
    }

    // Create egg group
    pEggGroup = new SpriteGroup();

    // Create eggs
    LoadEggs();

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
            player->Move();
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
    // Update player
    player->Update();

    // Update enemy(s)
    pEnemyGroup->Update();

    // Check for player/tile collisions
    if (SpriteCollide(player, pTileGroup, true))
    {
        // Player his something, so slow down
        player->Set_Speed(1);

        // Zero out collision location in tilemap
        SDL_Rect tRect = player->GetRect();
        int iRow = tRect.x / TILE_SIZE;
        int iCol = tRect.y / TILE_SIZE;
        tilemap[iCol][iRow] = 0;
    }

    // Check for player/enemy collisions
    if (SpriteCollide(player, pEnemyGroup, true))
    {
        // Make enemy dying sound
        if(nullptr != m_pScreamWavs) 
        {
            int iScream = 0;
            do
            {
                iScream = rand()%8;
            } while(iScream == m_iScream);
            m_iScream = iScream;

            Mix_PlayChannel(3, m_pScreamWavs[iScream], 0);
            Mix_Volume(3, MIX_MAX_VOLUME/4);
        }

        // Update enemy dead count
        m_iEnemyDead++;

        // Check enemy count
        if (pEnemyGroup->Size() == 0)
        {
            m_bPlaying = false;
            Mix_HaltChannel(1);
        }
    }

    // Get vector of sprites to loop through for collisions
    std::vector<Sprite *> sprites = pEnemyGroup->Get_Sprites();

    // Check for enemy/tile collisions
    for (int i = 0; i < sprites.size(); i++)
    {
        if (SpriteCollide(sprites[i], pTileGroup, false))
        {
            // Ran into rock, so turn back
            static_cast<Enemy *>(sprites[i])->Maneuver();
        }
    }

    // Check for enemy/egg collisions
    for (int i = 0; i < sprites.size(); i++)
    {
        if (SpriteCollide(sprites[i], pEggGroup, true))
        {
            // Make egg squashing sound
            if(nullptr != m_pSquashWav) 
            {
                int iChannel = Mix_PlayChannel(2, m_pSquashWav, 0);
                Mix_Volume(2, MIX_MAX_VOLUME/4);
            }

            // Check egg count
            if (pEggGroup->Size() == 0)
            {
                // Game over
                m_bPlaying = false;
                Mix_HaltChannel(1);
            }
        }
    }

    // See if enemy can turn
    for (int i = 0; i < sprites.size(); i++)
    {
        if (TurnCheck(sprites[i]))
        {
            // Make a sound when redshirt turns
            ;
        }
    }

    // See if enemy can fire
    m_bFirePhaser = false;    
    for (int i = 0; i < sprites.size(); i++)
    {
        PhaserCheck(sprites[i]);

        if(m_bFirePhaser)
        { 
            // Make a sound when redshirt fires
            if(nullptr != m_pPhaserWav) 
            {
                m_iPhaserCh = Mix_PlayChannel(1, m_pPhaserWav, -1);
                Mix_Volume(m_iPhaserCh, MIX_MAX_VOLUME/4);
            }     
        }
        else // Shut off phaser firing sound
        {
            Mix_HaltChannel(m_iPhaserCh);
        }
    }
}

void Game::Draw()
{
    // Clear offscreen to black
    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_pRenderer);      

    // Draw map
    pTileGroup->Draw();

    // Draw eggs
    pEggGroup->Draw();

      // Draw phaser, if firing
    if (m_bFirePhaser)
    {
        // Check if player is dead
        m_iPlayerAlive--; // -= m_iPhaserPower;
        player->SetAlpha(m_iPlayerAlive);        
        if(m_iPlayerAlive <= (m_iPhaserPower-1))
        {
            m_bPlaying = false;
            Mix_HaltChannel(m_iPhaserCh);         
        }   
        else
        {
            // Draw phaser line
            SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(m_pRenderer, m_EnemyFire.e_xpos, m_EnemyFire.e_ypos, m_EnemyFire.p_xpos, m_EnemyFire.p_ypos);   
        }
    } 

    // Draw enemy(s);
    pEnemyGroup->Draw();    

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
    for (int row = 0; row < MAP_WIDTH; row++)
    {
        for (int column = 0; column < MAP_HEIGHT; column++)
        {
            tilemap[row][column] = 1;
        }
    }

    // Generate tunnels
    for (int tunn = 0; tunn < NUM_TUNNELS; tunn++)
    {
        // Compute random x and y starting points
        int iXPos = 2 * (std::rand() % (MAP_WIDTH / 2));
        int iYPos = 2 * (std::rand() % (MAP_HEIGHT / 2));
        int iDir = (std::rand() % HEADS_OR_TAILS);
        //std::cout << iXPos << "," << iYPos << "," << iDir << std::endl;

        // Determine which direction to draw the tunnel
        if (DIR_VERTICAL == iDir)
        {
            // Create vertical tunnel
            int row = iXPos;
            if (iYPos > (MAP_HEIGHT) / 2)
            {
                for (int column = 0; column < iYPos; column++)
                {
                    tilemap[row][column] = 0;
                }
                m_pEnemyInit[tunn].xpos = iXPos * TILE_SIZE;
                m_pEnemyInit[tunn].ypos = iYPos * TILE_SIZE;
                m_pEnemyInit[tunn].dir = DIR_UP;
            }
            else
            {
                for (int column = iYPos; column < MAP_HEIGHT; column++)
                {
                    tilemap[row][column] = 0;
                }
                m_pEnemyInit[tunn].xpos = iXPos * TILE_SIZE;
                m_pEnemyInit[tunn].ypos = iYPos * TILE_SIZE;
                m_pEnemyInit[tunn].dir = DIR_DOWN;
            }
        }
        else // DIR_HORIZONTAL == iDir
        {
            // Create horizontal tunnel
            int column = iYPos;
            if (iXPos > (MAP_WIDTH) / 2)
            {
                for (int row = 0; row < iXPos; row++)
                {
                    tilemap[row][column] = 0;
                }
                m_pEnemyInit[tunn].xpos = iXPos * TILE_SIZE;
                m_pEnemyInit[tunn].ypos = iYPos * TILE_SIZE;
                m_pEnemyInit[tunn].dir = DIR_LEFT;
            }
            else
            {
                for (int row = iXPos; row < MAP_WIDTH; row++)
                {
                    tilemap[row][column] = 0;
                }
                m_pEnemyInit[tunn].xpos = iXPos * TILE_SIZE;
                m_pEnemyInit[tunn].ypos = iYPos * TILE_SIZE;
                m_pEnemyInit[tunn].dir = DIR_RIGHT;
            }
        }
    }

    // Setup tiles
    for (int row = 0; row < MAP_WIDTH; row++)
    {
        for (int column = 0; column < MAP_HEIGHT; column++)
        {
            int iTile = tilemap[row][column];
            if (iTile)
            {
                Tile *tile = new Tile("/home/terry/repos/drd2/img/rock_tile.png", row * TILE_SIZE, column * TILE_SIZE);
                pTileGroup->Add(tile);
            }
        }
    }
}

void Game::LoadEggs()
{
    // Setup tiles
    for (int row = 0; row < MAP_WIDTH; row++)
    {
        for (int column = 0; column < MAP_HEIGHT; column++)
        {
            int iTile = tilemap[row][column];
            if (!iTile)
            {
                Egg *pEgg = new Egg("/home/terry/repos/drd2/img/egg.png", row * TILE_SIZE, column * TILE_SIZE);
                pEggGroup->Add(pEgg);
            }
        }
    }
}

bool Game::SpriteCollide(Sprite *pSprite, SpriteGroup *pSpriteGroup, bool bRemove)
{
    bool bCollide = false;

    // Check for valid pointers
    if ((nullptr == pSprite) || (nullptr == pSpriteGroup))
    {
        std::cerr << "Invalid pointers" << std::endl;
        return (bCollide);
    }

    // Get vector from sprite group
    std::vector<Sprite *> sprites = pSpriteGroup->Get_Sprites();

    // Loop through sprites and check for collision
    for (int i = 0; i < sprites.size(); i++)
    {
        // Get sprite coordinates
        SDL_Rect recA = sprites[i]->GetRect();
        SDL_Rect recB = pSprite->GetRect();

        // Check for collision
        if (SDL_TRUE == SDL_HasIntersection(&recA, &recB))
        {
            // Check to remove
            if (bRemove)
            {
                pSpriteGroup->Remove(*sprites[i]);
            }

            // Hit something, so set collide flag
            bCollide = true;
        }
    }

    return (bCollide);
}

bool Game::TurnCheck(Sprite *pSprite)
{
    int x_pos = pSprite->GetRect().x;
    int y_pos = pSprite->GetRect().y;
    int x_index = (int)round((double)x_pos / (double)TILE_SIZE);
    int y_index = (int)round((double)y_pos / (double)TILE_SIZE);
    int iDir = static_cast<Enemy *>(pSprite)->GetDir();
    int iDice = 0;

    if (iDir == DIR_UP || iDir == DIR_DOWN)
    {
        // Look to make right or left turns
        if (x_index > 0 && x_index < MAP_WIDTH)
        {
            // Look right
            if (tilemap[x_index + 1][y_index] == 0)
            {
                if (y_pos == y_index * TILE_SIZE)
                {
                    // Roll dice to see if we turn right
                    if (!(rand() % TURN_ODDS))
                    {
                        static_cast<Enemy *>(pSprite)->SetDir(DIR_RIGHT);
                        return (true);
                    }
                }
            }

            // Look left
            if (tilemap[x_index - 1][y_index] == 0)
            {
                if (y_pos == y_index * TILE_SIZE)
                {
                    // Roll dice to see if we turn left
                    if (!(rand() % TURN_ODDS))
                    {
                        static_cast<Enemy *>(pSprite)->SetDir(DIR_LEFT);
                        return (true);
                    }
                }
            }
        }
    }
    else // iDir is horizontal
    {
        // Look to make up or down turns
        if (y_index > 0 && y_index < MAP_HEIGHT)
        {
            // Look up
            if (tilemap[x_index][y_index - 1] == 0)
            {
                if (x_pos == x_index * TILE_SIZE)
                {
                    // Roll dice to see if we turn up
                    if (!(rand() % TURN_ODDS))
                    {
                        static_cast<Enemy *>(pSprite)->SetDir(DIR_UP);
                        return (true);
                    }
                }
            }

            // Look down
            if (tilemap[x_index][y_index + 1] == 0)
            {
                if (x_pos == x_index * TILE_SIZE)
                {
                    // Roll dice to see if we turn down
                    if (!(rand() % TURN_ODDS))
                    {
                        static_cast<Enemy *>(pSprite)->SetDir(DIR_DOWN);
                        return (true);
                    }
                }
            }
        }
    }

    return (false);
}

bool Game::PhaserCheck(Sprite *pSprite)
{
    // Get player and enemy positions
    SDL_Rect pRect = player->GetRect();
    SDL_Rect eRect = pSprite->GetRect();
    int dist = eRect.x - pRect.x;
    int dir = static_cast<Enemy *>(pSprite)->GetDir();

    // See if they're on the same horzontal line
    if (eRect.y / TILE_SIZE == pRect.y / TILE_SIZE)
    {
        // Is enemy pointing toward player?
        if (dist > 0)
        {
            if (dir == DIR_LEFT)
            {
                //std::cout << "Enemy facing player to the left" << std::endl;
                ;
            }
            else
            {
                return (false);
            }
        }
        else // dist < 0
        {
            if (dir == DIR_RIGHT)
            {
                //std::cout << "Enemy facing player to the right" << std::endl;
                ;
            }
            else
            {
                return (false);
            }
        }

        // See if there's a clear line of fire
        int ex_index = eRect.x / TILE_SIZE;
        int ey_index = eRect.y / TILE_SIZE;
        int px_index = pRect.x / TILE_SIZE;
        int py_index = pRect.y / TILE_SIZE;

        if (dir == DIR_LEFT)
        {
            // Check for rock tiles in line of sight
            for (int i = px_index; i < ex_index; i++)
            {
                if (tilemap[i][ey_index] == 1)
                {
                    return (false);
                }
            }
        }
        else
        {
            for (int i = ex_index; i < px_index; i++)
            {
                if (tilemap[i][ey_index] == 1)
                {
                    return (false);
                }
            }
        }

        // Clear line, so store firing coordinates
        m_EnemyFire.Enemy_Num = 1;
        m_EnemyFire.e_xpos = eRect.x;
        m_EnemyFire.e_ypos = eRect.y + TILE_SIZE/2;
        m_EnemyFire.p_xpos = pRect.x + TILE_SIZE/2;
        m_EnemyFire.p_ypos = eRect.y + TILE_SIZE/2;

        m_bFirePhaser = true;

        return (true);
    }

    return (false);
}

void Game::LoadWavs()
{
    // Load squash sound
    m_pSquashWav = Mix_LoadWAV("/home/terry/repos/drd2/snd/squash.wav");  

    // Load squash sound
    m_pPhaserWav = Mix_LoadWAV("/home/terry/repos/drd2/snd/tos_phaser_7.wav"); 
    
    // Load scream sounds
    m_pScreamWavs[0] = Mix_LoadWAV("/home/terry/repos/drd2/snd/man_die_1.wav"); 
    m_pScreamWavs[1] = Mix_LoadWAV("/home/terry/repos/drd2/snd/man_die_2.wav"); 
    m_pScreamWavs[2] = Mix_LoadWAV("/home/terry/repos/drd2/snd/man_die_3.wav"); 
    m_pScreamWavs[3] = Mix_LoadWAV("/home/terry/repos/drd2/snd/man_die_4.wav");   
    m_pScreamWavs[4] = Mix_LoadWAV("/home/terry/repos/drd2/snd/man_die_5.wav"); 
    m_pScreamWavs[5] = Mix_LoadWAV("/home/terry/repos/drd2/snd/man_die_6.wav"); 
    m_pScreamWavs[6] = Mix_LoadWAV("/home/terry/repos/drd2/snd/man_die_7.wav"); 
    m_pScreamWavs[7] = Mix_LoadWAV("/home/terry/repos/drd2/snd/wilhelm_scream.wav");         
}

