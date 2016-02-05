#include "GameBoard.h"

USING_NS_CC;

const int BOARD_WIDTH_IN_BLOCKS = 8 /* pieces with */ * 2 /* blocks of width */;
const int BOARD_HEIGHT_IN_BLOCKS = 6 /* pieces with */ * 2 /* blocks of height */;

const int BLOCK_VALUE_BLANK = 0;
const int BLOCK_VALUE_TYPE_1 = 1;
const int BLOCK_VALUE_TYPE_2 = 2;
const int BLOCK_VALUE_TYPE_1_MARKED_TO_EXPLODE = 3;
const int BLOCK_VALUE_TYPE_2_MARKED_TO_EXPLODE = 4;
const int BLOCK_VALUE_ABOUT_TO_EXPLODE = 5;

float GAME_BOARD_TICK_IN_SECONDS = 0.3f;

int INVALID_BOARD_POS = -1;

GameBoard::GameBoard()
{
    board = new char[getBoardWidthInBlocks() * getBoardHeightInBlocks()];
    setBoardToInitialState();
}

void GameBoard::setBoardToInitialState()
{
    memset(board, BLOCK_VALUE_BLANK, sizeof(char) * getBoardWidthInBlocks() * getBoardHeightInBlocks());
    blankBlocks = getBoardWidthInBlocks() * getBoardHeightInBlocks();
    explosionBarPos = 0;
    barInBigBlock = false;
    bigBlockStartX = INVALID_BOARD_POS;
}

void GameBoard::spawnNewPiece()
{
    if (blankBlocks < 4)
        return;
    char randShort = (char) (rand() % 16);
    //CCLog("randShort: %i", (int) randShort);
    //CCLog("block: %i %i %i %i", (int) (randShort & 0x08), (int) (randShort & 0x04), (int) (randShort & 0x02), (int) (randShort & 0x01));
    char block[4];
    block[0] = (randShort & 0x08) == 0 ? BLOCK_VALUE_TYPE_1 : BLOCK_VALUE_TYPE_2;
    block[1] = (randShort & 0x04) == 0 ? BLOCK_VALUE_TYPE_1 : BLOCK_VALUE_TYPE_2;
    block[2] = (randShort & 0x02) == 0 ? BLOCK_VALUE_TYPE_1 : BLOCK_VALUE_TYPE_2;
    block[3] = (randShort & 0x01) == 0 ? BLOCK_VALUE_TYPE_1 : BLOCK_VALUE_TYPE_2;
    //CCLog("block: %i %i %i %i", (int) block[0], (int) block[1], (int) block[2], (int) block[3]);
    
    //printBoard();
    int leftPosInPieces;
    int topPosInPieces;
    int tries = 0;
    int maxTries = blankBlocks * 2;
    do {
        leftPosInPieces = rand() % (getBoardWidthInBlocks() - 1);
        topPosInPieces = 0;//rand() % (getBoardHeightInBlocks() - 1);
    } while(!isPlaceValidForNewPiece(leftPosInPieces, topPosInPieces) && ++tries < maxTries);
    if (tries == maxTries) 
    {
        CCLog("couldn't find a suitable place for a new piece");
        return;
    }
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            board[leftPosInPieces + j + ((topPosInPieces + i) * getBoardWidthInBlocks())] = block[j+i*2];
        }
    }
    blankBlocks -= 4;
    //printBoard();
}

bool GameBoard::isPlaceValidForNewPiece(int x, int y)
{
    //CCLog("valid? x=%d y=%d", x, y);
    bool valid = board[x+(y*getBoardWidthInBlocks())] == 0 &&
                 board[x+((y+1)*getBoardWidthInBlocks())] == 0 &&
                 board[(x+1)+(y*getBoardWidthInBlocks())] == 0 &&
                 board[(x+1)+((y+1)*getBoardWidthInBlocks())] == 0;

    return valid;
}

void GameBoard::printBoard() 
{
    CCLog("_____");
    std::stringstream ss;
    for (int i = 0; i < getBoardHeightInBlocks(); i++) 
    {
        for (int j = 0; j < getBoardWidthInBlocks(); j++) 
        {
            ss << (int) board[j + (i * getBoardWidthInBlocks())] << " ";
        }
        CCLog("%s,", ss.str().c_str());
        ss.str(std::string());
    }        
    CCLog("-----");
}

void GameBoard::resetBoard()
{
    setBoardToInitialState();
}

