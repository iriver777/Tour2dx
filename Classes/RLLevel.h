//
//  RLLevel.h
//  Tour2dx
//
//  Created by Melon on 14-11-9.
//
//

#ifndef __TOUR2DX__RLLEVEL__
#define __TOUR2DX__RLLEVEL__

#include "cocos2d.h"
#include "RLCookie.h"
#include "RLTile.h"
#include "RLSwap.h"
#include "RLChain.h"
#include "RLProgress.h"

#include "json/rapidjson.h"
#include "json/document.h"
//#include "json/writer.h"
//#include "json/stringbuffer.h"



USING_NS_CC;
using namespace rapidjson;

static const int NumColumns = 9;
static const int NumRows = 9;
static const std::string gameFile = "gamestatistic.xml";

class RLLevel: public Ref {
private:
    RLCookie* _cookies[NumColumns][NumRows];
    RLTile* _tiles[NumColumns][NumRows];
    
    Vector<RLCookie *> createInitialCookies();
    Vector<RLSwap *> possibleSwaps;
    RLCookie* createCookie(int column, int row, unsigned int cookietype);
    
    bool hasChainAtColumnOrRow(int col, int row);
    
    void removeCookies(Vector<RLChain *> &chains);
    
public:
    RLLevel();
    ~RLLevel();
    
    static RLLevel* create();

    RLCookie* cookieAtColumnAndRow(int column, int row);
    Vector<RLCookie *> shuffle();
    
    void initWithJSONFile(const std::string &filename);
    
    CC_SYNTHESIZE(int, pattern, Pattern);
    CC_SYNTHESIZE(unsigned int, curLevel, CurrentLevel);
    CC_SYNTHESIZE(unsigned int, targetScore, TargetScore);
    CC_SYNTHESIZE(unsigned int, maxMoves, MaxMoves);
    CC_SYNTHESIZE(unsigned int, score, Score);
    
    
    RLTile* tileAtColumnAndRow(int column, int row);
    
    void detectPossibleSwaps();
    void performSwap(RLSwap* swap);
    bool isPossibleSwap(const RLSwap* swap);
    
    Vector<RLChain *> detectHorizontalMatches();
    Vector<RLChain *> detectVerticalMatches();
    Vector<RLChain *> removeMatches();
    
    //handle falling cookies after removing matches
    Vector<RLChain *> fillHoles();      //vertically
    Vector<RLChain *> topUpCookies();   //vertically
    
    void calcScores(Vector<RLChain *> &chains);
    
    //progress info
    bool initProgressFile();
    RLProgress* findProgress(unsigned int valPattern);
    bool saveProgress(RLProgress* progress);
};

#endif /* defined(__TOUR2DX__RLLEVEL__) */
