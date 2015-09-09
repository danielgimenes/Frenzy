#include "GameScene.h"
#include "Colors.h"

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

    auto colorLayer = LayerColor::create(GAME_SCENE_BACKGROUND_COLOR);
    this->addChild(colorLayer, 0);
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    board = new GameBoard();
    board->spawnNewBlock();
    drawer = new GameBoardDrawer(board, this, origin, visibleSize); 
    drawer->drawGameBoard();

    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
        [] (Ref *sender) {
            Director::getInstance()->end();
            #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
            #endif
        });
    closeItem->setScaleX(2.0f);
    closeItem->setScaleY(2.0f);

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 - 20,
                                origin.y + closeItem->getContentSize().height/2 + 30));

    auto resetItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
        [this] (Ref *sender) {
            this->board->resetBoard();
            this->drawer->drawGameBoard();
        });
    resetItem->setScaleX(2.0f);
    resetItem->setScaleY(2.0f);

    resetItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 - 70,
                                origin.y + closeItem->getContentSize().height/2 + 30));

    Vector<MenuItem*> menuItems;
    menuItems.pushBack(closeItem);
    menuItems.pushBack(resetItem);

    auto menu = Menu::createWithArray(menuItems);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(Game::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->schedule(schedule_selector(Game::onBoardTimerTick), this, 0.5f, CC_REPEAT_FOREVER, 0.0f, false);

    return true;
}

void Game::onBoardTimerTick(float delta)
{
    board->processBoard();
    drawer->drawGameBoard();
}

bool Game::onTouchBegan(Touch *touch, Event *event)
{
    board->spawnNewBlock();
    drawer->drawGameBoard();
    return true;
}

void Game::onTouchMoved(Touch *touch, Event *event)
{
}

void Game::onTouchEnded(Touch *touch, Event *event)
{
}
