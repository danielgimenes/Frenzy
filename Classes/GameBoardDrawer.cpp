#include "GameBoardDrawer.h"
#include "GameBoard.h"
#include "Colors.h"

USING_NS_CC;

const int BLOCK_SIZE = 30;
const int BLOCK_SPACING = 2;

const std::string BLOCK_TEXTURE_FILE_BY_VALUE[] =
{
    "invalid.png", 
    "texture1.png",
    "texture2.png",
    "texture4.png",
    "texture5.png",
    "texture3.png"
};

GameBoardDrawer::GameBoardDrawer(GameBoard *board, Node *node, cocos2d::Vec2 origin, cocos2d::Size visibleSize)
{
    this->board = board;
    horizontalCenter = origin.x + (visibleSize.width / 2);
    verticalCenter = origin.y + (visibleSize.height / 2);

    horizontalLineSize = board->getBoardWidthInBlocks() * (BLOCK_SIZE + BLOCK_SPACING);
    verticalLineSize = board->getBoardHeightInBlocks() * (BLOCK_SIZE + BLOCK_SPACING);

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
    drawBlocks();
    drawExplodingBar();
}

void GameBoardDrawer::drawBoardGrid()
{
    auto drawBoardNode = DrawNode::create();
    
    // horizontal lines
    for (int i = 2, topPos = 0; i < board->getBoardHeightInBlocks(); ++i)
    { 
        topPos = topPosBoard - (i * (BLOCK_SIZE + BLOCK_SPACING));
        drawBoardNode->drawLine(Vec2(leftPosBoard, topPos), Vec2(rightPosBoard, topPos), BOARD_LINES_COLOR);
    }

    // vertical lines
    for (int i = 1, leftPos = 0; i < board->getBoardWidthInBlocks(); ++i)
    {
        leftPos = leftPosBoard + (i * (BLOCK_SIZE + BLOCK_SPACING));
        drawBoardNode->drawLine(Vec2(leftPos, bottomPosBoard),
            Vec2(leftPos, topPosBoard - ((BLOCK_SIZE + BLOCK_SPACING) * 2)), BOARD_LINES_COLOR);
    }

    // board enclosing rectangle
    drawBoardNode->drawRect(Vec2(leftPosBoard, bottomPosBoard),
         Vec2(rightPosBoard, topPosBoard - ((BLOCK_SIZE + BLOCK_SPACING) * 2)), BOARD_RECT_COLOR);

    boardRootNode->addChild(drawBoardNode, 1);
}

void GameBoardDrawer::drawBlocks()
{
    char *boardRep = board->getBoardRep();

    for (int i = 0; i < board->getBoardHeightInBlocks(); i++) 
    {
        for (int j = 0; j < board->getBoardWidthInBlocks(); j++) 
        {
            int blockValue = (int) boardRep[j + (i * board->getBoardWidthInBlocks())];
            if (blockValue > 0)
            {
                drawBlockAtBoardPos(j, i, blockValue);
            }
        }
    }        
}

void GameBoardDrawer::drawExplodingBar()
{
    auto drawBoardNode = DrawNode::create();
    int leftPos = leftPosBoard + (board->getExplosionBarPos() * (BLOCK_SIZE + BLOCK_SPACING));
    drawBoardNode->drawLine(Vec2(leftPos, bottomPosBoard - 10),
        Vec2(leftPos, topPosBoard - ((BLOCK_SIZE + BLOCK_SPACING) * 2) + 10), EXPLODING_BAR_COLOR);
    boardRootNode->addChild(drawBoardNode, 1);
}

void GameBoardDrawer::drawBlockAtBoardPos(int x, int y, int blockValue)
{
    int leftPos = (BLOCK_SPACING / 2) + leftPosBoard + (x * (BLOCK_SIZE + BLOCK_SPACING));
    int topPos = (BLOCK_SPACING / 2) + topPosBoard - ((y+1) * (BLOCK_SIZE + BLOCK_SPACING));
    
    auto block = Sprite::create(BLOCK_TEXTURE_FILE_BY_VALUE[blockValue], Rect(0, 0, BLOCK_SIZE, BLOCK_SIZE));
    block->setAnchorPoint(Vec2(0, 0));
    block->setPosition(Vec2(leftPos, topPos));
    boardRootNode->addChild(block, 5);
}

void GameBoardDrawer::createPieceAtPos(int blockCenterX, int blockCenterY)
{
    int blockLeftPos = blockCenterX - (BLOCK_SPACING / 2) - BLOCK_SIZE;
    int blockRightPos = blockCenterX + (BLOCK_SPACING / 2);
    int blockTopPos = blockCenterY + (BLOCK_SPACING / 2);
    int blockBottomPos = blockCenterY - (BLOCK_SPACING / 2) - BLOCK_SIZE;

    std::string texture1FilePath = "texture1.png";
    std::string texture2FilePath = "texture2.png";

    auto blockLeftTop = Sprite::create(texture1FilePath, Rect(0, 0, BLOCK_SIZE, BLOCK_SIZE));
    blockLeftTop->setAnchorPoint(Vec2(0, 0));
    blockLeftTop->setPosition(Vec2(blockLeftPos, blockTopPos));
    boardRootNode->addChild(blockLeftTop, 5);

    auto blockRightTop = Sprite::create(texture2FilePath, Rect(0, 0, BLOCK_SIZE, BLOCK_SIZE));
    blockRightTop->setAnchorPoint(Vec2(0, 0));
    blockRightTop->setPosition(Vec2(blockRightPos, blockTopPos));
    boardRootNode->addChild(blockRightTop, 5);

    auto blockLeftBottom = Sprite::create(texture2FilePath, Rect(0, 0, BLOCK_SIZE, BLOCK_SIZE));
    blockLeftBottom->setAnchorPoint(Vec2(0, 0));
    blockLeftBottom->setPosition(Vec2(blockLeftPos, blockBottomPos));
    boardRootNode->addChild(blockLeftBottom, 5);

    auto blockRightBottom = Sprite::create(texture1FilePath, Rect(0, 0, BLOCK_SIZE, BLOCK_SIZE));
    blockRightBottom->setAnchorPoint(Vec2(0, 0));
    blockRightBottom->setPosition(Vec2(blockRightPos, blockBottomPos));
    boardRootNode->addChild(blockRightBottom, 5);
}

