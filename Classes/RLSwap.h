//
//  RLSwap.h
//  Tour2dx
//
//  Created by Melon on 14-11-14.
//
//

#ifndef __TOUR2DX__RLSWAP__
#define __TOUR2DX__RLSWAP__

#include "cocos2d.h"
#include "RLCookie.h"

USING_NS_CC;

class RLSwap: public Node {
private:
    
public:
    static RLSwap* create();
    
    CC_SYNTHESIZE(RLCookie*, cookieA, CookieA);
    CC_SYNTHESIZE(RLCookie*, cookieB, CookieB);
    
    virtual bool equals(const RLSwap &other) const;
    virtual bool operator==(const RLSwap &other) const;
};

#endif /* defined(__TOUR2DX__RLSWAP__) */
