//
//  OptionScene.cpp
//  Tour2dx
//
//  Created by Melon on 14-11-18.
//
//

#include "OptionScene.h"
#include "HomeScene.h"


Scene* OptionScene::createScene()
{
    Scene* scene = Scene::create();
    auto layer = OptionScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OptionScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    Size winSize = Director::getInstance()->getVisibleSize();
    Sprite *bg = Sprite::create("BgAbout.png");
    bg->setPosition(Point(winSize.width*0.5, winSize.height*0.5));
    this->addChild(bg);
    
    const std::string _title = "Options";
    auto titleLabel = Label::createWithBMFont("futura-48.fnt",_title);
    titleLabel->setPosition(Point(winSize.width*0.5, winSize.height*0.85));
    titleLabel->setScale(1.5);
    this->addChild(titleLabel);

    const Color3B titleColor = Color3B(217, 109, 0);
    const Color3B valueColor = Color3B(185,185,201);
    const std::string titleFontName = "American Typewriter";
    const std::string valueFontName = "Marker Felt";
    

    auto title0a = Label::createWithSystemFont("Sound", titleFontName, 20);
    auto item0a = MenuItemLabel::create(title0a);
    item0a->setEnabled(false);
    item0a->setColor(titleColor);


    auto optOn1 = Label::createWithSystemFont("On", valueFontName, 30);
    auto optOff1 = Label::createWithSystemFont("Off", valueFontName, 30);
    
    auto itemOn1 = MenuItemLabel::create(optOn1);
    auto itemOff1 = MenuItemLabel::create(optOff1);
    itemOn1->setColor(valueColor);
    itemOff1->setColor(valueColor);
    
    auto item1 = MenuItemToggle::createWithCallback( CC_CALLBACK_1(OptionScene::menuSoundCallback, this),
                                                    itemOn1,
                                                    itemOff1,
                                                    NULL );
    
    
    if (!RLAudioCtrl::getInstance()->getEffectState()) {
        item1->setSelectedIndex(1);
    }
    
    
    auto title0b = Label::createWithSystemFont("Music", titleFontName, 20);
    auto item0b = MenuItemLabel::create(title0b);
    item0b->setEnabled(false);
    item0b->setColor(titleColor);
    
    auto optOn2 = Label::createWithSystemFont("On", valueFontName, 30);
    auto optOff2 = Label::createWithSystemFont("Off", valueFontName, 30);
    
    auto itemOn2 = MenuItemLabel::create(optOn2);
    auto itemOff2 = MenuItemLabel::create(optOff2);
    itemOn2->setColor(valueColor);
    itemOff2->setColor(valueColor);
    
    auto item2 = MenuItemToggle::createWithCallback(CC_CALLBACK_1(OptionScene::menuMusicCallback, this),
                                                    itemOn2,
                                                    itemOff2,
                                                    NULL );
    item2->setColor(valueColor);
    if (!RLAudioCtrl::getInstance()->getMusicState()) {
        item2->setSelectedIndex(1);
    }
    
    
    int ilevel = UserDefault::getInstance()->getIntegerForKey("pattern", 1);
    std::string txtpattern = StringUtils::format("Scene Pattern: %d", ilevel);
    this->lblPattern = Label::createWithSystemFont(txtpattern, titleFontName, 20);
    auto item3 = MenuItemLabel::create(this->lblPattern);
    item3->setEnabled(false);
    item3->setColor(titleColor);
    
    Label* blank00 = Label::createWithSystemFont(" ", valueFontName, 30);
    auto item00 = MenuItemLabel::create(blank00);
    
    
    Label* blank01 = Label::createWithSystemFont(" ", valueFontName, 40);
    auto item01 = MenuItemLabel::create(blank01);
    
    Label* blank02 = Label::createWithSystemFont(" ", valueFontName, 60);
    auto item02 = MenuItemLabel::create(blank02);
    
    
    MenuItemLabel* patterns[4];
    for (int i = 1; i <= 4; i++) {
        auto ll = Label::createWithSystemFont(StringUtils::format("%d",i), valueFontName, 30);
        ll->setColor(valueColor);
        
        auto bb = MenuItemLabel::create(ll, CC_CALLBACK_1(OptionScene::menuPatternCallback, this));
        bb->setTag(i*10);
        
        patterns[i-1] = bb;
    }
    
    
//    Label *label = Label::createWithBMFont("bitmapFontTest.fnt", "Back");
//    auto back = MenuItemLabel::create(label, CC_CALLBACK_1(OptionScene::menuBackCallback, this) );
//    
//    ActionInterval *fadeIn = FadeTo::create(1, 80);
//    ActionInterval *fadeOut = FadeTo::create(1, 255);
//    EaseSineInOut *ease1 = EaseSineInOut::create(fadeIn);
//    EaseSineInOut *ease2 = EaseSineInOut::create(fadeOut);
//    FiniteTimeAction *seq = CCSequence::create(ease1, ease2, NULL);
//    
//    back->runAction(RepeatForever::create((ActionInterval*)seq));

    
    auto menu = Menu::create(
                             item0a, item0b,
                             item1, item2,
                             item00,
                             item3, item01,
                             patterns[0],patterns[1],patterns[2],patterns[3],
                             item02,
//                             back,
                             NULL ); //10  items.
    
    menu->alignItemsInColumns(2, 2, 1, 2, 4, 1, NULL);
    menu->setPosition(Point(winSize.width/2, winSize.height/2));
    addChild(menu);
    
    
    MenuItemImage *menuBack = MenuItemImage::create("back.png", "backHighlighted.png",CC_CALLBACK_1(OptionScene::menuBackCallback, this));
    menuBack->setPosition(Point(winSize.width*0.8, menuBack->getContentSize().height/2 +10));
    Menu *menu2 = Menu::create(menuBack, NULL);
    menu2->setPosition(Point::ZERO);
    this->addChild(menu2);
    
    return true;
}

