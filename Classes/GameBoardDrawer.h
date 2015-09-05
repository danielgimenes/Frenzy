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

    int leftPosBoard; 
    int rightPosBoard; 

    int topPosBoard;
    int bottomPosBoard;

    void drawBoardGrid();

    void drawSquares();

    void drawSquareAtBoardPos(int x, int y, int squareValue);

    void createBlockAtPos(int block_center_x, int block_center_y);
};

#endif 
