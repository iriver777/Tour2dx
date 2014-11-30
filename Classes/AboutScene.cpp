//
//  AboutScene.cpp
//  Tour2dx
//
//  Created by Melon on 14-11-9.
//
//

#include "AboutScene.h"


Scene* AboutScene::createScene()
{
    auto scene = Scene::create();
    auto layer = AboutScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool AboutScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();

    Sprite *bg = Sprite::create("BgAbout.png");
    bg->setPosition(Point(winSize.width*0.5, winSize.height*0.5));
    this->addChild(bg);
    
    MenuItemImage *menuBack = MenuItemImage::create("back.png", "backHighlighted.png",CC_CALLBACK_1(AboutScene::goBack, this));
    menuBack->setPosition(Point(winSize.width*0.8, menuBack->getContentSize().height/2 +10));
    
    Menu* menu = Menu::create(menuBack, NULL);
    menu->setPosition(Point::ZERO);
    
    this->addChild(menu);
    
    float menuBackHeight = menuBack->getContentSize().height;
    LayerColor* layer1 = LayerColor::create(Color4B(227,231,236,127));
    layer1->setPosition(winSize.width*0.05, menuBackHeight + 20);
    Size contentSize = Size(winSize.width*0.9, winSize.height - 2*menuBackHeight - 20);
    layer1->setContentSize(contentSize);
    this->addChild(layer1);

    
    std::string strVal = "Game Rule\nDifferent fruits can be swapped so as to the more than 3 fruits with the same category could meet and crush. Each round can be passed after reaching the target of it.\n\n\nHow to\nFour different scenes are offerred in the game. And player can choose any one of them on the option scene.";
    Label *content = Label::createWithSystemFont(strVal, "Couries New", 16);
    content->setPosition(Point(contentSize.width*0.5, contentSize.height*0.5));
    content->setColor(Color3B(83, 148,200));
    content->setDimensions(contentSize.width-40, contentSize.height-40);
    content->setAlignment(TextHAlignment::LEFT);
    
    layer1->addChild(content);
   
//    float lblHeight =content->getContentSize().height;
//    log("content Size: (w)%f (h)%f", contentSize.width, contentSize.height);
//    log("label Size: (w)%f (h)%f", content->getContentSize().width, lblHeight);
    
    return true;
}


void AboutScene::goBack(Ref *pSender)
{
    Director::getInstance()->popScene();
}