//
//  HomeScene.h
//  Tour2dx
//
//  Created by Melon on 14-11-18.
//
//

#ifndef __TOUR2DX__HOMESCENE__
#define __TOUR2DX__HOMESCENE__

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class HomeScene: public cocos2d::Layer {
    
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    virtual bool init();
	CREATE_FUNC(HomeScene);
    
private:
    void startGameCallback();
    void menuCallback(cocos2d::Ref* pSender);
    void exitCallback(cocos2d::Ref* pSender);
    
};

#endif /* defined(__TOUR2DX__HOMESCENE__) */
