#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "MainScene.h"
#include "HomeScene.h"


USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("Match3 Crush Game");
        director->setOpenGLView(glview);
    }

    glview->setDesignResolutionSize(320, 480, ResolutionPolicy::FIXED_WIDTH);
    
    vector<string> searchPath;
    searchPath.push_back("w640");
    searchPath.push_back("fonts");
    searchPath.push_back("Sounds");
    searchPath.push_back("Levels");
    
    
    FileUtils::getInstance()->setSearchPaths(searchPath);
    director->setContentScaleFactor(2.0);
    
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
//    auto scene = HelloWorld::createScene();
//    auto scene = MainScene::createScene();
    auto scene = HomeScene::createScene();
    
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    if (RLAudioCtrl::getInstance()->getMusicState()) {
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
    if (RLAudioCtrl::getInstance()->getEffectState()) {
            SimpleAudioEngine::getInstance()->resumeAllEffects();
    }

}
