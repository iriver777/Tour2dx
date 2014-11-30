//
//  MainScene.cpp
//  Tour2dx
//
//  Created by Melon on 14-11-9.
//
//

#include "MainScene.h"
#include "RLPauseLayer.h"
#include "RLWinORLoseLayer.h"

USING_NS_CC;
using namespace std;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

MainScene::~MainScene()
{
    CC_SAFE_DELETE(this->level);
    
//    if (this->selSprite != NULL) {
//        this->selSprite->release();
//    }
    
    //unsubscribe the winorlose message
    NotificationCenter::getInstance()->removeObserver(this, MSG_DLGDONE);
}

bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
//    Point origin = Director::getInstance()->getVisibleOrigin();
    
//    auto label1 = LabelTTF::create("Fuck","Arial",30);
//    label1->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
//    this->addChild(label1);
    
    //init sprite sheet
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("cookie.plist","cookie.png");
    spriteSheet = SpriteBatchNode::create("cookie.png");

    
    this->level = RLLevel::create();
    this->level->retain();
    this->level->initProgressFile();
    
    int iPattern = UserDefault::getInstance()->getIntegerForKey("pattern", 1);
    this->level->setPattern(iPattern);
    std::string filelevel = StringUtils::format("Level_%d.json", iPattern);

    this->level->initWithJSONFile(filelevel);
    
