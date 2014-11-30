//
//  HomeScene.cpp
//  Tour2dx
//
//  Created by Melon on 14-11-18.
//
//

#include "HomeScene.h"
#include "MainScene.h"
#include "OptionScene.h"
#include "AboutScene.h"

Scene* HomeScene::createScene()
{
    auto scene = Scene::create();
    auto layer = HomeScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool HomeScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
//    auto layer = LayerGradient::create(Color4B(102,0,0,255), Color4B(102,102,0,127), Point(0,-1));
//    this->addChild(layer);
    
    Size winSize = Director::getInstance()->getWinSize();
    
    Sprite *bg = Sprite::create("homescene.png");
    bg->setPosition(Point(winSize.width*0.5, winSize.height*0.5));
    this->addChild(bg);
    
    
    const std::string _title = "Match3 Crush";
    auto titleLabel = Label::createWithBMFont("futura-48.fnt",_title);
    titleLabel->setPosition(Point(winSize.width*0.5, winSize.height*0.8));
    titleLabel->setScale(1.8);
    this->addChild(titleLabel);
    
//    std::string fontname = "bitmapFontTest3.fnt";
    std::string fontname = "Marker Felt";
//    float scaleSize = 1.7;
    const float menuFontSize = 25.0;
    const Color3B fontcolor = Color3B(197, 97, 49);
    
    Label* lblPlay = Label::createWithSystemFont("New Game", fontname, menuFontSize);
    lblPlay->setColor(fontcolor);
//    Label* lblPlay = Label::createWithBMFont(fontname, "New Game");
//    lblPlay->setScale(scaleSize);
    MenuItemLabel* itemPlay = MenuItemLabel::create(lblPlay, CC_CALLBACK_0(HomeScene::startGameCallback, this));
    
    Label* lblSetting = Label::createWithSystemFont("Options", fontname, menuFontSize);
    lblSetting->setColor(fontcolor);
//    Label* lblSetting = Label::createWithBMFont(fontname,"Options");
//    lblSetting->setScale(scaleSize);
    MenuItemLabel* itemSetting = MenuItemLabel::create(lblSetting, CC_CALLBACK_1(HomeScene::menuCallback, this));
    itemSetting->setTag(20);
    
    Label* lblAbout = Label::createWithSystemFont("Introduction", fontname, menuFontSize);
    lblAbout->setColor(fontcolor);
//    Label* lblAbout = Label::createWithBMFont(fontname, "Introduction");
//    lblAbout->setScale(scaleSize);
    MenuItemLabel* itemAbout = MenuItemLabel::create(lblAbout, CC_CALLBACK_1(HomeScene::menuCallback, this));
    itemAbout->setTag(21);
    
    Label* lblExit = Label::createWithSystemFont("Exit", fontname, menuFontSize);
    lblExit->setColor(fontcolor);
//    Label* lblExit = Label::createWithBMFont(fontname, "Exit");
//    lblExit->setScale(scaleSize);
    MenuItemLabel* itemExit = MenuItemLabel::create(lblExit, CC_CALLBACK_1(HomeScene::exitCallback, this));
    
    auto mainMenu = Menu::create(itemPlay, itemSetting, itemAbout, itemExit, NULL);
    mainMenu->setPosition(Point(winSize.width*0.5, winSize.height*0.45));
    mainMenu->alignItemsVerticallyWithPadding(27);
    this->addChild(mainMenu);
    
    bool bmusicOn = UserDefault::getInstance()->getBoolForKey("ismusicon", true);
    bool beffectOn = UserDefault::getInstance()->getBoolForKey("iseffecton", true);
    
    
    RLAudioCtrl* audioctrl = RLAudioCtrl::getInstance();
    
    audioctrl->preloadAudioResources();
    audioctrl->setMusicState(bmusicOn);
    audioctrl->setEffectState(beffectOn);
    
    if(audioctrl->getMusicState() && !SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()){
        audioctrl->playBackgroundMusic();
    }
    
    if (!audioctrl->getMusicState()) {
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
    
    return true;
}


void HomeScene::startGameCallback()
{
    auto scene = MainScene::createScene();
    Director::getInstance()->replaceScene(TransitionCrossFade::create(1.2f, scene));
}

void HomeScene::menuCallback(cocos2d::Ref *pSender)
{
    int tag = dynamic_cast<Node*>(pSender)->getTag();
    
    Scene* scene = NULL;
    
    switch (tag) {
        case 20:    //option scene
            scene = OptionScene::createScene();
            break;
            
        case 21:
            scene = AboutScene::createScene();
            break;
            
        default:
            break;
    }
    
    if (scene != NULL) {
        Director::getInstance()->pushScene(TransitionFade::create(1.2f, scene));
    }
}


void HomeScene::exitCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    RLAudioCtrl::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}