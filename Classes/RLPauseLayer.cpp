//
//  RLPauseLayr.cpp
//  Tour2dx
//
//  Created by Melon on 14-11-20.
//
//

#include "RLPauseLayer.h"
#include "RLAudioCtrl.h"
#include "HomeScene.h"

bool RLPauseLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    
//    LayerColor* layer1 = LayerColor::create(Color4B(83,147,200,180), winSize.width*0.8, winSize.height*0.8);
    LayerColor* layer1 = LayerColor::create(Color4B(213,208,151,230));
//    layer1->setPosition(winSize.width*0.1, winSize.height*0.1);
    this->addChild(layer1);
    
    Sprite *bg = Sprite::create("pause.png");
    bg->setPosition(Point(winSize.width*0.5, winSize.height*0.5));
    this->addChild(bg);
    
    Size bgsize = bg->getContentSize();
    
    const float menuFontSize = 22.0;
    const Color3B strColor = Color3B::WHITE;
    Label* lblResume = Label::createWithSystemFont("Continue", "Marker Felt", menuFontSize);
    lblResume->setColor(strColor);
    MenuItemLabel* itemResume= MenuItemLabel::create(lblResume, CC_CALLBACK_0(RLPauseLayer::resumeGameCallback, this));
    
    
    Label* lblExit = Label::createWithSystemFont("Exit", "Marker Felt", menuFontSize);
    lblExit->setColor(strColor);
    MenuItemLabel* itemExit = MenuItemLabel::create(lblExit, CC_CALLBACK_1(RLPauseLayer::exitGameCallback, this));
    
    auto mainMenu = Menu::create(itemResume, itemExit, NULL);
    
    mainMenu->setPosition(Point(bgsize.width*0.5, bgsize.height*0.5));
    mainMenu->alignItemsVerticallyWithPadding(22);
    bg->addChild(mainMenu);
    
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(RLPauseLayer::onTouchBegan, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void RLPauseLayer::resumeGameCallback()
{
    Director::getInstance()->resume();
    if (RLAudioCtrl::getInstance()->getEffectState()) {
        SimpleAudioEngine::getInstance()->resumeAllEffects();
    }
    if (RLAudioCtrl::getInstance()->getMusicState()) {
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
    
    this->removeFromParent();
}

void RLPauseLayer::exitGameCallback(cocos2d::Ref *pSender)
{
    Director::getInstance()->resume();
    auto scene = HomeScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
}

bool RLPauseLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    return true;
}