void GameBoard::processBoard()
{
    // run through the board from bottom to top, left to right
    // skiping topmost row
    // bringing blocks down if possible
    // and checking for blocks that can explode

    blankBlocks = 0;
    for (int i = getBoardHeightInBlocks() - 1; i > 0; i--)
    {
        for (int j = 0; j < getBoardWidthInBlocks(); j++)
        {
            int currentBlockPosInBoard = j + (i * getBoardWidthInBlocks());
            int currentBlockValue = board[currentBlockPosInBoard];
            
            // drop the squares
            if (currentBlockValue == BLOCK_VALUE_BLANK)
            {
                blankBlocks++;
                int squareOnTopPosInBoard = j + ((i - 1) * getBoardWidthInBlocks());
                if (board[squareOnTopPosInBoard] != BLOCK_VALUE_BLANK)
                {
                    board[currentBlockPosInBoard] = board[squareOnTopPosInBoard];
                    board[squareOnTopPosInBoard] = BLOCK_VALUE_BLANK;
                }
            // mark squares to explode
            } else if (currentBlockValue != BLOCK_VALUE_ABOUT_TO_EXPLODE) {
                if (j < getBoardWidthInBlocks() - 1)
                {
                    int normalValue, explodeValue;
                    if (currentBlockValue == BLOCK_VALUE_TYPE_1 || currentBlockValue == BLOCK_VALUE_TYPE_1_MARKED_TO_EXPLODE)
                    {
                        normalValue = BLOCK_VALUE_TYPE_1;
                        explodeValue = BLOCK_VALUE_TYPE_1_MARKED_TO_EXPLODE;

                    } else if (currentBlockValue == BLOCK_VALUE_TYPE_2 || currentBlockValue == BLOCK_VALUE_TYPE_2_MARKED_TO_EXPLODE) {
                        normalValue = BLOCK_VALUE_TYPE_2;
                        explodeValue = BLOCK_VALUE_TYPE_2_MARKED_TO_EXPLODE;
                    }
                    int squareOnTopPos = j + ((i - 1) * getBoardWidthInBlocks());
                    int squareOnTopRightPos = (j + 1) + ((i - 1) * getBoardWidthInBlocks());
                    int squareOnRightPos = (j + 1) + (i * getBoardWidthInBlocks());
                    if ((board[squareOnTopPos] == normalValue || board[squareOnTopPos] == explodeValue) &&
                        (board[squareOnTopRightPos] == normalValue || board[squareOnTopRightPos] == explodeValue) &&
                        (board[squareOnRightPos] == normalValue || board[squareOnRightPos] == explodeValue))
                    {
                        board[squareOnTopPos] = explodeValue;
                        board[squareOnTopRightPos] = explodeValue;
                        board[squareOnRightPos] = explodeValue;
                        board[currentBlockPosInBoard] = explodeValue;
                    } 
                }
            }
        }
    }

    // advance explosion bar
    if (explosionBarPos == getBoardWidthInBlocks())
    {
        explosionBarPos = 0;
    } else {
        explosionBarPos++;
    }

    bool endOfBigBlock = true;
    if (explosionBarPos != getBoardWidthInBlocks())
    {
        for (int i = getBoardHeightInBlocks() - 1; i > 0; i--)
        {
            int currentBlockPosInBoard = explosionBarPos + (i * getBoardWidthInBlocks());
            int currentBlockValue = board[currentBlockPosInBoard];
            
            // mark squares as 'about to explode'
            if (currentBlockValue == BLOCK_VALUE_TYPE_1_MARKED_TO_EXPLODE || currentBlockValue == BLOCK_VALUE_TYPE_2_MARKED_TO_EXPLODE)
            {
                board[currentBlockPosInBoard] = BLOCK_VALUE_ABOUT_TO_EXPLODE;
                barInBigBlock = true;
                endOfBigBlock = false;
            }
        } 

        if (barInBigBlock && bigBlockStartX == INVALID_BOARD_POS)
        {
            bigBlockStartX = explosionBarPos;
        }
    }

    // explode big block of squares if end of big block 
    if (barInBigBlock && (endOfBigBlock || explosionBarPos == getBoardWidthInBlocks() - 1))
    {
        for (int i = getBoardHeightInBlocks() - 1; i > 0; i--)
        {
            for (int j = bigBlockStartX; j < explosionBarPos + 1; j++)
            {
                int currentSquarePosInBoard = j + (i * getBoardWidthInBlocks());
                int currentSquareValue = board[currentSquarePosInBoard];
                if (currentSquareValue == BLOCK_VALUE_ABOUT_TO_EXPLODE)
                {
                    board[currentSquarePosInBoard] = BLOCK_VALUE_BLANK;
                }
            }
        }

        barInBigBlock = false;
        bigBlockStartX = INVALID_BOARD_POS;
    }
}

char* GameBoard::getBoardRep()
{
    return board;
}

int GameBoard::getBoardWidthInBlocks()
{
    return BOARD_WIDTH_IN_BLOCKS;
}

int GameBoard::getBoardHeightInBlocks()
{
    return BOARD_HEIGHT_IN_BLOCKS;
}

int GameBoard::getExplosionBarPos()
{
    return explosionBarPos;
}
