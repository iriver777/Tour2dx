//
//  RLLevel.cpp
//  Tour2dx
//
//  Created by Melon on 14-11-9.
//
//

#include "RLLevel.h"
#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

static const char* valAttName = "val";

RLLevel::RLLevel(){
    for (int col = 0; col < NumColumns; col++) {
        for (int row = 0; row < NumRows; row++) {
            _cookies[col][row] = NULL;
            _tiles[col][row] = NULL;
        }
    }
}

RLLevel::~RLLevel(){
    for (int col = 0; col < NumColumns; col++) {
        for (int row = 0; row < NumRows; row++) {
            if (_cookies[col][row] != NULL) {
                if (_cookies[col][row]->getSprite() != NULL) {
                    _cookies[col][row]->setSprite(NULL);
                    
                }
                _cookies[col][row]->release();
                _cookies[col][row] = NULL;
            }
        }
    }
}


RLLevel* RLLevel::create()
{
    RLLevel * level = new RLLevel();
    level->autorelease();
    return level;
}



RLCookie* RLLevel::cookieAtColumnAndRow(int column, int row)
{
    CCASSERT(column >= 0 && column < NumColumns, "Invalid column");
    CCASSERT(row >= 0 && row < NumRows, "Invalid row");
    
    return _cookies[column][row];
}


RLCookie* RLLevel::createCookie(int column, int row, unsigned cookietype)
{
    RLCookie* cookie = RLCookie::create(column, row, cookietype);
    
    cookie->retain();   //retain for _cookies matrix
    
    _cookies[column][row] = cookie;
    
    return cookie;
}

Vector<RLCookie *> RLLevel::createInitialCookies()
{
    Vector<RLCookie *> vector;
    
    for (int row = 0; row < NumRows; row++) {
        for (int col = 0; col < NumColumns; col++) {
            
            //reset _cookie(col, row) as NULL
//            _cookies[col][row] = NULL;
            if (_cookies[col][row] != NULL) {
                
                if (_cookies[col][row]->getSprite() != NULL) {
                    _cookies[col][row]->getSprite()->removeFromParent();
                    _cookies[col][row]->setSprite(NULL);
                }

                _cookies[col][row] = NULL;
            }
            
            if(_tiles[col][row] != NULL){
                unsigned int cookietype;
                
                do {
                    cookietype = CCRANDOM_0_1() * NumCookieTypes + 1;
                } while ((col >= 2 &&
                          (_cookies[col-1][row] != NULL && _cookies[col-1][row]->getCookieType() == cookietype) &&
                          (_cookies[col-2][row] != NULL && _cookies[col-2][row]->getCookieType() == cookietype))||
                         (row >= 2 &&
                          (_cookies[col][row-1] != NULL && _cookies[col][row-1]->getCookieType() == cookietype) &&
                          (_cookies[col][row-2] != NULL && _cookies[col][row-2]->getCookieType() == cookietype)));
                
                
                RLCookie * cookie = RLCookie::create(col, row, cookietype);
                
                cookie->retain();   //retain for _cookies
                _cookies[col][row] = cookie;
                vector.pushBack(cookie);
            }
        }
    }
    
    return vector;
}

Vector<RLCookie *> RLLevel::shuffle()
{
    Vector<RLCookie*> vect;
    
    do {
        vect = this->createInitialCookies();
        
        this->detectPossibleSwaps();
        
    } while (this->possibleSwaps.size() == 0);
    
    return vect;
}

//read JSON file to cofigure the level
void RLLevel::initWithJSONFile(const std::string &filename)
{
    rapidjson::Document doc;
    
    if (!FileUtils::getInstance()->isFileExist(filename)) {
        log("json file can't be found");
        return;
    }
    
    std::string data = FileUtils::getInstance()->getStringFromFile(filename);
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    
    if (doc.HasParseError()) {
        log("get json data error");
        return;
    }
    
    rapidjson::Value &valArr = doc["tiles"];
    if (valArr.IsArray()) {
        for (int _row=0; _row<valArr.Size(); _row++) {
            rapidjson::Value &rowArr = valArr[_row];
            
            for (int _col=0; _col<rowArr.Capacity(); _col++) {
                rapidjson::Value &valTile = rowArr[_col];
                int iTile = valTile.GetInt();
                
                //read from upside down
                int tileRow = NumRows - _row - 1;
                
//                log("(%d,%d): %d", tileRow, _col, iTile);
                if (iTile == 1) {
                    _tiles[_col][tileRow] = RLTile::create();
                }else{
                    _tiles[_col][tileRow] = NULL;
                }
                
            }
        }
    }
    
//    rapidjson::Value &valScore = doc["targetScore"];
//    this->targetScore = valScore.GetInt();
    
    rapidjson::Value &valMoves = doc["moves"];
    this->maxMoves = valMoves.GetInt();
    
    RLProgress *progress = findProgress(this->pattern);
    if (progress != NULL) {
        this->curLevel = progress->getLevel();
        this->score = progress->getScore();
        this->targetScore = progress->getTargetToGo();
    }
}

