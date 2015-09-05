#include "GameScene.h"
#include "GameBoard.h"

USING_NS_CC;

int SQUARE_SIZE = 30;
int SQUARE_SPACING = 2;

Scene* Game::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Game::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Game::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
        [] (Ref *sender) {
            Director::getInstance()->end();
            #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
            #endif
        });

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2,
                                origin.y + closeItem->getContentSize().height/2));

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto board = new GameBoard();
    board->spawnNewBlock();
    board->spawnNewBlock();
    board->spawnNewBlock();
    board->spawnNewBlock();
    board->spawnNewBlock();
    board->spawnNewBlock();
    board->spawnNewBlock();
    board->spawnNewBlock();
    board->spawnNewBlock();
    board->spawnNewBlock();
    board->spawnNewBlock();
    board->spawnNewBlock();
    board->spawnNewBlock();
    board->spawnNewBlock();
    board->spawnNewBlock();
    
    drawGameBoard(board);

    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(Game::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

void Game::drawGameBoard(GameBoard *board)
{
    auto drawBoardNode = DrawNode::create();
    auto boardLinesColor = Color4F::RED;
    auto boardRectColor = Color4F::WHITE;
    
    int horizontalCenter = origin.x + (visibleSize.width / 2);
    int verticalCenter = origin.y + (visibleSize.height / 2);

    int horizontalLineSize = board->getBoardWidthInSquares() * (SQUARE_SIZE + SQUARE_SPACING);
    int verticalLineSize = board->getBoardHeightInSquares() * (SQUARE_SIZE + SQUARE_SPACING);

    int leftPosBoard = (int) (horizontalCenter - (horizontalLineSize / 2)); 
    int rightPosBoard = (int) (horizontalCenter + (horizontalLineSize / 2)); 

    int topPosBoard = (int) (verticalCenter + (verticalLineSize / 2));
    int bottomPosBoard = (int) (verticalCenter - (verticalLineSize / 2));

    // horizontal lines
    for (int i = 1, topPos = 0; i < board->getBoardHeightInSquares(); ++i)
    { 
        topPos = topPosBoard - (i * (SQUARE_SIZE + SQUARE_SPACING));
        drawBoardNode->drawLine(Vec2(leftPosBoard, topPos), Vec2(rightPosBoard, topPos), boardLinesColor);
    }

    // vertical lines
    for (int i = 1, leftPos = 0; i < board->getBoardWidthInSquares(); ++i)
    {
        leftPos = leftPosBoard + (i * (SQUARE_SIZE + SQUARE_SPACING));
        drawBoardNode->drawLine(Vec2(leftPos, bottomPosBoard), Vec2(leftPos, topPosBoard), boardLinesColor);
    }

    // board enclosing rectangle
    drawBoardNode->drawRect(Vec2(leftPosBoard, bottomPosBoard), Vec2(rightPosBoard, topPosBoard), boardRectColor);

    this->addChild(drawBoardNode, 1);
}

bool Game::onTouchBegan(Touch *touch, Event *event)
{
    EventTouch* e = (EventTouch*)event;
    Vec2 touchPos = e->getTouches()[0]->getLocation();
    createBlockAtPos((int) touchPos.x, (int) touchPos.y);
    return true;
}

void Game::onTouchMoved(Touch *touch, Event *event)
{
}

void Game::onTouchEnded(Touch *touch, Event *event)
{
}

void Game::createBlockAtPos(int blockCenterX, int blockCenterY)
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
    this->addChild(squareLeftTop, 5);

    auto squareRightTop = Sprite::create(texture2FilePath, Rect(0, 0, SQUARE_SIZE, SQUARE_SIZE));
    squareRightTop->setAnchorPoint(Vec2(0, 0));
    squareRightTop->setPosition(Vec2(squareRightPos, squareTopPos));
    this->addChild(squareRightTop, 5);

    auto squareLeftBottom = Sprite::create(texture2FilePath, Rect(0, 0, SQUARE_SIZE, SQUARE_SIZE));
    squareLeftBottom->setAnchorPoint(Vec2(0, 0));
    squareLeftBottom->setPosition(Vec2(squareLeftPos, squareBottomPos));
    this->addChild(squareLeftBottom, 5);

    auto squareRightBottom = Sprite::create(texture1FilePath, Rect(0, 0, SQUARE_SIZE, SQUARE_SIZE));
    squareRightBottom->setAnchorPoint(Vec2(0, 0));
    squareRightBottom->setPosition(Vec2(squareRightPos, squareBottomPos));
    this->addChild(squareRightBottom, 5);
}

