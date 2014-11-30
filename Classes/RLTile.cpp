//
//  RLTile.cpp
//  Tour2dx
//
//  Created by Melon on 14-11-10.
//
//

#include "RLTile.h"

RLTile* RLTile::create()
{
    RLTile* tile = new RLTile();
    tile->autorelease();
    
    return tile;
}

