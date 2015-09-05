#include "MainMenuScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto title = Label::createWithTTF("F r e n z y", "fonts/Marker Felt.ttf", 60);
    title->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + ((visibleSize.height / 4) * 3)));
    this->addChild(title, 1);

    auto startLabel = Label::createWithTTF("START", "fonts/Marker Felt.ttf", 32);
    auto startMenuItem = MenuItemLabel::create(startLabel, [] (Ref *sender) {
        Director::getInstance()->replaceScene(Game::createScene());
    });
    startMenuItem->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + title->getPosition().y - (title->getContentSize().height / 2) - 40));

    auto exitLabel = Label::createWithTTF("EXIT", "fonts/Marker Felt.ttf", 32);
    auto exitMenuItem = MenuItemLabel::create(exitLabel, [] (Ref *sender) {
        Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    });
    exitMenuItem->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + startMenuItem->getPosition().y - (startMenuItem->getContentSize().height / 2) - 40));

    Vector<MenuItem*> menuItems;
    menuItems.pushBack(startMenuItem);
    menuItems.pushBack(exitMenuItem);

    auto menu = Menu::createWithArray(menuItems);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}

