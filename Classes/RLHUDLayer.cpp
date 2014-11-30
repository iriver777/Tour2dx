//
//  RLHUDLayer.cpp
//  Tour2dx
//
//  Created by Melon on 14-11-17.
//
//

#include "RLHUDLayer.h"

RLHUDLayer::RLHUDLayer()
{
}

RLHUDLayer::~RLHUDLayer()
{
}

bool RLHUDLayer::init()
{
    if (!Layer::init()) return false;
    
    Size winSize = Director::getInstance()->getWinSize();
    
    const string fontName1 = "Marker Felt";
    const string fontName2 = "Couries New";
    
    lblLevelTitle = Label::createWithSystemFont("Level:", fontName1, 18);
    lblLevelTitle->setAnchorPoint(Point::ANCHOR_MIDDLE);
    lblLevelTitle->setPosition(Point(winSize.width*0.13, winSize.height*0.9));
    lblLevelTitle->setColor(Color3B(231,132,0));
    this->addChild(lblLevelTitle);
    
    Rect rect = lblLevelTitle->getBoundingBox();
    Size size = lblLevelTitle->getContentSize();
    float yTitile = lblLevelTitle->getPositionY();
    
    lblTargetTitle = Label::createWithSystemFont("Target:", fontName1, 18);
    lblTargetTitle->setAnchorPoint(Point::ANCHOR_MIDDLE);
    lblTargetTitle->setPosition(Point(winSize.width*0.13, yTitile-30));
    lblTargetTitle->setColor(Color3B(231,132,0));
    this->addChild(lblTargetTitle);
    
    
    lblMovesTitle = Label::createWithSystemFont("Moves", fontName1, 18);
    lblMovesTitle->setAnchorPoint(Point::ANCHOR_MIDDLE);
    lblMovesTitle->setPosition(Point(winSize.width*0.58, yTitile));
    lblMovesTitle->setColor(Color3B(231,132,0));
    this->addChild(lblMovesTitle);
    
    lblScoreTitle = Label::createWithSystemFont("Score", fontName1, 18);
    lblScoreTitle->setAnchorPoint(Point::ANCHOR_MIDDLE);
    lblScoreTitle->setPosition(Point(winSize.width*0.8, yTitile));
    lblScoreTitle->setColor(Color3B(231,132,0));
    this->addChild(lblScoreTitle);
    
    
    lblLevelValue = Label::createWithSystemFont("9999", fontName2, 15);
    lblLevelValue->setPosition(Point(rect.origin.x + size.width + 5, yTitile));
    lblLevelValue->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    lblLevelValue->setColor(Color3B(215,165,0));
    this->addChild(lblLevelValue);
    
    lblTargetValue = Label::createWithSystemFont("999999", fontName2, 15);
    lblTargetValue->setPosition(Point(lblLevelValue->getBoundingBox().origin.x,
                                     lblTargetTitle->getPositionY()));
    lblTargetValue->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    lblTargetValue->setColor(Color3B(215,165,0));
    this->addChild(lblTargetValue);
    
    yTitile -= 30;
    lblMovesValue = Label::createWithSystemFont("999999", fontName2, 15);
    lblMovesValue->setPosition(Point(winSize.width*0.58, yTitile));
    lblMovesValue->setAnchorPoint(Point::ANCHOR_MIDDLE);
    lblMovesValue->setColor(Color3B(215,165,0));
    this->addChild(lblMovesValue);
    
    lblScoreValue = Label::createWithSystemFont("999999", fontName2, 15);
    lblScoreValue->setPosition(Point(winSize.width*0.8, yTitile));
    lblScoreValue->setAnchorPoint(Point::ANCHOR_MIDDLE);
    lblScoreValue->setColor(Color3B(215,165,0));
    this->addChild(lblScoreValue);
    
    return true;
}

void RLHUDLayer::setTargetVal(const std::string &val)
{
    this->lblTargetValue->setString(val);
}

void RLHUDLayer::setMovesVal(const std::string &val)
{
    this->lblMovesValue->setString(val);
}

void RLHUDLayer::setScoreVal(const std::string &val)
{
    this->lblScoreValue->setString(val);
}

void RLHUDLayer::setLevelVal(const std::string &val)
{
    this->lblLevelValue->setString(val);
}