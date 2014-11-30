//
//  MainScene.h
//  Tour2dx
//
//  Created by Melon on 14-11-9.
//
//

#ifndef __TOUR2DX__MAINSCENE__
#define __TOUR2DX__MAINSCENE__

#include "cocos2d.h"
#include "RLLevel.h"
#include "RLCookie.h"
#include "RLSwap.h"
#include "RLAudioCtrl.h"
#include "RLHUDLayer.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

static const float TileWidth = 32.0;
static const float TileHeight = 36.0;
static const int INTMINUSONE = -1;

class MainScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
    
    ~MainScene();
    
    CC_SYNTHESIZE(RLLevel *, level, Level);
    CC_SYNTHESIZE(Layer *, gamelayer, GameLayer);
    CC_SYNTHESIZE(Layer *, cookieslayer, CookiesLayer);
    CC_SYNTHESIZE(Layer *, tileslayer, TilesLayer);
    CC_SYNTHESIZE(RLHUDLayer *, hudLayer, HUDLayer);
    
    //cookie being selected currently
    CC_SYNTHESIZE(Sprite*, selSprite, SelectionSprite);
    
    CC_SYNTHESIZE(RLAudioCtrl*, audioctrl, AudioCtrl);
    
    void addSpritesForCookies(const Vector<RLCookie*> &cookies);
    
    void handleWonORLose(Ref* pData);
    
private:
    Point pointForColumnAndRow(int col, int row);
    bool convertPointToColumnAndRow(Point point, int& col, int& row);
    void beginGame();
    void shuffle();
    SpriteBatchNode *spriteSheet;
    void addTiles();
    void trySwapHorzOrVert(int horzDelta, int vertDelta);
    
    //swipe
    int swipeFromColumn;
    int swipeFromRow;
    
    //touch event
    void onTouchesBegan(const std::vector<Touch*>& touches, Event *event);
    void onTouchesMoved(const std::vector<Touch*>& touches, Event *event);
    void onTouchesEnded(const std::vector<Touch*>& touches, Event *event);
    void onTouchesCancelled(const std::vector<Touch*>& touches, Event *event);
    
    //callback func
    std::function<void(RLSwap* swap)> swipeHandler;
    
    //animation
    
    void showSelectionIndicatorForCookie(RLCookie* cookie);
    void hideSelectionIndicator();
    
    void animateSwap(RLSwap* swap, std::function<void()> completion);
    void animateInvalidSwap(RLSwap* swap, std::function<void()> completion);
    void animateMatchedCookies(Vector<RLChain*> &chains, std::function<void()> completion);
    void animateFallingCookies(Vector<RLChain*> &columns,std::function<void()> completion);
    void animateNewCookies(Vector<RLChain*> &columns,std::function<void()> completion);
    void animateScoreForChain(RLChain * chain);
    
    //audio effect
    void preloadAudioResources();
    
    //match
    void handleMatches();
    
    void beginNextTurn();
    
    
    //hud display
    unsigned int curlevel;
    unsigned int score;
    unsigned int movesleft;
    void updateLabels();
    
    void decrementMoves();
    
    //ui button
    ControlButton *standardButtonWithTitle(const char * title);
    void addCtrlButtons();

//    void touchupShuffleAction(Ref *pSender, Control::EventType controlEvent);
//    void touchupPauseAction(Ref *pSender, Control::EventType controlEvent);
    void touchShuffle(Ref* pSender);
    void touchPause(Ref* pSender);

};

#endif //__TOUR2DX__MAINSCENE__
