//
//  RLCookie.h
//  Tour2dx
//
//  Created by Melon on 14-11-9.
//
//

#ifndef __TOUR2DX__RLCOOKIE__
#define __TOUR2DX__RLCOOKIE__

#include "cocos2d.h"
using namespace std;
USING_NS_CC;

static const unsigned int NumCookieTypes = 7;

class RLCookie: public Ref {
public:
    static RLCookie* create(int column, int row, unsigned cookietype);
    
    CC_SYNTHESIZE(int, column, Column);
    CC_SYNTHESIZE(int, row, Row);
    CC_SYNTHESIZE(unsigned, cookietype, CookieType);
    CC_SYNTHESIZE(Sprite *, sprite, Sprite);

    string spriteName();
    string highlightedSpriteName();
};


#endif /* defined(__TOUR2DX__RLCOOKIE__) */
