#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    unsigned int ii = 9999 * 1100;
    log("%u", ii);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
 

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("开心对对碰", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    log("test here");
    
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("cookie.plist", "cookie.png");
//    SpriteBatchNode* spriteSheet = SpriteBatchNode::create("cookie.png");
//    this->addChild(spriteSheet);
    
//    Sprite* s1 = Sprite::createWithSpriteFrameName("Cupcake.png");
////    Sprite* s1 = Sprite::create("Cupcake.png");
//    s1->setPosition(100,130);
//    this->addChild(s1);
//    
//    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Croissant-Highlighted.png");
////    Texture2D *texture = frame->getTexture();
////    Director::getInstance()->getTextureCache()->addImage("")
//    
////    Rect rect = frame->getRect();
////    
////    
//    
////    s2->setTexture("Donut.png");
    
    
//    Sprite* s2 = Sprite::createWithSpriteFrame(frame);
//    
//    
//    
//    s2->setPosition(120, 120);
//    s2->setVisible(true);
//    
//    this->addChild(s2);
//    
//    Sprite* s3 = Sprite::create();
////    s3->setTexture(s2->getTexture());
//    s3->setSpriteFrame(frame);
////    s3->updateDisplayedOpacity(100);
//    s3->setPosition(200, 120);
//    this->addChild(s3);
    
    
    return true;
}



void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
