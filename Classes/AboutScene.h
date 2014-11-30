//
//  AboutScene.h
//  Tour2dx
//
//  Created by Melon on 14-11-9.
//
//

#ifndef __TOUR2DX__ABOUTSCENE__
#define __TOUR2DX__ABOUTSCENE__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class AboutScene: public cocos2d::Layer {
    
public:
    static cocos2d::Scene * createScene();
    virtual bool init();
    
    CREATE_FUNC(AboutScene);

private:
    void goBack(Ref* pSender);
};

#endif /* defined(__TOUR2DX__ABOUTSCENE__) */