RLTile* RLLevel::tileAtColumnAndRow(int column, int row)
{
    CCASSERT(column >= 0 && column < NumColumns, "Invalid column");
    CCASSERT(row >= 0 && row < NumRows, "Invalid row");
    
    return _tiles[column][row];
}

void RLLevel::performSwap(RLSwap *swap)
{
    int colA = swap->getCookieA()->getColumn();
    int rowA = swap->getCookieA()->getRow();
    int colB = swap->getCookieB()->getColumn();
    int rowB = swap->getCookieB()->getRow();
    
    _cookies[colA][rowA] = swap->getCookieB();
    swap->getCookieB()->setColumn(colA);
    swap->getCookieB()->setRow(rowA);
    
    _cookies[colB][rowB] = swap->getCookieA();
    swap->getCookieA()->setColumn(colB);
    swap->getCookieA()->setRow(rowB);
}

bool RLLevel::hasChainAtColumnOrRow(int col, int row)
{
    if (_cookies[col][row] == NULL) {
        return false;
    }
    
    unsigned int cookietype = _cookies[col][row]->getCookieType();
    
    unsigned int horzLen = 1;
    for (int i = col-1; i >=0 && (_cookies[i][row] != NULL && _cookies[i][row]->getCookieType() == cookietype);i--,horzLen++);
    for (int i = col+1; i < NumColumns && (_cookies[i][row] != NULL && _cookies[i][row]->getCookieType() == cookietype);i++,horzLen++);
    if (horzLen >= 3) {
        return true;
    }
    
    unsigned int vertLen = 1;
    for (int i = row-1; i >=0 && (_cookies[col][i] != NULL && _cookies[col][i]->getCookieType() == cookietype);i--,vertLen++);
    for (int i = row+1; i < NumRows && (_cookies[col][i] != NULL && _cookies[col][i]->getCookieType() == cookietype);i++,vertLen++);
    return (vertLen>=3);
    
}

//detect possible swaps
void RLLevel::detectPossibleSwaps()
{
    Vector<RLSwap*> set;
    
    for (int row = 0; row < NumRows; row++) {
        for (int col = 0; col < NumColumns; col++) {
            RLCookie* cookie = _cookies[col][row];
            if (cookie != NULL) {
                //detection logic
                //Is it possible to swap this cookie with the one on the right?
                if (col < NumColumns - 1) {
                    RLCookie* other = _cookies[col+1][row];
                    if (other != NULL) {
                        
                        //swap them for detection first
                        _cookies[col][row] = other;
                        _cookies[col+1][row] = cookie;
                        
                        if (this->hasChainAtColumnOrRow(col+1, row) ||
                            this->hasChainAtColumnOrRow(col, row)) {
                            RLSwap* swap = RLSwap::create();
                            swap->setCookieA(cookie);
                            swap->setCookieB(other);
                            
                            set.pushBack(swap);
                        }
                        
                        //swap them back after detection
                        _cookies[col][row] = cookie;
                        _cookies[col+1][row] = other;
                    }
                }
                
                //Is it possible to swap this cookie with the one on the above?
                if (row < NumRows-1) {
                    RLCookie* other = _cookies[col][row+1];
                    if (other != NULL) {
                        //swap them first for detection
                        _cookies[col][row+1] = cookie;
                        _cookies[col][row] = other;
                        
                        if (this->hasChainAtColumnOrRow(col, row+1)||
                            this->hasChainAtColumnOrRow(col, row)) {
                            RLSwap* swap = RLSwap::create();
                            swap->setCookieA(cookie);
                            swap->setCookieB(other);

                            set.pushBack(swap);
                        }
                        
                        //swap them back after detection
                        _cookies[col][row+1] = other;
                        _cookies[col][row] = cookie;
                    }
                }
            }
        }
    }
    
    this->possibleSwaps = set;
}

