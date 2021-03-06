#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "GameBoard.h"
#include "GameBoardDrawer.h"

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(GameScene);

private:
    cocos2d::Vec2 origin;
    cocos2d::Size visibleSize;

    cocos2d::EventListenerTouchOneByOne *touchListener;

    GameBoard *board;

    GameBoardDrawer *drawer;

    void onBoardTimerTick(float delta);

    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);

    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

};

#endif 
