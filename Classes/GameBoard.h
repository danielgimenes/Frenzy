#ifndef __GAME_BOARD_H__
#define __GAME_BOARD_H__

#include "cocos2d.h"

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

private:
    char *board;

    int blankSquares;

    void printBoard();

    bool isPlaceValidForNewBlock(int x, int y);
};

#endif 
