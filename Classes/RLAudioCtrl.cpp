//
//  RLAudioCtrl.cpp
//  Tour2dx
//
//  Created by Melon on 14-11-15.
//
//

#include "RLAudioCtrl.h"

RLAudioCtrl* RLAudioCtrl::m_audioctrl = nullptr;

RLAudioCtrl* RLAudioCtrl::getInstance()
{
    if (nullptr == m_audioctrl) {
        m_audioctrl = new RLAudioCtrl();
        
    }
    return m_audioctrl;
}

void RLAudioCtrl::end()
{
    if (m_audioctrl) {
        delete m_audioctrl;
        m_audioctrl = NULL;
    }
    
    SimpleAudioEngine::end();
}

void RLAudioCtrl::preloadAudioResources()
{
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BGMUSIC);
    
    SimpleAudioEngine::getInstance()->preloadEffect(SWAP_FILE);
    SimpleAudioEngine::getInstance()->preloadEffect(INVALIDSWAP_FILE);
    SimpleAudioEngine::getInstance()->preloadEffect(MATCH_FILE);
    SimpleAudioEngine::getInstance()->preloadEffect(FALLING_FILE);
    SimpleAudioEngine::getInstance()->preloadEffect(ADDCOOKIE);

}

void RLAudioCtrl::playSoundEffect(const AudioEffectType &effect){
    const char * soundfile = NULL;
    
    switch (effect) {
        case SoundSwap:
            soundfile = SWAP_FILE;
            break;
        
        case SoundInvalidSwap:
            soundfile = INVALIDSWAP_FILE;
            break;
        
        case SoundMatch:
            soundfile = MATCH_FILE;
            break;
        
        case SoundFalling:
            soundfile = FALLING_FILE;
            break;
        
        case SoundAddCookie:
            soundfile = ADDCOOKIE;
            break;
            
        default:
            break;
    }
    if (soundfile != NULL) {
        SimpleAudioEngine::getInstance()->playEffect(soundfile);
    }else{
        log("unidentified sound effect type");
    }
    
}

void RLAudioCtrl::playBackgroundMusic()
{
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);
    SimpleAudioEngine::getInstance()->playBackgroundMusic(BGMUSIC,true);
}
