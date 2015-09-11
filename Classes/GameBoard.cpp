#include "GameBoard.h"

USING_NS_CC;

const int BOARD_WIDTH_IN_SQUARES = 8 /* blocks with */ * 2 /* squares of width */;
const int BOARD_HEIGHT_IN_SQUARES = 6 /* blocks with */ * 2 /* squares of width */;

const int SQUARE_VALUE_BLANK = 0;
const int SQUARE_VALUE_TYPE_1 = 1;
const int SQUARE_VALUE_TYPE_2 = 2;
const int SQUARE_VALUE_TYPE_1_MARKED_TO_EXPLODE = 3;
const int SQUARE_VALUE_TYPE_2_MARKED_TO_EXPLODE = 4;

GameBoard::GameBoard()
{
    board = new char[getBoardWidthInSquares() * getBoardHeightInSquares()];
    memset(board, SQUARE_VALUE_BLANK, sizeof(char) * getBoardWidthInSquares() * getBoardHeightInSquares());
    blankSquares = getBoardWidthInSquares() * getBoardHeightInSquares();
}

void GameBoard::spawnNewBlock()
{
    if (blankSquares < 4)
        return;
    char randShort = (char) (rand() % 16);
    //CCLog("randShort: %i", (int) randShort);
    //CCLog("block: %i %i %i %i", (int) (randShort & 0x08), (int) (randShort & 0x04), (int) (randShort & 0x02), (int) (randShort & 0x01));
    char block[4];
    block[0] = (randShort & 0x08) == 0 ? SQUARE_VALUE_TYPE_1 : SQUARE_VALUE_TYPE_2;
    block[1] = (randShort & 0x04) == 0 ? SQUARE_VALUE_TYPE_1 : SQUARE_VALUE_TYPE_2;
    block[2] = (randShort & 0x02) == 0 ? SQUARE_VALUE_TYPE_1 : SQUARE_VALUE_TYPE_2;
    block[3] = (randShort & 0x01) == 0 ? SQUARE_VALUE_TYPE_1 : SQUARE_VALUE_TYPE_2;
    //CCLog("block: %i %i %i %i", (int) block[0], (int) block[1], (int) block[2], (int) block[3]);
    
    //printBoard();
    int leftPosInSquares;
    int topPosInSquares;
    int tries = 0;
    int maxTries = blankSquares * 2;
    do {
        leftPosInSquares = rand() % (getBoardWidthInSquares() - 1);
        topPosInSquares = 0;//rand() % (getBoardHeightInSquares() - 1);
    } while(!isPlaceValidForNewBlock(leftPosInSquares, topPosInSquares) && ++tries < maxTries);
    if (tries == maxTries) 
    {
        CCLog("couldn't find a suitable place for a new block");
        return;
    }
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            board[leftPosInSquares + j + ((topPosInSquares + i) * getBoardWidthInSquares())] = block[j+i*2];
        }
    }
    blankSquares -= 4;
    //printBoard();
}

bool GameBoard::isPlaceValidForNewBlock(int x, int y) 
{
    //CCLog("valid? x=%d y=%d", x, y);
    bool valid = board[x+(y*getBoardWidthInSquares())] == 0 &&
                 board[x+((y+1)*getBoardWidthInSquares())] == 0 &&
                 board[(x+1)+(y*getBoardWidthInSquares())] == 0 &&
                 board[(x+1)+((y+1)*getBoardWidthInSquares())] == 0;

    return valid;
}

void GameBoard::printBoard() 
{
    CCLog("_____");
    std::stringstream ss;
    for (int i = 0; i < getBoardHeightInSquares(); i++) 
    {
        for (int j = 0; j < getBoardWidthInSquares(); j++) 
        {
            ss << (int) board[j + (i * getBoardWidthInSquares())] << " ";
        }
        CCLog("%s,", ss.str().c_str());
        ss.str(std::string());
    }        
    CCLog("-----");
}

void GameBoard::resetBoard()
{
    memset(board, SQUARE_VALUE_BLANK, sizeof(char) * getBoardWidthInSquares() * getBoardHeightInSquares());
    blankSquares = getBoardWidthInSquares() * getBoardHeightInSquares();
}

void GameBoard::processBoard()
{
    // run through the board from bottom to top, left to right
    // skiping topmost row
    // bringing squares down if possible
    // and checking for blocks that can explode

    for (int i = getBoardHeightInSquares() - 1; i > 0; i--)
    {
        for (int j = 0; j < getBoardWidthInSquares(); j++)
        {
            int currentSquarePosInBoard = j + (i * getBoardWidthInSquares());
            int currentSquareValue = board[currentSquarePosInBoard];
            
            // if this square is blank
            if (currentSquareValue == SQUARE_VALUE_BLANK)
            {
                int squareOnTopPosInBoard = j + ((i - 1) * getBoardWidthInSquares());
                // and square on top is not blank
                if (board[squareOnTopPosInBoard] != SQUARE_VALUE_BLANK)
                {
                    // set this square as top square's value
                    board[currentSquarePosInBoard] = board[squareOnTopPosInBoard];
                    // set top square as blank
                    board[squareOnTopPosInBoard] = SQUARE_VALUE_BLANK;
                }
            } else {
                if (j < getBoardWidthInSquares() - 1)
                {
                    int normalValue, explodeValue;
                    if (currentSquareValue == SQUARE_VALUE_TYPE_1 || currentSquareValue == SQUARE_VALUE_TYPE_1_MARKED_TO_EXPLODE)
                    {
                        normalValue = SQUARE_VALUE_TYPE_1;
                        explodeValue = SQUARE_VALUE_TYPE_1_MARKED_TO_EXPLODE;

                    } else if (currentSquareValue == SQUARE_VALUE_TYPE_2 || currentSquareValue == SQUARE_VALUE_TYPE_2_MARKED_TO_EXPLODE) {
                        normalValue = SQUARE_VALUE_TYPE_2;
                        explodeValue = SQUARE_VALUE_TYPE_2_MARKED_TO_EXPLODE;
                    }
                    int squareOnTopPos = j + ((i - 1) * getBoardWidthInSquares());
                    int squareOnTopRightPos = (j + 1) + ((i - 1) * getBoardWidthInSquares());
                    int squareOnRightPos = (j + 1) + (i * getBoardWidthInSquares());
                    if ((board[squareOnTopPos] == normalValue || board[squareOnTopPos] == explodeValue) &&
                        (board[squareOnTopRightPos] == normalValue || board[squareOnTopRightPos] == explodeValue) &&
                        (board[squareOnRightPos] == normalValue || board[squareOnRightPos] == explodeValue))
                    {
                        board[squareOnTopPos] = explodeValue;
                        board[squareOnTopRightPos] = explodeValue;
                        board[squareOnRightPos] = explodeValue;
                        board[currentSquarePosInBoard] = explodeValue;
                    } 
                }    

            }

            
        }
    }

}

char* GameBoard::getBoardRep()
{
    return board;
}

int GameBoard::getBoardWidthInSquares()
{
    return BOARD_WIDTH_IN_SQUARES;
}

int GameBoard::getBoardHeightInSquares()
{
    return BOARD_HEIGHT_IN_SQUARES;
}

