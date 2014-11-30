//
//  RLWinORLoseLayer.h
//  Tour2dx
//
//  Created by Melon on 14-11-24.
//
//

#ifndef __TOUR2DX__RLWINORLOSELAYER__
#define __TOUR2DX__RLWINORLOSELAYER__

#include "cocos2d.h"
#include "RLLevel.h"

USING_NS_CC;

#define MSG_DLGDONE "MSGNEXTORTRY"

class RLWinORLoseLayer: public cocos2d::Layer{
public:
    static RLWinORLoseLayer* create(bool bVictory, unsigned int curScore, unsigned int curLevel);
    bool init(bool bVictory, unsigned int curScore, unsigned int curLevel);
    CC_PROPERTY(bool, victory, Victory);
    CC_SYNTHESIZE(unsigned int, score, Score);  //score by far, whether won or lose
    CC_SYNTHESIZE(unsigned int, currentlevel, CurrentLevel);    //level passed from mainscene
    
    void saveWonResult(RLLevel *level);
    
private:
    void nextGameCallback();
    void exitGameCallback(Ref* pSender);
    bool onTouchBegan(Touch* touch, Event *event);
    
    Sprite *m_bgSprite;
    Size m_bgSize;
    Label* lblNextButton;
    void setDlgTitle();
    void setDlgContent();
};

#endif /* defined(__TOUR2DX__RLWINORLOSELAYER__) */
