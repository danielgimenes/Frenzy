#ifndef __GAME_BOARD_DRAWER_H__
#define __GAME_BOARD_DRAWER_H__

#include "cocos2d.h"
#include "GameBoard.h"

class GameBoardDrawer
{
public:
    GameBoardDrawer(GameBoard *board, cocos2d::Node *node, cocos2d::Vec2 origin, cocos2d::Size visibleSize);

    void drawGameBoard();

private:
    GameBoard *board;
    cocos2d::Node *boardRootNode;

    int horizontalCenter;
    int verticalCenter;

    int horizontalLineSize;
    int verticalLineSize;

    float leftPosBoard; 
    float rightPosBoard; 

    float topPosBoard;
    float bottomPosBoard;

    void drawBoardGrid();

    void drawBlocks();

    void drawExplodingBar();

    void drawBlockAtBoardPos(int x, int y, int squareValue);

    void createPieceAtPos(int pieceCenterX, int pieceCenterY);
};

#endif 