//    this->setAnchorPoint(Point(0.5, 0.5));
    
    Sprite *bg = Sprite::create("Background.png");
    bg->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    this->addChild(bg);
    
    //add HUD layer
    this->hudLayer = RLHUDLayer::create();
    this->addChild(this->hudLayer);
    
    
    this->gamelayer = Layer::create();
    this->addChild(this->gamelayer);
    
    Point layerPos = Point((visibleSize.width - TileWidth*NumColumns)/2, (visibleSize.height-TileHeight*NumRows)*0.48);

    this->cookieslayer = Layer::create();
    this->cookieslayer->setPosition(layerPos);
    
    //add tileslayer before adding spriteSheet
    this->tileslayer = Layer::create();
    this->cookieslayer->addChild(this->tileslayer);
    
    this->addTiles();
    
    this->cookieslayer->addChild(spriteSheet);  //take spriteSheet as child of cookielayer
    
    this->gamelayer->addChild(this->cookieslayer);
    
    
    this->addCtrlButtons(); //add bottom control buttons
    
    this->swipeFromColumn = INTMINUSONE;
    this->swipeFromRow = INTMINUSONE;
    
    
    //highlighted cookie
    Sprite* sel = Sprite::create();
    this->setSelectionSprite(sel);
    this->addChild(this->selSprite);
    
    auto block = [&](RLSwap* swap){
//        this->cookieslayer->setTouchEnabled(false);
        log("block starting...");
        
        if (this->level->isPossibleSwap(swap)) {
            this->level->performSwap(swap);
            this->animateSwap(swap, [&](){
                this->handleMatches();
//            this->cookieslayer->setTouchEnabled(true);
            log("block ended");
            });
        }else{
//            this->cookieslayer->setTouchEnabled(true);
            this->animateInvalidSwap(swap, [&]{
                log("block ended");
            });
            
        }

    };
    
    this->swipeHandler = block;
    
    //touch event
    auto listener = EventListenerTouchAllAtOnce::create();
    
    listener->onTouchesBegan = CC_CALLBACK_2(MainScene::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(MainScene::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(MainScene::onTouchesEnded, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(MainScene::onTouchesCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //load music
    this->setAudioCtrl(RLAudioCtrl::getInstance());
    
    //subscribe the message from RLWinORLoseLayer
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainScene::handleWonORLose), MSG_DLGDONE, NULL);
    
    //start the game
    this->beginGame();

    return true;
}

void MainScene::addCtrlButtons()
{
    /*ControlButton* btnShuffle = standardButtonWithTitle("Shuffle");
    ControlButton* btnPause = standardButtonWithTitle("Pause");
    
    btnPause->addTargetWithActionForControlEvents(this, cccontrol_selector(MainScene::touchupPauseAction), Control::EventType::TOUCH_UP_INSIDE);
    
    btnShuffle->addTargetWithActionForControlEvents(this, cccontrol_selector(MainScene::touchupShuffleAction), Control::EventType::TOUCH_UP_INSIDE);
    
    Point p1 = this->cookieslayer->getBoundingBox().origin;
    Point p2 = this->gamelayer->getBoundingBox().origin;
    
    float preferW = btnShuffle->getContentSize().width + 4;
    float preferH = btnShuffle->getContentSize().height + 4;
    
    float yoffset = (p1.y - p2.y)/2;
    float x1offset = p1.x + preferW/2 + 2*TileWidth;
    btnShuffle->setPreferredSize(Size(preferW, preferH));
    btnShuffle->setPosition(Point(x1offset, yoffset));
    
    x1offset += preferW + TileWidth;
    btnPause->setPreferredSize(Size(preferW, preferH));
    btnPause->setPosition(Point(x1offset, yoffset));
    
    
    this->gamelayer->addChild(btnShuffle);
    this->gamelayer->addChild(btnPause);*/
    
    //use image instead of button Nov 28, 2014
    MenuItemImage *menuShuffle = MenuItemImage::create("btnrefresh.png", "btnrefreshHighlighted.png",
                                                       CC_CALLBACK_1(MainScene::touchShuffle, this));
    MenuItemImage *menuPause = MenuItemImage::create("btnpause.png", "btnpauseHighlighted.png",
                                                       CC_CALLBACK_1(MainScene::touchPause, this));
    
    
    Point p1 = this->cookieslayer->getBoundingBox().origin;
    Point p2 = this->gamelayer->getBoundingBox().origin;
    
    float yoffset = (p1.y - p2.y)/2;
    float xoffset = p1.x + NumColumns*TileWidth/2;
    
    Menu* menu = Menu::create(menuShuffle, menuPause, NULL);
    menu->alignItemsHorizontallyWithPadding(TileWidth*2);
    menu->setPosition(Point(xoffset, yoffset));
    
    this->gamelayer->addChild(menu);

}

void MainScene::touchShuffle(cocos2d::Ref *pSender){
    this->shuffle();
    this->decrementMoves();
}

void MainScene::touchPause(cocos2d::Ref *pSender){
    Director::getInstance()->pause();
    
    if (RLAudioCtrl::getInstance()->getMusicState()) {
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
    
    if (RLAudioCtrl::getInstance()->getEffectState()) {
        SimpleAudioEngine::getInstance()->pauseAllEffects();
    }
    
    RLPauseLayer* pauseLayer = RLPauseLayer::create();
    this->addChild(pauseLayer, 999);
}

ControlButton *MainScene::standardButtonWithTitle(const char * title)
{
    /** Creates and return a button with a default background and title color. */
    auto backgroundButton = Scale9Sprite::create("button.png");
    auto backgroundHighlightedButton = Scale9Sprite::create("buttonHighlighted.png");
    
    auto titleButton = Label::createWithTTF(title, "Marker Felt.ttf", 18);
    titleButton->setColor(Color3B::BLACK);
    
    ControlButton *button = ControlButton::create(titleButton, backgroundButton);
    button->setBackgroundSpriteForState(backgroundHighlightedButton, Control::State::HIGH_LIGHTED);
    button->setTitleColorForState(Color3B::WHITE, Control::State::HIGH_LIGHTED);
    
    return button;
}


/*void MainScene::touchupPauseAction(cocos2d::Ref *pSender, Control::EventType controlEvent)
{
    Director::getInstance()->pause();
    
    if (RLAudioCtrl::getInstance()->getMusicState()) {
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
    
    if (RLAudioCtrl::getInstance()->getEffectState()) {
        SimpleAudioEngine::getInstance()->pauseAllEffects();
    }
    
    RLPauseLayer* pauseLayer = RLPauseLayer::create();
    this->addChild(pauseLayer, 999);
    
}

void MainScene::touchupShuffleAction(cocos2d::Ref *pSender, Control::EventType controlEvent)
{
    
    this->shuffle();
    this->decrementMoves();
}*/

void MainScene::addSpritesForCookies(const Vector<RLCookie *> &cookies)
{
    for (RLCookie* c : cookies) {
        Sprite *sprite = Sprite::createWithSpriteFrameName(c->spriteName());
        sprite->setPosition(this->pointForColumnAndRow(c->getColumn(), c->getRow()));
        spriteSheet->addChild(sprite);
        
        c->setSprite(sprite);
    }
    
}

Point MainScene::pointForColumnAndRow(int col, int row)
{
    return Point(col*TileWidth + TileWidth/2, row*TileHeight + TileHeight/2);
}

bool MainScene::convertPointToColumnAndRow(Point point, int& col, int& row)
{
    if (point.x >= 0 && point.x < NumColumns*TileWidth && point.y >= 0 && point.y < NumRows*TileHeight) {
        col = point.x / TileWidth;
        row = point.y / TileHeight;
        return true;
    }else{
        col = INTMINUSONE;
        row = INTMINUSONE;
        return false;
    }
}

void MainScene::shuffle()
{
    Vector<RLCookie *> mycookies = this->level->shuffle();
    this->addSpritesForCookies(mycookies);
}

void MainScene::beginGame()
{
//    this->score = 0;
    this->score = this->level->getScore();  //use history accumulated score as start score of current round
    this->movesleft = this->level->getMaxMoves();
    
//    just for test
//    this->movesleft = 2;
//    this->score += 900;
    
    this->updateLabels();
    
    this->shuffle();
}

void MainScene::addTiles()
{
    for (int row = 0; row<NumRows; row++) {
        for (int col = 0; col<NumColumns; col++) {
            if (this->level->tileAtColumnAndRow(col, row) != NULL) {
                Sprite* spriteTile = Sprite::createWithSpriteFrameName("Tile.png");
                spriteTile->setPosition(Point(this->pointForColumnAndRow(col, row)));
                this->tileslayer->addChild(spriteTile);
            }
        }
    }
}

void MainScene::onTouchesBegan(const std::vector<Touch*>& touches, Event *event)
{
    std::vector<Touch*>::const_iterator touchIter = touches.begin();
    Touch* ptouch = (Touch*)(*touchIter);
    Point loc = ptouch->getLocation();
    
    Point location = this->cookieslayer->convertToNodeSpace(loc);
    log("on TouchesBegan");
    log("loc in (%f,%f) -> (%f,%f)",loc.x, loc.y, location.x, location.y);
    
    int col, row;
    bool inCookiesFlag = false;
    
    inCookiesFlag = this->convertPointToColumnAndRow(location, col, row);
    log("touch tile(col,row): (%d,%d)",col, row);
    
    if (inCookiesFlag) {
        RLCookie* cookie = this->level->cookieAtColumnAndRow(col, row);
        if (cookie != NULL) {
            this->swipeFromColumn = col;
            this->swipeFromRow = row;
            
            this->showSelectionIndicatorForCookie(cookie);  //highlighted selection
        }
    }
}

void MainScene::onTouchesMoved(const std::vector<Touch*>& touches, Event *event)
{
    if (this->swipeFromColumn == INTMINUSONE) {
        return;
    }
    
    std::vector<Touch*>::const_iterator touchIter = touches.begin();
    Touch* ptouch = (Touch*)(*touchIter);
    Point loc = ptouch->getLocation();
    
    Point location = this->cookieslayer->convertToNodeSpace(loc);
    log("on TouchesMoved");
//    log("loc in (%f,%f) -> (%f,%f)",loc.x, loc.y, location.x, location.y);
    
    int col, row;
    bool inCookiesFlag = false;
    
    inCookiesFlag = this->convertPointToColumnAndRow(location, col, row);
    log("touch tile(col,row): (%d,%d)",col, row);
    
    if (inCookiesFlag) {
        int horzDelta = 0, vertDelta = 0;
        if (col < this->swipeFromColumn) {
            horzDelta = -1;
        } else if(col > this->swipeFromColumn){
            horzDelta = 1;
        } else if(row < this->swipeFromRow){
            vertDelta = -1;
        } else if(row > this->swipeFromRow){
            vertDelta = 1;
        }
        
        if (horzDelta != 0 || vertDelta != 0) {
            //swap tiles
            this->trySwapHorzOrVert(horzDelta, vertDelta);
            
            //hide highlighted cookie
            this->hideSelectionIndicator();
            
            this->swipeFromColumn = INTMINUSONE;
        }
    }
}

void MainScene::onTouchesEnded(const std::vector<Touch*>& touches, Event *event)
{
    if (this->selSprite->getParent() != nullptr && this->swipeFromColumn != INTMINUSONE) {
        this->hideSelectionIndicator();
    }
    this->swipeFromColumn = this->swipeFromRow = INTMINUSONE;
}

void MainScene::onTouchesCancelled(const std::vector<Touch*>& touches, Event *event)
{
    this->onTouchesEnded(touches, event);
}

//swap cookies
void MainScene::trySwapHorzOrVert(int horzDelta, int vertDelta)
{
    int toColumn = this->swipeFromColumn + horzDelta;
    int toRow = this->swipeFromRow + vertDelta;
    
    if (toColumn < 0 || toColumn >= NumColumns || toRow < 0 || toRow >= NumRows) {
        return;
    }
    
    RLCookie* toCookie = this->level->cookieAtColumnAndRow(toColumn, toRow);
    if (toCookie == NULL) return;
    
    RLCookie* fromCookie = this->level->cookieAtColumnAndRow(this->swipeFromColumn, this->swipeFromRow);
    
    if (this->swipeHandler != nullptr) {
        RLSwap* swap = RLSwap::create();
        swap->setCookieA(fromCookie);
        swap->setCookieB(toCookie);
        
        this->swipeHandler(swap);
    }

}

//highlighted selection
void MainScene::showSelectionIndicatorForCookie(RLCookie *cookie)
{
    if (this->getSelectionSprite()->getParent() != NULL) {
        this->selSprite->retain();
        this->selSprite->removeFromParent();
    }
    
//    SpriteFrame *frame = SpriteFrame::create(cookie->highlightedSpriteName(), cookie->getSprite()->getTextureRect());
//    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Danish-Highlighted.png");
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(cookie->highlightedSpriteName());
    
//    Texture2D *texture = frame->getTexture();
//    Size size = texture->getContentSize();
//    log("size: w(%f) h(%f)", size.width, size.height);
//    this->selSprite->setTexture(texture);
    
    this->selSprite->setSpriteFrame(frame);
    this->selSprite->setAnchorPoint(Point(0,0));
    cookie->getSprite()->addChild(this->selSprite);
    this->selSprite->setOpacity(255);
}

void MainScene::hideSelectionIndicator()
{
    Action *actions = Sequence::create(FadeOut::create(0.3),
                                CallFunc::create([&]{this->selSprite->removeFromParent();
                                }), NULL);
    this->selSprite->runAction(actions);
}

void MainScene::updateLabels()
{
    this->hudLayer->setTargetVal(StringUtils::format("%lu", (long)this->level->getTargetScore()));
    this->hudLayer->setLevelVal(StringUtils::format("%u",this->level->getCurrentLevel()));
    this->hudLayer->setMovesVal(StringUtils::format("%lu", (long)this->movesleft));
    this->hudLayer->setScoreVal(StringUtils::format("%lu", (long)this->score));
}

void MainScene::decrementMoves()
{
    this->movesleft--;
    this->updateLabels();
    
    int curStatus = 10;
    if (this->score >= this->level->getTargetScore()){
        curStatus = 20; //win
    }else if (this->movesleft <= 0){
        curStatus = 30; //lose
    }
    
    if (curStatus == 20 || curStatus == 30) {
        Director::getInstance()->pause();
        bool flag = (curStatus==20)?true:false;
        RLWinORLoseLayer* winLayer = RLWinORLoseLayer::create(flag, this->score, this->level->getCurrentLevel());

        if (curStatus == 20)
            winLayer->saveWonResult(this->level);   //save victory info when passing the current round
        
        this->addChild(winLayer, 999);

    }
    
}

/**
 @pData 100 - win and next round 200 - lose and try again
 */
void MainScene::handleWonORLose(Ref* pData)
{
    int iNext = (int)pData;
    if (iNext == 100 || iNext == 200) {
        this->beginGame();
    }
}

void MainScene::beginNextTurn()
{
    this->level->detectPossibleSwaps();
//    this->setTouchEnabled(true);
    
    this->decrementMoves();
}

//handle matches
void MainScene::handleMatches()
{
    Vector<RLChain*> chains = this->level->removeMatches();
    
    if (chains.size() == 0) {
        this->beginNextTurn();
        
        return;
    }
    
    this->animateMatchedCookies(chains, [=]{
        log("matches removed");
        
        for (const RLChain * chain : chains) {
            this->score += chain->getScore();
        }
        this->updateLabels();
        
        
        Vector<RLChain*> fallingcookies = this->level->fillHoles();
        this->animateFallingCookies(fallingcookies, [=]{
            log("filling completed");
            
            Vector<RLChain*> newcookies = this->level->topUpCookies();
            this->animateNewCookies(newcookies, [this]{
                log("new cookies generated");
                
                //recursion handle for cascade removing
                this->handleMatches();
            });
        });
    });
}

//animation for swap
void MainScene::animateSwap(RLSwap* swap, std::function<void()> completion)
{
    swap->getCookieA()->getSprite()->setZOrder(100);
    swap->getCookieB()->getSprite()->setZOrder(90);
    
    const float duration = 0.3f;
    
    MoveTo* moveA = MoveTo::create(duration, swap->getCookieB()->getSprite()->getPosition());
//    swap->getCookieA()->getSprite()->runAction(moveA);
    CallFunc* callfunc = CallFunc::create(completion);
    Action* actions = Sequence::create(moveA, callfunc, NULL);
    swap->getCookieA()->getSprite()->runAction(actions);
    
    
    MoveTo* moveB = MoveTo::create(duration, swap->getCookieA()->getSprite()->getPosition());
    swap->getCookieB()->getSprite()->runAction(moveB);
    
    if (this->audioctrl->getEffectState()) {
        this->audioctrl->playSoundEffect(SoundSwap);
    }
}

void MainScene::animateInvalidSwap(RLSwap* swap, std::function<void()> completion)
{
    swap->getCookieA()->getSprite()->setZOrder(100);
    swap->getCookieB()->getSprite()->setZOrder(90);
    
    const float duration = 0.3f;

    MoveTo* moveA = MoveTo::create(duration, swap->getCookieB()->getSprite()->getPosition());
    MoveTo* moveB = MoveTo::create(duration, swap->getCookieA()->getSprite()->getPosition());
    
    swap->getCookieA()->getSprite()->runAction(Sequence::create(moveA,moveB,
                                                                CallFunc::create(completion), NULL));
    
    swap->getCookieB()->getSprite()->runAction(Sequence::create(moveB, moveA, NULL));
    
    if (this->audioctrl->getEffectState()) {
        this->audioctrl->playSoundEffect(SoundInvalidSwap);
    }
}

void MainScene::animateMatchedCookies(Vector<RLChain *> &chains, std::function<void ()> completion)
{
    Vector<RLChain *>::iterator itr;
    for (itr = chains.begin(); itr != chains.end(); itr++) {
        RLChain* chain = (*itr);
        Vector<RLCookie*> cookies = chain->getCookies();
        
        this->animateScoreForChain(chain);
        
        Vector<RLCookie *>::iterator itr_ck;
        for (itr_ck = cookies.begin(); itr_ck != cookies.end(); itr_ck++) {
            RLCookie* cookie = (*itr_ck);

            
            if (cookie->getSprite() != NULL) {
                FadeOut *fadeout = FadeOut::create(0.2f);
                
                Action *action = Sequence::create(fadeout,
                                                  CallFunc::create([cookie]{cookie->getSprite()->removeFromParent();}), NULL);
                cookie->getSprite()->runAction(action);

            }
        }
    }
    
    //play audio effect of matching
    if (this->audioctrl->getEffectState()) {
        this->audioctrl->playSoundEffect(SoundMatch);
    }
    
    
    this->runAction(Sequence::create(DelayTime::create(0.3), CallFunc::create(completion), NULL));
    
}

void MainScene::animateFallingCookies(Vector<RLChain*> &columns,std::function<void()> completion)
{
    float longestDuration = 0.0f;
    
    Vector<RLChain*>::iterator itr_cols;
    for (itr_cols=columns.begin(); itr_cols!=columns.end(); itr_cols++) {
        RLChain* col = (*itr_cols);
        Vector<RLCookie*> cookies = col->getCookies();
        
        Vector<RLCookie*>::iterator itr_col;
        for (itr_col = cookies.begin(); itr_col != cookies.end(); itr_col++) {
            RLCookie* cookie = (*itr_col);
            
            Point newPos = this->pointForColumnAndRow(cookie->getColumn(), cookie->getRow());
            float delay = 0.05 + 0.1*cookies.getIndex((*itr_col));
            
            float duration = ((cookie->getSprite()->getPosition().y - newPos.y)/TileHeight)*0.1;
            
            longestDuration = MAX(longestDuration, duration+delay);
            
            MoveTo *moveto = MoveTo::create(duration, newPos);
            
            cookie->getSprite()->runAction(Sequence::create(DelayTime::create(delay),
                                                            moveto,
                                                            CallFunc::create([this]{
                                                                if (this->audioctrl->getEffectState())
                                                                    this->audioctrl->playSoundEffect(SoundFalling);
                                                            }),
                                                            NULL));
            
//            auto func = [=,&longestDuration](){
//                Point newPos = this->pointForColumnAndRow(cookie->getColumn(), cookie->getRow());
//                float delay = 0.05 + 0.1*cookies.getIndex((*itr_col));
//                
//                float duration = ((cookie->getSprite()->getPosition().y - newPos.y)/TileHeight)*0.1;
//                
//                longestDuration = MAX(longestDuration, duration+delay);
//                
//                MoveTo *moveto = MoveTo::create(duration, newPos);
//                
//                cookie->getSprite()->runAction(Sequence::create(
//                                                DelayTime::create(delay),
//                                                moveto,
//                                                CallFunc::create([this]{
//                                                if (this->audioctrl->getEffectState())
//                                                    this->audioctrl->playSoundEffect(SoundFalling);
//                                                }),
//                                                NULL));
//            };
//            cookie->getSprite()->runAction(CallFunc::create(func));
        }
    }
    
    this->runAction(Sequence::create(DelayTime::create(longestDuration),
                                     CallFunc::create(completion), NULL));
}

void MainScene::animateNewCookies(Vector<RLChain*> &columns,std::function<void()> completion)
{
    float longestDuration = 0.0f;
    
    //loop column in all columns with updated cookies
    for (RLChain* col : columns){
        Vector<RLCookie*> cookies = col->getCookies();
        
        int startRow = (*cookies.begin())->getRow() + 1;
        
        //loop cookie in each column
        for (RLCookie* cookie : cookies){
            
            Sprite* sprite = Sprite::createWithSpriteFrameName(cookie->spriteName());
            //The new cookie sprite should start out just above the first tile in this column
            sprite->setPosition(this->pointForColumnAndRow(cookie->getColumn(), startRow));
            this->spriteSheet->addChild(sprite);
            cookie->setSprite(sprite);

            int idx = cookies.getIndex(cookie);
            float delay = 0.1 + 0.1*(cookies.size() - idx - 1);

            float duration = (startRow - cookie->getRow())*0.1;

            longestDuration = MAX(longestDuration, duration+delay);

            Point newPos = this->pointForColumnAndRow(cookie->getColumn(), cookie->getRow());

            MoveTo *moveto = MoveTo::create(duration, newPos);
            
            cookie->getSprite()->runAction(Sequence::create(DelayTime::create(delay),
                                                            CallFunc::create([this]{
                                                                if (this->audioctrl->getEffectState())
                                                                    this->audioctrl->playSoundEffect(SoundAddCookie);
                                                            }),
                                                            moveto, NULL));
            
//            auto func = [=,&longestDuration](){
//                
//                Sprite* sprite = Sprite::createWithSpriteFrameName(cookie->spriteName());
//                //The new cookie sprite should start out just above the first tile in this column
//                sprite->setPosition(this->pointForColumnAndRow(cookie->getColumn(), startRow));
//                this->spriteSheet->addChild(sprite);
//                cookie->setSprite(sprite);
//                
//                int idx = cookies.getIndex(cookie);
//                float delay = 0.1 + 0.1*(cookies.size() - idx - 1);
//                
//                float duration = (startRow - cookie->getRow())*0.1;
//                
//                longestDuration = MAX(longestDuration, duration+delay);
//                
//                Point newPos = this->pointForColumnAndRow(cookie->getColumn(), cookie->getRow());
//                
//                MoveTo *moveto = MoveTo::create(duration, newPos);
//                
//                cookie->getSprite()->runAction(Sequence::create(DelayTime::create(delay),
//                                                                CallFunc::create([this]{
//                                                                    if (this->audioctrl->getEffectState())
//                                                                        this->audioctrl->playSoundEffect(SoundAddCookie);
//                                                                }),
//                                                                moveto, NULL));
//            };
//            cookie->getSprite()->runAction(CallFunc::create(func));
        }
    }

    this->runAction(Sequence::create(DelayTime::create(longestDuration),
                                     CallFunc::create(completion), NULL));
}


void MainScene::animateScoreForChain(RLChain *chain)
{
    Vector<RLCookie *> cookies = chain->getCookies();
    if (cookies.empty()) return;
    
    RLCookie * firstObj = cookies.at(0);
    RLCookie * lastObj = cookies.at(cookies.size() - 1);
    
    Point centrePos = Point((firstObj->getSprite()->getPositionX() + lastObj->getSprite()->getPositionX())/2,
                            (firstObj->getSprite()->getPositionY() + lastObj->getSprite()->getPositionY())/2 - 8);
    
    auto scoreLabel = Label::createWithBMFont("futura-48.fnt",
                                              StringUtils::format("%lu",(long)chain->getScore()));
    
    scoreLabel->setZOrder(400);
    scoreLabel->setPosition(centrePos);
    this->cookieslayer->addChild(scoreLabel);
    
    MoveBy* moveby = MoveBy::create(1.0f, Point(0, 2));
    FadeOut* fadeout = FadeOut::create(1.0f);
    FiniteTimeAction * actions = Spawn::create(moveby, fadeout, NULL);
    scoreLabel->runAction(Sequence::create(actions,
                                           CallFunc::create([scoreLabel](){
                                                scoreLabel->removeFromParent();
                                            }),
                                           NULL));
    
    
    
}