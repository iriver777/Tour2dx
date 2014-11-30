//
//  RLHUDLayer.h
//  Tour2dx
//
//  Created by Melon on 14-11-17.
//
//

#ifndef __TOUR2DX__RLHUDLAYER__
#define __TOUR2DX__RLHUDLAYER__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class RLHUDLayer:public cocos2d::Layer {
    
public:
    RLHUDLayer(void);
	~RLHUDLayer(void);
    
    virtual bool init();
	CREATE_FUNC(RLHUDLayer);
    
    void setScoreVal(const std::string &val);
    void setTargetVal(const std::string &val);
    void setMovesVal(const std::string &val);
    void setLevelVal(const std::string &val);
private:
    Label* lblTargetTitle;
    Label* lblMovesTitle;
    Label* lblScoreTitle;
    Label* lblLevelTitle;
    
    Label* lblTargetValue;
    Label* lblMovesValue;
    Label* lblScoreValue;
    Label* lblLevelValue;
};

#endif /* defined(__TOUR2DX__RLHUDLAYER__) */
