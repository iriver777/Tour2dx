//
//  RLProgress.h
//  Tour2dx
//
//  Created by Melon on 14-11-23.
//
//

#ifndef __TOUR2DX__RLPROGRESS__
#define __TOUR2DX__RLPROGRESS__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class RLProgress:public Ref {
public:
    static RLProgress* create();
    
    CC_SYNTHESIZE(int, pattern, Pattern);
    CC_SYNTHESIZE(unsigned int, level, Level);
    CC_SYNTHESIZE(unsigned int, score, Score);
    CC_PROPERTY(unsigned int, targetToGo, TargetToGo);
    CC_SYNTHESIZE(unsigned int, targetDone, TargetDone);

private:
    unsigned int calcTargetToGo(unsigned int score);
};

#endif /* defined(__TOUR2DX__RLPROGRESS__) */