bool RLLevel::isPossibleSwap(const RLSwap *swap)
{
//    return this->possibleSwaps.contains(swap);
    
    vector<RLSwap *>::const_iterator itr;
    for (itr = this->possibleSwaps.begin(); itr != this->possibleSwaps.end(); ++itr) {
        RLSwap *curSwap = *itr;
        if (curSwap->equals(*swap)) {
            return true;
        }
    }
    
    return false;
}

Vector<RLChain *> RLLevel::detectHorizontalMatches()
{
    Vector<RLChain *> vect;
    
    for (int row = 0; row < NumRows; row++) {
        for (int col = 0; col < NumColumns-2; ) {
            if (_cookies[col][row] != NULL) {
                unsigned int matchtype = _cookies[col][row]->getCookieType();
                
                if ((_cookies[col+1][row]!= NULL && _cookies[col+1][row]->getCookieType()==matchtype) &&
                    (_cookies[col+2][row]!= NULL && _cookies[col+2][row]->getCookieType()==matchtype)) {
                    RLChain* chain = RLChain::create();
                    chain->setChainType(ChainTypeHorizontal);
                    
                    do {
                        chain->addCookie(_cookies[col][row]);
                        col += 1;
                    } while (col<NumColumns && _cookies[col][row]!= NULL
                             && _cookies[col][row]->getCookieType()==matchtype);
                    
                    vect.pushBack(chain);
                    continue;
                }
            }
            
            col += 1;
        }
    }
    
    return vect;
}

Vector<RLChain *> RLLevel::detectVerticalMatches()
{
    Vector<RLChain *> vect;
    
    for (int col = 0; col < NumColumns; col++) {
        for (int row = 0; row < NumRows-2; ) {
            if (_cookies[col][row] != NULL) {
                unsigned int matchtype = _cookies[col][row]->getCookieType();
                
                if ((_cookies[col][row+1]!= NULL && _cookies[col][row+1]->getCookieType()==matchtype) &&
                    (_cookies[col][row+2]!= NULL && _cookies[col][row+2]->getCookieType()==matchtype)) {
                    RLChain* chain = RLChain::create();
                    chain->setChainType(ChainTypeVertical);
                    
                    do {
                        chain->addCookie(_cookies[col][row]);
                        row += 1;
                    } while (row < NumRows && _cookies[col][row]!= NULL
                             && _cookies[col][row]->getCookieType()==matchtype);
                    
                    vect.pushBack(chain);
                    continue;
                }
            }
            
            row += 1;
        }
    }
    
    return vect;
}

Vector<RLChain *> RLLevel::removeMatches()
{
    Vector<RLChain *> horzChains = this->detectHorizontalMatches();
    Vector<RLChain *> vertChains = this->detectVerticalMatches();
    
    this->removeCookies(horzChains);
    this->removeCookies(vertChains);
    
    this->calcScores(horzChains);
    this->calcScores(vertChains);
    
    horzChains.pushBack(vertChains);
    
    return horzChains;
}

void RLLevel::removeCookies(Vector<RLChain *> &chains)
{
    Vector<RLChain *>::iterator itr;
    for (itr = chains.begin(); itr != chains.end(); itr++) {
        RLChain* chain = (*itr);
        Vector<RLCookie*> cookies = chain->getCookies();
        
        Vector<RLCookie *>::iterator itr_ck;
        for (itr_ck = cookies.begin(); itr_ck != cookies.end(); itr_ck++) {
            RLCookie* cookie = (*itr_ck);
            
            this->_cookies[cookie->getColumn()][cookie->getRow()] = NULL;
        }
    }
}

//calculate scores in the matched chains
void RLLevel::calcScores(Vector<RLChain *> &chains)
{
    for (RLChain* chain : chains)
    {
        chain->setScore(60 * ((chain->getCookies()).size() - 2));
    }
}

