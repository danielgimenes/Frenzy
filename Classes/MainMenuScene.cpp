#include "MainMenuScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    std::ostringstream logStream;
    logStream << "visibleSize: " << visibleSize.width << ", " << visibleSize.height << std::endl;
    logStream << "origin: " << origin.x << ", " << origin.y << std::endl;
    Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    logStream << "frame: " << frameSize.width << ", " << frameSize.height; 
    auto screenLog = Label::createWithTTF(logStream.str(), "fonts/arial.ttf", 20);
    screenLog->setAnchorPoint(Vec2(0, 0));
    screenLog->setPosition(Vec2(origin.x, origin.y));
    this->addChild(screenLog, 1);
    
    auto title = Label::createWithTTF("F r e n z y", "fonts/Marker Felt.ttf", 90);
    title->setPosition(Vec2(origin.x + (visibleSize.width / 2), origin.y + ((visibleSize.height / 4) * 3)));
    this->addChild(title, 1);

    auto startLabel = Label::createWithTTF("START", "fonts/Marker Felt.ttf", 60);
    auto startMenuItem = MenuItemLabel::create(startLabel, [] (Ref *sender) {
        Director::getInstance()->replaceScene(GameScene::createScene());
    });
    startMenuItem->setPosition(Vec2(origin.x + (visibleSize.width / 2), title->getPosition().y - (int)(title->getContentSize().height * 1.5f)));

    auto exitLabel = Label::createWithTTF("EXIT", "fonts/Marker Felt.ttf", 60);
    auto exitMenuItem = MenuItemLabel::create(exitLabel, [] (Ref *sender) {
        Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    });
    exitMenuItem->setPosition(Vec2(origin.x + (visibleSize.width / 2), startMenuItem->getPosition().y - (int)(startMenuItem->getContentSize().height * 1.5f)));

    Vector<MenuItem*> menuItems;
    menuItems.pushBack(startMenuItem);
    menuItems.pushBack(exitMenuItem);

    auto menu = Menu::createWithArray(menuItems);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}

