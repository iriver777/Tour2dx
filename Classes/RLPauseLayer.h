//
//  RLPauseLayr.h
//  Tour2dx
//
//  Created by Melon on 14-11-20.
//
//

#ifndef __Tour2dx__RLPauseLayer__
#define __Tour2dx__RLPauseLayer__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

class RLPauseLayer: public cocos2d::Layer{
public:
    virtual bool init();
	CREATE_FUNC(RLPauseLayer);

private:
    void resumeGameCallback();
    void exitGameCallback(Ref* pSender);
    bool onTouchBegan(Touch* touch, Event *event);
};


#endif /* defined(__Tour2dx__RLPauseLayer__) */
