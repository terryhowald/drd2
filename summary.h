#ifndef SUMMARY_H
#define SUMMARY_H

#include "SDL2/SDL_ttf.h"
#include <string>

class Summary
{
public:
    Summary();
    ~Summary();
    
    void Show(int iScreen);
    void SetEnemyDead(int iVal);
    void SetEggsSaved(int iVal);
    void SetTotalScore(int iVal);
    void DisplayText(std::string text, SDL_Rect rect);    

private:

    int m_iEnemyDead;
    int m_iEggsSaved;
    int m_iTotalScore;
    TTF_Font* m_pFont;

};

#endif // SUMMARY_H