#include "GameScene.h"
#include "Colors.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
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
    
    int SCREEN_BORDER = 20;

    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
        [] (Ref *sender) {
            Director::getInstance()->end();
            #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
            #endif
        });
    closeItem->setScaleX(4.0f);
    closeItem->setScaleY(4.0f);

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->boundingBox().size.width/2 - SCREEN_BORDER,
                                origin.y + closeItem->boundingBox().size.height/2 + SCREEN_BORDER));

    auto resetItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
        [this] (Ref *sender) {
            this->board->resetBoard();
            this->drawer->drawGameBoard();
        });
    resetItem->setScaleX(4.0f);
    resetItem->setScaleY(4.0f);
    int SEPARATOR = 5;
    resetItem->setPosition(Vec2(closeItem->getPosition().x - closeItem->boundingBox().size.width/2 - resetItem->boundingBox().size.width/2 - SEPARATOR,
                                origin.y + resetItem->boundingBox().size.height/2 + SCREEN_BORDER));

    Vector<MenuItem*> menuItems;
    menuItems.pushBack(closeItem);
    menuItems.pushBack(resetItem);

    auto menu = Menu::createWithArray(menuItems);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->schedule(schedule_selector(GameScene::onBoardTimerTick), this, GAME_BOARD_TICK_IN_SECONDS, CC_REPEAT_FOREVER, 0.0f, false);

    return true;
}

void GameScene::onBoardTimerTick(float delta)
{
    board->processBoard();
    drawer->drawGameBoard();
}

bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
    board->spawnNewBlock();
    drawer->drawGameBoard();
    return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *event)
{
}

void GameScene::onTouchEnded(Touch *touch, Event *event)
{
}