Vector<RLChain *> RLLevel::fillHoles()
{
    Vector<RLChain *> columns;
    
    for (int col = 0; col < NumColumns; col++) {
        RLChain* chain = NULL;

        
        for (int row = 0; row < NumRows; row++) {
            if (_tiles[col][row] != NULL && _cookies[col][row] == NULL) {
                for (int lookup = row+1; lookup < NumRows; lookup++) {
                    RLCookie* cookie = _cookies[col][lookup];
                    if (cookie != NULL) {
                        _cookies[col][lookup] = NULL;
                        _cookies[col][row] = cookie;
                        cookie->setRow(row);

                        if (chain == NULL) {
                            chain = RLChain::create();
                            chain->setChainType(ChainTypeVertical);
                            
                            columns.pushBack(chain);
                        }

                        chain->addCookie(cookie);

                        break;  //getinto next row loop i.e. before lookup for loop
                    }
                }
            }
        }
    }
    
    return columns;
}


Vector<RLChain *> RLLevel::topUpCookies()
{
    Vector<RLChain *> columns;
    unsigned int cookietype = 0;
    
    for (int col = 0; col < NumColumns; col++) {
        RLChain* chain = NULL;
        
        for (int row = NumRows-1; row >=0 && _cookies[col][row]== NULL; row--) {
            if (_tiles[col][row] != NULL) {
                unsigned int newcookietype;
                
                do {
                    newcookietype = CCRANDOM_0_1() * NumCookieTypes + 1;
                } while (newcookietype == cookietype);
                cookietype = newcookietype;
                
//                RLCookie* cookie = RLCookie::create(col, row, cookietype);
                
                RLCookie* cookie = this->createCookie(col, row, cookietype);
                if (chain == NULL) {
                    chain = RLChain::create();
                    chain->setChainType(ChainTypeVertical);
                    
                    columns.pushBack(chain);
                }
                
                chain->addCookie(cookie);
                
            }
        }
    }
    
    return columns;
}

bool RLLevel::initProgressFile()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    log("%s", path.c_str());
    path.append(gameFile);
    
    if (!FileUtils::getInstance()->isFileExist(path)) {
        XMLDocument *pDoc = new XMLDocument();
        if (pDoc == NULL) {
            log("can't create xml instance");
            return false;
        }
        
        XMLDeclaration *pDel = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
        if (pDel == NULL) {
            log("can't create xml declaration");
            return false;
        }
        pDoc->LinkEndChild(pDel);
        
        //create root node
        XMLElement *rootElement = pDoc->NewElement("statistic");
        rootElement->SetAttribute("version", "1.0");
        pDoc->LinkEndChild(rootElement);
        
        for (int i=1; i<=4; i++) {
            XMLElement *patternNode = pDoc->NewElement("pattern");
            //            std::string p = StringUtils::format("%d", i);
            patternNode->SetAttribute("val", i);
            rootElement->LinkEndChild(patternNode);
            
            XMLElement *levelNode = pDoc->NewElement("level");
            levelNode->SetAttribute("val", 1);
            //            levelNode->LinkEndChild(pDoc->NewText("1"));
            patternNode->LinkEndChild(levelNode);
            
            XMLElement *scoreNode = pDoc->NewElement("score");
            scoreNode->SetAttribute("val", 0);
            //            scoreNode->LinkEndChild(pDoc->NewText("0"));
            patternNode->LinkEndChild(scoreNode);
            
            XMLElement *doneNode = pDoc->NewElement("done");
            doneNode->SetAttribute("val", 0);
            //            doneNode->LinkEndChild(pDoc->NewText("0"));
            patternNode->LinkEndChild(doneNode);
            
            XMLElement *togoNode = pDoc->NewElement("togo");
            togoNode->SetAttribute("val", 1000);
            //            togoNode->LinkEndChild(pDoc->NewText("1000"));
            patternNode->LinkEndChild(togoNode);
        }
        
        pDoc->SaveFile(path.c_str());
        pDoc->Print();
        
        delete pDoc;
    }
    
    return true;
    
}

