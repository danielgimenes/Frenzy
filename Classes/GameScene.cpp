#include "GameScene.h"

USING_NS_CC;

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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

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
    

    /*
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [start, exit] (Touch* touch, Event* event) {
        Node* targetNode = ((EventTouch*) event)->getCurrentTarget();
        if (targetNode == start)
        {
            CCLog("start");
            return true;
        }
        else if (targetNode == exit)
        {
            CCLog("exit");
            return true;
        }
        CCLog("other");
        CCLog("target: %i at %f, %f", targetNode, targetNode->getPosition().x, targetNode->getPosition().y);
        CCLog("start: %i", start);
        CCLog("exit: %i", exit);
        return false;
    }; 
    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    */

    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(Game::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
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
//    CCLOG("click pos %d, %d", blockCenterX, blockCenterY);

    int SQUARE_SIZE = 30;
    int SQUARE_SPACING = 2;

    int squareLeftPos = blockCenterX - (SQUARE_SPACING / 2) - SQUARE_SIZE;
    int squareRightPos = blockCenterX + (SQUARE_SPACING / 2);
    int squareTopPos = blockCenterY + (SQUARE_SPACING / 2);
    int squareBottomPos = blockCenterY - (SQUARE_SPACING / 2) - SQUARE_SIZE;

/*
    CCLOG("squareLeftPos = %d", squareLeftPos);
    CCLOG("squareRightPos = %d", squareRightPos);
    CCLOG("squareTopPos = %d", squareTopPos);
    CCLOG("squareBottomPos = %d", squareBottomPos);
*/

    std::string texture1FilePath = "texture1.png";
    std::string texture2FilePath = "texture2.png";

    auto squareLeftTop = Sprite::create(texture1FilePath, Rect(0, 0, SQUARE_SIZE, SQUARE_SIZE));
    squareLeftTop->setAnchorPoint(Vec2(0, 0));
    squareLeftTop->setPosition(Vec2(squareLeftPos, squareTopPos));
    this->addChild(squareLeftTop, 0);

    auto squareRightTop = Sprite::create(texture2FilePath, Rect(0, 0, SQUARE_SIZE, SQUARE_SIZE));
    squareRightTop->setAnchorPoint(Vec2(0, 0));
    squareRightTop->setPosition(Vec2(squareRightPos, squareTopPos));
    this->addChild(squareRightTop, 0);

    auto squareLeftBottom = Sprite::create(texture2FilePath, Rect(0, 0, SQUARE_SIZE, SQUARE_SIZE));
    squareLeftBottom->setAnchorPoint(Vec2(0, 0));
    squareLeftBottom->setPosition(Vec2(squareLeftPos, squareBottomPos));
    this->addChild(squareLeftBottom, 0);

    auto squareRightBottom = Sprite::create(texture1FilePath, Rect(0, 0, SQUARE_SIZE, SQUARE_SIZE));
    squareRightBottom->setAnchorPoint(Vec2(0, 0));
    squareRightBottom->setPosition(Vec2(squareRightPos, squareBottomPos));
    this->addChild(squareRightBottom, 0);
}

