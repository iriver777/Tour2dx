//
//  RLChain.h
//  Tour2dx
//
//  Created by Melon on 14-11-15.
//
//

#ifndef __TOUR2DX__RLCHAIN__
#define __TOUR2DX__RLCHAIN__

#include "cocos2d.h"
#include "RLCookie.h"


USING_NS_CC;

typedef enum {
    ChainTypeHorizontal,
    ChainTypeVertical
} ChainType;

class RLChain:public Ref {
private:
    Vector<RLCookie*> _cookies;
    
public:
    static RLChain* create();
    CC_SYNTHESIZE(ChainType, chaintype, ChainType)
    CC_PROPERTY_READONLY(Vector<RLCookie*>, cookies, Cookies);
    CC_SYNTHESIZE(unsigned int, score, Score);
    
    void addCookie(RLCookie* cookie);
};
#endif /* defined(__TOUR2DX__RLCHAIN__) */
