//  RLCookie.cpp
//  Tour2dx
//
//  Created by Melon on 14-11-9.
//
//

#include "RLCookie.h"

RLCookie* RLCookie::create(int column, int row, unsigned cookietype)
{
    RLCookie* cookie = new RLCookie();
    cookie->column = column;
    cookie->row = row;
    cookie->cookietype = cookietype;
    cookie->autorelease();
    
    return cookie;
}

string RLCookie::spriteName()
{
    static string const spriteNames[] = {"pieapple.png",
        "orange.png",
        "apple.png",
        "kiwi.png",
        "banana.png",
        "grape.png",
        "strawberry.png"};
    return spriteNames[this->cookietype - 1];
}

string RLCookie::highlightedSpriteName()
{
    static string const highlightedSpriteNames[] = {
        "pieapple-Highlighted.png",
        "orange-Highlighted.png",
        "apple-Highlighted.png",
        "kiwi-Highlighted.png",
        "banana-Highlighted.png",
        "grape-Highlighted.png",
        "strawberry-Highlighted.png"
    };
    
    return highlightedSpriteNames[this->cookietype - 1];
}


