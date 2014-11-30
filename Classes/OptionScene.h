//
//  OptionScene.h
//  Tour2dx
//
//  Created by Melon on 14-11-18.
//
//

#ifndef __TOUR2DX__OPTIONSCENE__
#define __TOUR2DX__OPTIONSCENE__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "RLAudioCtrl.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;


class OptionScene:public Layer {
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
	CREATE_FUNC(OptionScene);
    
private:
    void menuSoundCallback(cocos2d::Ref* pSender);
    void menuMusicCallback(cocos2d::Ref* pSender);
    void menuBackCallback(cocos2d::Ref* pSender);
    void menuPatternCallback(Ref *pSender);
    
    Label* lblPattern;
//    ControlButton* standardButtonWithTitle(const char * title);
    
};

#endif /* defined(__TOUR2DX__OPTIONSCENE__) */
