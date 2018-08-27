/* 
 DRD2
  
 Author: Terry Howald

 */

#include "game.h"

int main(int argc, char **argv)
{
  // Initialize game
  Game *pGame = new Game();

  if(pGame == nullptr)
  {
    return 0;
  }

  // Show start game screen
  pGame->ShowScreen(SHOW_SG_SCREEN);

  // Start game
  while (pGame->IsRunning())
  {
    // Start new game
    pGame->New();

    // Show game over screen
    pGame->ShowScreen(SHOW_GO_SCREEN);
  }

  // Free game point from heap
  delete pGame;

  return 0;
}