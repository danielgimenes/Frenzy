#ifndef __GAME_BOARD_H__
#define __GAME_BOARD_H__

#include "cocos2d.h"

extern float GAME_BOARD_TICK_IN_SECONDS;

class GameBoard 
{
public:
    GameBoard();

    void spawnNewBlock();

    void processBoard();

    char* getBoardRep();

    int getBoardWidthInSquares();

    int getBoardHeightInSquares();
    
    void resetBoard();

    int getExplosionBarPos();

private:
    char *board;

    int blankSquares;

    int explosionBarPos;

    bool barInBigBlock;

    int bigBlockStartX;

    void printBoard();

    bool isPlaceValidForNewBlock(int x, int y);

    void setBoardToInitialState();
};

#endif 
