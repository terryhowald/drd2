#ifndef SUMMARY_H
#define SUMMARY_H

class Summary
{
public:
    Summary();
    ~Summary();
    
    void Show(int iScreen);
    void SetEnemyDead(int iVal);
    void SetEggsSaved(int iVal);
    void SetTotalScore(int iVal);

private:

    int m_iEnemyDead;
    int m_iEggsSaved;
    int m_iTotalScore;

};

#endif // SUMMARY_H