//
//  RLProgress.cpp
//  Tour2dx
//
//  Created by Melon on 14-11-23.
//
//

#include "RLProgress.h"

RLProgress* RLProgress::create()
{
    RLProgress* p = new RLProgress();
    p->autorelease();
    return p;
}

unsigned int RLProgress::getTargetToGo()
{
    if (level == 1) {
        targetToGo = 1000;
    }else{
        targetToGo = calcTargetToGo(this->score);
    }
    return targetToGo;
}

void RLProgress::setTargetToGo(unsigned int var)
{
    targetToGo = var;
}

unsigned int RLProgress::calcTargetToGo(unsigned int score)
{
    unsigned int result = 1000;
//    int mm = (score - this->targetDone)% 1000;
//    
//    if (mm >= 0 && mm <= 300) {
//        result = 1000;
//    }else if(mm > 300 && mm <= 500){
//        result = 1100;
//    }else if(mm > 500 && mm <= 800){
//        result = 1300;
//    }else if(mm > 800 && mm <= 999){
//        result = 1400;
//    }
    
    result += this->targetDone;
    
    return result;
}