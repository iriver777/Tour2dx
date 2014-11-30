//
//  RLAudioCtrl.h
//  Tour2dx
//
//  Created by Melon on 14-11-15.
//
//

#ifndef __TOUR2DX__RLAUDIOCTRL__
#define __TOUR2DX__RLAUDIOCTRL__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

#define SWAP_FILE "Chomp.wav"
#define INVALIDSWAP_FILE "Error.wav"
#define MATCH_FILE "Ka-Ching.wav"
#define FALLING_FILE "Scrape.wav"
#define ADDCOOKIE "Drip.wav"
#define BGMUSIC "apple.mp3"

typedef enum {
    SoundSwap,
    SoundInvalidSwap,
    SoundMatch,
    SoundFalling,
    SoundAddCookie
} AudioEffectType;

class RLAudioCtrl:public Ref {
    
public:
    static RLAudioCtrl* getInstance();
    static void end();
//    RLAudioCtrl* create();
    void preloadAudioResources();
    void playSoundEffect(const AudioEffectType &effect);
    void playBackgroundMusic();
    CC_SYNTHESIZE(bool, musicState, MusicState);
    CC_SYNTHESIZE(bool, effectState, EffectState);
    
private:
    static RLAudioCtrl* m_audioctrl;
};

#endif /* defined(__TOUR2DX__RLAUDIOCTRL__) */
