#ifndef __GAME_BOARD_H__
#define __GAME_BOARD_H__

#include "cocos2d.h"

extern float GAME_BOARD_TICK_IN_SECONDS;

class GameBoard 
{
public:
    GameBoard();

    void spawnNewPiece();

    void processBoard();

    char* getBoardRep();

    int getBoardWidthInBlocks();

    int getBoardHeightInBlocks();
    
    void resetBoard();

    int getExplosionBarPos();

private:
    char *board;

    int blankBlocks;

    int explosionBarPos;

    bool barInBigBlock;

    // big block = set of blocks that are about to explode!
    int bigBlockStartX;

    void printBoard();

    bool isPlaceValidForNewPiece(int x, int y);

    void setBoardToInitialState();
};

#endif 
