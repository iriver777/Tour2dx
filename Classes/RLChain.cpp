//
//  RLChain.cpp
//  Tour2dx
//
//  Created by Melon on 14-11-15.
//
//

#include "RLChain.h"

RLChain* RLChain::create()
{
    RLChain* chain = new RLChain();
    chain->autorelease();
    return chain;
}

void RLChain::addCookie(RLCookie *cookie)
{
    _cookies.pushBack(cookie);
}

Vector<RLCookie*> RLChain::getCookies() const
{
    return _cookies;
}