void OptionScene::menuPatternCallback(Ref *pSender)
{
    int tag = dynamic_cast<Node*>(pSender)->getTag();
    int pattern = -1;
    
    switch (tag) {
        case 10:
            pattern = 1;
            break;
            
        case 20:
            pattern = 2;
            break;
            
        case 30:
            pattern = 3;
            break;
            
        case 40:
            pattern = 4;
            break;
            
        default:
            break;
    }
    
    if (pattern != -1) {
        std::string txtpattern = StringUtils::format("Scene Pattern: %d", pattern);
        this->lblPattern->setString(txtpattern);
    }
    
    UserDefault::getInstance()->setIntegerForKey("pattern", pattern);
}

/** toggle sound effect playing
 */
void OptionScene::menuSoundCallback(cocos2d::Ref *pSender)
{
    bool isEffectOn = RLAudioCtrl::getInstance()->getEffectState();
    
    if (isEffectOn) { //effect on -> off
        SimpleAudioEngine::getInstance()->pauseAllEffects();
        RLAudioCtrl::getInstance()->setEffectState(false);
        UserDefault::getInstance()->setBoolForKey("iseffecton", false);

    }else{// effect off -> on
        SimpleAudioEngine::getInstance()->resumeAllEffects();
        RLAudioCtrl::getInstance()->setEffectState(true);
        UserDefault::getInstance()->setBoolForKey("iseffecton", true);
    }
}

/** toggle background music playing
 */
void OptionScene::menuMusicCallback(cocos2d::Ref *pSender)
{
    bool isMusicOn = RLAudioCtrl::getInstance()->getMusicState();
    
    if (isMusicOn) {//music on -> off
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
        RLAudioCtrl::getInstance()->setMusicState(false);
        UserDefault::getInstance()->setBoolForKey("ismusicon", false);
    }else{//music off -> on
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
        if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
            RLAudioCtrl::getInstance()->playBackgroundMusic();
        }
        RLAudioCtrl::getInstance()->setMusicState(true);
        UserDefault::getInstance()->setBoolForKey("ismusicon", true);
    }
}

/** go back to home scene */
void OptionScene::menuBackCallback(cocos2d::Ref *pSender)
{
    Director::getInstance()->popScene();
}