//find current progress info under one pattern from xml file
RLProgress* RLLevel::findProgress(unsigned int curPattern)
{
    RLProgress* progress = NULL;
    
    auto path = FileUtils::getInstance()->getWritablePath();
    log("%s", path.c_str());
    path.append(gameFile);
    
    if (FileUtils::getInstance()->isFileExist(path)) {
        progress = RLProgress::create();
        
        XMLDocument *pDoc = new XMLDocument();
        pDoc->LoadFile(path.c_str());
        
        XMLElement *rootEle = pDoc->RootElement();
        
        XMLElement *patternEle = rootEle->FirstChildElement(); //pattern
        
        while (patternEle) {
            int iPattern = 0;
            patternEle->QueryIntAttribute("val", &iPattern);
            log("pattern: %d", iPattern);
            
            if (iPattern == curPattern) {
                //level
                XMLElement *levelEle = patternEle->FirstChildElement();
                unsigned int iLevel = 1;
                levelEle->QueryUnsignedAttribute("val",&iLevel);
                progress->setLevel(iLevel);
                
                XMLElement *scoreEle = levelEle->NextSiblingElement();
                unsigned int iScore = 0;
                scoreEle->QueryUnsignedAttribute("val",&iScore);
                progress->setScore(iScore);
                
                XMLElement *doneEle = scoreEle->NextSiblingElement();
                unsigned int iDone = 0;
                doneEle->QueryUnsignedAttribute("val",&iDone);
                progress->setTargetDone(iDone);
 
                XMLElement *togoEle = doneEle->NextSiblingElement();
                unsigned int iTogo = 0;
                togoEle->QueryUnsignedAttribute("val",&iTogo);
                progress->setTargetToGo(iTogo);
                
                break;
                
            } else {
                //loop to find the matched pattern node
                patternEle = patternEle->NextSiblingElement();
            }
        }
    }
    
    return progress;
}

//save progress after winning each round
bool RLLevel::saveProgress(RLProgress *progress)
{
    
    int curPattern = progress->getPattern();
    
    auto path = FileUtils::getInstance()->getWritablePath();
    log("%s", path.c_str());
    path.append(gameFile);
    
    if (FileUtils::getInstance()->isFileExist(path)) {
        
    
        XMLDocument *pDoc = new XMLDocument();
        pDoc->LoadFile(path.c_str());
    
        XMLElement *rootEle = pDoc->RootElement();

        XMLElement *patternEle = rootEle->FirstChildElement(); //pattern
        
        while (patternEle) {
            int iPattern = 0;
            patternEle->QueryIntAttribute("val", &iPattern);
            log("pattern: %d", iPattern);

            if (iPattern == curPattern) {
                //level
                XMLElement *levelEle = patternEle->FirstChildElement();
                unsigned int iLevel = progress->getLevel();
                levelEle->SetAttribute(valAttName, iLevel);
//                log("level: %u", iLevel);
                
                XMLElement *scoreEle = levelEle->NextSiblingElement();
                unsigned int iScore = progress->getScore();
                scoreEle->SetAttribute(valAttName, iScore);
//                log("level: %u", iScore);
                
                
                XMLElement *doneEle = scoreEle->NextSiblingElement();
                unsigned int iDone = progress->getTargetDone();
                doneEle->SetAttribute(valAttName, iDone);
//                log("level: %u", iDone);
                
                XMLElement *togoEle = doneEle->NextSiblingElement();
                unsigned int iTogo = progress->getTargetToGo();
                togoEle->SetAttribute(valAttName, iTogo);
//                log("level: %u", iTogo);
                
                break;
            } else {
                //loop to find the matched pattern node
                patternEle = patternEle->NextSiblingElement();
            }
        }
        
        pDoc->SaveFile(path.c_str());
        pDoc->Print();
        
        delete pDoc;
        return true;
        
    }else{
        
        return false;
    }
//    rapidjson::Document doc;
//    doc.SetObject();
//    
//    Document::AllocatorType &allocator = doc.GetAllocator();
//    
//    rapidjson::Value arr(rapidjson::kArrayType);
//    rapidjson::Value obj(rapidjson::kObjectType);
//    
//    obj.AddMember("level", progress->getLevel(), allocator);
//    obj.AddMember("score", progress->getScore(), allocator);
//    obj.AddMember("targetdone", progress->getTargetDone(), allocator);
//    obj.AddMember("targettogo", progress->getTargetToGo(), allocator);
//    arr.PushBack(obj, allocator);
//    
//    doc.AddMember("pattern", progress->getPattern(), allocator);
//    doc.AddMember("detail", arr, allocator);
//    
//    auto path = FileUtils::getInstance()->getWritablePath();
//    log("%s", path.c_str());
//    path.append("gamestatistic.json");
//    
//    rapidjson::StringBuffer buffer;
//    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
//    
//    doc.Accept(writer);
//    FILE* file = fopen(path.c_str(), "wb");
//    if (file) {
//        fputs(buffer.GetString(), file);
//        fclose(file);
//    }

}