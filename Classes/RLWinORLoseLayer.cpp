//
//  RLWinORLoseLayer.cpp
//  Tour2dx
//
//  Created by Melon on 14-11-24.
//
//

#include "RLWinORLoseLayer.h"
#include "HomeScene.h"

RLWinORLoseLayer* RLWinORLoseLayer::create(bool bVictory, unsigned int curScore, unsigned int curLevel)
{
    RLWinORLoseLayer* layer = new RLWinORLoseLayer();
    if (layer && layer->init(bVictory, curScore, curLevel))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool RLWinORLoseLayer::init(bool bVictory, unsigned int curScore, unsigned int curLevel)
{
    if (!Layer::init()) {
        return false;
    }
    
    this->victory = bVictory;
    this->score = curScore;
    this->currentlevel = curLevel;
    
    Size winSize = Director::getInstance()->getWinSize();
    
    LayerColor* layer1 = LayerColor::create(Color4B(213,208,151,230));
    this->addChild(layer1);
    
    Sprite *bg = Sprite::create("wonorlose.png");
    m_bgSprite = bg;
    bg->setPosition(Point(winSize.width*0.5, winSize.height*0.5));
    this->addChild(bg);
    
    m_bgSize = m_bgSprite->getContentSize();
    
    const float menuFontSize = 22.0;
    const Color3B strColor = Color3B::WHITE;
    
    std::string buttontext = victory?"Continue":"Try again";
    
    lblNextButton = Label::createWithSystemFont(buttontext, "Marker Felt", menuFontSize);
    lblNextButton->setColor(strColor);
    MenuItemLabel* itemResume= MenuItemLabel::create(lblNextButton, CC_CALLBACK_0(RLWinORLoseLayer::nextGameCallback, this));
    
    
    Label* lblExit = Label::createWithSystemFont("Exit", "Marker Felt", menuFontSize);
    lblExit->setColor(strColor);
    MenuItemLabel* itemExit = MenuItemLabel::create(lblExit, CC_CALLBACK_1(RLWinORLoseLayer::exitGameCallback, this));
    
    auto mainMenu = Menu::create(itemResume, itemExit, NULL);
    mainMenu->setPosition(Point(m_bgSize.width*0.5, m_bgSize.height*0.25));
    mainMenu->alignItemsHorizontallyWithPadding(20);
    bg->addChild(mainMenu);
    
    this->setDlgTitle();
    this->setDlgContent();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(RLWinORLoseLayer::onTouchBegan, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void RLWinORLoseLayer::setDlgTitle()
{
    std::string strVal = victory?"Victory":"Defeated";
    Label *title = Label::createWithSystemFont(strVal, "Marker Felt", 23);
    title->setPosition(Point(m_bgSize.width/2, m_bgSize.height- title->getContentSize().height*0.7));
    title->setColor(Color3B(14, 140,228));
    m_bgSprite->addChild(title);
}

void RLWinORLoseLayer::setDlgContent()
{
    std::string strVal = "";
    if (victory) {
        if (this->currentlevel == 9999) {
            strVal = "Great job! You have reached 10000 levels. And score & target will refresh from now.";
        }else{
            strVal = "Congratulations! Now you can get into next round.";
        }
    }else{
        strVal = "Failed. More efforts needed to pass this round! Good luck!";
    }
    
    Label *content = Label::createWithSystemFont(strVal, "Couries New", 16);
    content->setPosition(Point(m_bgSize.width/2, m_bgSize.height*0.5));
    content->setColor(Color3B(248, 248,248));
    content->setDimensions(m_bgSize.width-60, m_bgSize.height-100);
    content->setAlignment(TextHAlignment::LEFT);
    m_bgSprite->addChild(content);
}

bool RLWinORLoseLayer::getVictory(){
    return victory;
}

void RLWinORLoseLayer::setVictory(bool var){
    victory = var;
}

void RLWinORLoseLayer::saveWonResult(RLLevel *level)
{
    RLProgress * progress = RLProgress::create();
    progress->setPattern(level->getPattern());
    progress->setLevel(level->getCurrentLevel()+1);

    progress->setScore(this->score); //score value passed from main scene
    progress->setTargetDone(level->getTargetScore());
    progress->setTargetToGo(progress->getTargetToGo());
    
    log("done: %u  togo:%u", progress->getTargetDone(), progress->getTargetToGo());
    
    if (progress->getLevel() == 10000) {
        log("you have reached 10000 rounds.");
        progress->setScore(0);
        progress->setTargetDone(0);
        progress->setTargetToGo(1000);
    }

    
    bool success = level->saveProgress(progress);
    if (success) {
        //update properties in RLLevel
        level->setCurrentLevel(progress->getLevel());
        level->setScore(progress->getScore());
        level->setTargetScore(progress->getTargetToGo());    //set new target score in coming round
    }
}

void RLWinORLoseLayer::nextGameCallback()
{
    Director::getInstance()->resume();
    
    int iResult = 0;
    if (victory) { //won status - the click will be start next round
        iResult = 100;
        log("won status: %d", iResult);
    }else{ //lose status - the click will be restart that round
        iResult = 200;
        log("lose status: %d", iResult);
    }
    
    NotificationCenter::getInstance()->postNotification(MSG_DLGDONE, (Ref *)iResult);
    this->removeFromParent();
}

void RLWinORLoseLayer::exitGameCallback(cocos2d::Ref *pSender)
{
    Director::getInstance()->resume();
    auto scene = HomeScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
}

bool RLWinORLoseLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    return true;
}