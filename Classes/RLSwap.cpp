//
//  RLSwap.cpp
//  Tour2dx
//
//  Created by Melon on 14-11-14.
//
//

#include "RLSwap.h"

RLSwap* RLSwap::create()
{
    RLSwap* swap = new RLSwap();
    swap->autorelease();
    
    return swap;
}

bool RLSwap::equals(const RLSwap &other) const
{
    return ((other.cookieA == this->cookieA && other.cookieB == this->cookieB)||
            (other.cookieB == this->cookieA && other.cookieA == this->cookieB));
}

bool RLSwap::operator==(const RLSwap &other) const
{
    return RLSwap::equals(other);
}