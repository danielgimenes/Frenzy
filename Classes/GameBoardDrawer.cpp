#include "GameBoardDrawer.h"
#include "GameBoard.h"
#include "Colors.h"

USING_NS_CC;

const int SQUARE_SIZE = 30;
const int SQUARE_SPACING = 2;

const std::string SQUARE_TEXTURE_FILE_BY_VALUE[] =
{
    "invalid.png", 
    "texture1.png",
    "texture2.png",
    "texture4.png",
    "texture5.png"
};

GameBoardDrawer::GameBoardDrawer(GameBoard *board, Node *node, cocos2d::Vec2 origin, cocos2d::Size visibleSize)
{
    this->board = board;
    horizontalCenter = origin.x + (visibleSize.width / 2);
    verticalCenter = origin.y + (visibleSize.height / 2);

    horizontalLineSize = board->getBoardWidthInSquares() * (SQUARE_SIZE + SQUARE_SPACING);
    verticalLineSize = board->getBoardHeightInSquares() * (SQUARE_SIZE + SQUARE_SPACING);

    leftPosBoard = horizontalCenter - (horizontalLineSize / 2); 
    rightPosBoard = horizontalCenter + (horizontalLineSize / 2); 

    topPosBoard = verticalCenter + (verticalLineSize / 2);
    bottomPosBoard = verticalCenter - (verticalLineSize / 2);

    boardRootNode = Node::create();
    node->addChild(boardRootNode);
}

void GameBoardDrawer::drawGameBoard()
{
    boardRootNode->removeAllChildren();
    drawBoardGrid();
    drawSquares();
}

void GameBoardDrawer::drawBoardGrid()
{
    auto drawBoardNode = DrawNode::create();
    
    // horizontal lines
    for (int i = 2, topPos = 0; i < board->getBoardHeightInSquares(); ++i)
    { 
        topPos = topPosBoard - (i * (SQUARE_SIZE + SQUARE_SPACING));
        drawBoardNode->drawLine(Vec2(leftPosBoard, topPos), Vec2(rightPosBoard, topPos), BOARD_LINES_COLOR);
    }

    // vertical lines
    for (int i = 1, leftPos = 0; i < board->getBoardWidthInSquares(); ++i)
    {
        leftPos = leftPosBoard + (i * (SQUARE_SIZE + SQUARE_SPACING));
        drawBoardNode->drawLine(Vec2(leftPos, bottomPosBoard),
            Vec2(leftPos, topPosBoard - ((SQUARE_SIZE + SQUARE_SPACING) * 2)), BOARD_LINES_COLOR);
    }

    // board enclosing rectangle
    drawBoardNode->drawRect(Vec2(leftPosBoard, bottomPosBoard),
         Vec2(rightPosBoard, topPosBoard - ((SQUARE_SIZE + SQUARE_SPACING) * 2)), BOARD_RECT_COLOR);

    boardRootNode->addChild(drawBoardNode, 1);
}

void GameBoardDrawer::drawSquares()
{
    char *boardRep = board->getBoardRep();

    for (int i = 0; i < board->getBoardHeightInSquares(); i++) 
    {
        for (int j = 0; j < board->getBoardWidthInSquares(); j++) 
        {
            int squareValue = (int) boardRep[j + (i * board->getBoardWidthInSquares())];
            if (squareValue > 0)
            {
                drawSquareAtBoardPos(j, i, squareValue);
            }
        }
    }        
}

void GameBoardDrawer::drawSquareAtBoardPos(int x, int y, int squareValue)
{
    int leftPos = (SQUARE_SPACING / 2) + leftPosBoard + (x * (SQUARE_SIZE + SQUARE_SPACING));
    int topPos = (SQUARE_SPACING / 2) + topPosBoard - ((y+1) * (SQUARE_SIZE + SQUARE_SPACING));
    
    auto square = Sprite::create(SQUARE_TEXTURE_FILE_BY_VALUE[squareValue], Rect(0, 0, SQUARE_SIZE, SQUARE_SIZE));
    square->setAnchorPoint(Vec2(0, 0));
    square->setPosition(Vec2(leftPos, topPos));
    boardRootNode->addChild(square, 5);
}

void GameBoardDrawer::createBlockAtPos(int blockCenterX, int blockCenterY)
{
    int squareLeftPos = blockCenterX - (SQUARE_SPACING / 2) - SQUARE_SIZE;
    int squareRightPos = blockCenterX + (SQUARE_SPACING / 2);
    int squareTopPos = blockCenterY + (SQUARE_SPACING / 2);
    int squareBottomPos = blockCenterY - (SQUARE_SPACING / 2) - SQUARE_SIZE;

    std::string texture1FilePath = "texture1.png";
    std::string texture2FilePath = "texture2.png";

    auto squareLeftTop = Sprite::create(texture1FilePath, Rect(0, 0, SQUARE_SIZE, SQUARE_SIZE));
    squareLeftTop->setAnchorPoint(Vec2(0, 0));
    squareLeftTop->setPosition(Vec2(squareLeftPos, squareTopPos));
    boardRootNode->addChild(squareLeftTop, 5);

    auto squareRightTop = Sprite::create(texture2FilePath, Rect(0, 0, SQUARE_SIZE, SQUARE_SIZE));
    squareRightTop->setAnchorPoint(Vec2(0, 0));
    squareRightTop->setPosition(Vec2(squareRightPos, squareTopPos));
    boardRootNode->addChild(squareRightTop, 5);

    auto squareLeftBottom = Sprite::create(texture2FilePath, Rect(0, 0, SQUARE_SIZE, SQUARE_SIZE));
    squareLeftBottom->setAnchorPoint(Vec2(0, 0));
    squareLeftBottom->setPosition(Vec2(squareLeftPos, squareBottomPos));
    boardRootNode->addChild(squareLeftBottom, 5);

    auto squareRightBottom = Sprite::create(texture1FilePath, Rect(0, 0, SQUARE_SIZE, SQUARE_SIZE));
    squareRightBottom->setAnchorPoint(Vec2(0, 0));
    squareRightBottom->setPosition(Vec2(squareRightPos, squareBottomPos));
    boardRootNode->addChild(squareRightBottom, 5);
}

