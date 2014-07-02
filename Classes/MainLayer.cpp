//
// MainLayer.cpp
// 2048
//
// Created by agile on 14-6-23.
//
//

#include "MainLayer.h"
#include "Resource.h"
#include "VisibleRect.h"
#include "Tile.h"

#define SWIPE_MIN_DISTANCE 10
#define SWIPE_THRESHOLD_VELOCITY 40
#define MOVE_THRESHOLD 250

MainLayer::MainLayer()
{
    sizeX=Configuration::getInstance()->getValue(M_FILE_KEY_SIZEX).asInt();
    sizeY=Configuration::getInstance()->getValue(M_FILE_KEY_SIZEY).asInt();
    mRePosition.resize(sizeX);
    mSprites.resize(sizeX);

    mBgColors.insert({2,Color4B(238, 228, 218,255)});
    mBgColors.insert({4,Color4B(237, 224, 200,255)});
    mBgColors.insert({8,Color4B(242, 177, 121,255)});
    mBgColors.insert({16,Color4B(235, 141, 84,255)});
    mBgColors.insert({32,Color4B(246, 123, 95,255)});
    mBgColors.insert({64,Color4B(246, 125, 98,255)});
    mBgColors.insert({128,Color4B(243, 203, 118,255)});
    mBgColors.insert({256,Color4B(240, 204, 90,255)});
    mBgColors.insert({512,Color4B(237, 204, 99,255)});
    mBgColors.insert({1024,Color4B(237, 197, 63,255)});
    mBgColors.insert({2048,Color4B(236, 196, 0,255)});
    
    mFontColors.insert({2,Color3B(115,109,98)});
    mFontColors.insert({4,Color3B(115,109,98)});
    mFontColors.insert({8,Color3B(254,254,254)});
    mFontColors.insert({16,Color3B(255,255,255)});
    mFontColors.insert({32,Color3B(254,254,254)});
    mFontColors.insert({64,Color3B::WHITE});
    mFontColors.insert({128,Color3B::WHITE});
    mFontColors.insert({256,Color3B::WHITE});
    mFontColors.insert({512,Color3B::WHITE});
    mFontColors.insert({1024,Color3B::WHITE});
    mFontColors.insert({2048,Color3B(254,254,254)});
}

MainLayer::~MainLayer()
{

    CC_SAFE_RELEASE_NULL(mLayerBg);
    for (size_t i=0; i!=sizeX; i++) {
        for (size_t j=0; j!=sizeY; j++) {
            if (mSprites[i][j]) {
                mSprites[i][j]->removeFromParentAndCleanup(true);
            }
            CC_SAFE_RELEASE_NULL(mSprites[i][j]);
            CC_SAFE_RELEASE_NULL(grid.field[i][j]);
            CC_SAFE_RELEASE_NULL(grid.lastField[i][j]);
        }
    }
    for (size_t i=0; i!=4; i++) {
        CC_SAFE_FREE(map[0]);
    }
}

Scene* MainLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = MainLayer::create();
    scene->addChild(layer);
    return scene;
}

bool MainLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    initData();
    return true;
}

#pragma mark - initData
void MainLayer::initData()
{
    auto mListener=EventListenerTouchOneByOne::create();
    mListener->setSwallowTouches(true);
    mListener->onTouchBegan=CC_CALLBACK_2(MainLayer::onTouchBegan, this);
    mListener->onTouchEnded=CC_CALLBACK_2(MainLayer::onTouchEnded, this);
    mListener->onTouchMoved=CC_CALLBACK_2(MainLayer::onTouchMoved,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mListener, this);
    
    auto mKeyBackPress=EventListenerKeyboard::create();
    mKeyBackPress->onKeyReleased=CC_CALLBACK_2(MainLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mKeyBackPress, this);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    mLayerBg=LayerColor::create(Color4B(246, 246, 238, 255), visibleSize.width, visibleSize.height);
    mLayerBg->setPosition(0, 0);
    mLayerBg->retain();
    this->addChild(mLayerBg,M_TAG_LAYER_BG);
    
    Sprite *sprite=Sprite::create(M_IMG_BGPL);
    sprite->setColor(Color3B(181, 170,156));
    sprite->setAnchorPoint(Vec2(0.5,0));
    sprite->setScale((visibleSize.width-40)/sprite->getContentSize().width);
    sprite->setPosition(Vec2(visibleSize.width/2, visibleSize.height/10));
    mLayerBg->addChild(sprite);
    log("x:%f=====y:%f==========width:%f========height:%f",sprite->getBoundingBox().origin.x,sprite->getBoundingBox().origin.y,sprite->getBoundingBox().size.width,sprite->getBoundingBox().size.height);
    
    
    TTFConfig config(M_FONT_CLEARSANS,50);
    auto label=Label::createWithTTF(config, "2048");
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setPosition(visibleSize.width/4,visibleSize.height/10*9);
    label->setColor(Color3B(115,109,98));
    label->setScale(visibleSize.width/3/label->getBoundingBox().size.width);
    mLayerBg->addChild(label);
    
    Sprite *scoreSprite=Sprite::create(M_IMG_SCORE);
    scoreSprite->setScale(((visibleSize.width)/2-25)/scoreSprite->getContentSize().width/2);
    scoreSprite->setPosition(visibleSize.width*5/8,visibleSize.height/10*9);
    scoreSprite->setColor(Color3B(189,174,156));
    scoreSprite->setTag(M_TAG_SCORE_SPRITE);
    mLayerBg->addChild(scoreSprite,1);
    
    TTFConfig scoreConfig(M_FONT_CLEARSANS,30);
    auto scoreLabel=Label::createWithTTF(scoreConfig, "SCORE");
    scoreLabel->setAnchorPoint(Vec2(0.5,1));
    scoreLabel->setScale(scoreSprite->getBoundingBox().size.height/2/scoreLabel->getBoundingBox().size.height);
    scoreLabel->setPosition(visibleSize.width*5/8,scoreSprite->getBoundingBox().origin.y+scoreSprite->getBoundingBox().size.height);
    scoreLabel->setColor(Color3B(238,226,213));
    mLayerBg->addChild(scoreLabel,2);
    
    TTFConfig scoreConfig2(M_FONT_CLEARSANS,30);
    auto scoreLabel2=Label::createWithTTF(scoreConfig2, "0");
    scoreLabel2->setScale(scoreSprite->getBoundingBox().size.height/2/scoreLabel2->getBoundingBox().size.height);
    scoreLabel2->setAnchorPoint(Vec2(0.5,0));
    scoreLabel2->setPosition(visibleSize.width*5/8,scoreSprite->getBoundingBox().origin.y);
    scoreLabel2->setColor(Color3B(255,255,255));
    scoreLabel2->setTag(M_TAG_SCORE);
    mLayerBg->addChild(scoreLabel2,2);

    
    Sprite *hightSprite=Sprite::create(M_IMG_SCORE);
    hightSprite->setScale(((visibleSize.width)/2-25)/scoreSprite->getContentSize().width/2);
    hightSprite->setPosition(visibleSize.width*7/8,visibleSize.height/10*9);
    hightSprite->setColor(Color3B(189,174,156));
//    hightSprite->setTag(M_TAG_HIGH_SCORE);
    mLayerBg->addChild(hightSprite,1);
    
    TTFConfig highscoreConfig(M_FONT_CLEARSANS,30);
    auto highscoreLabel=Label::createWithTTF(highscoreConfig, "BEST");
    highscoreLabel->setAnchorPoint(Vec2(0.5,1));
    highscoreLabel->setScale(MIN(hightSprite->getBoundingBox().size.height/2/highscoreLabel->getBoundingBox().size.height, (hightSprite->getBoundingBox().size.width-5)/highscoreLabel->getBoundingBox().size.width));
    highscoreLabel->setPosition(visibleSize.width*7/8,hightSprite->getBoundingBox().origin.y+scoreSprite->getBoundingBox().size.height);
    highscoreLabel->setColor(Color3B(238,226,213));
    mLayerBg->addChild(highscoreLabel,2);
    
    TTFConfig highscoreConfig2(M_FONT_CLEARSANS,30);
    long highScores=getHighScore();
    std::string score=__String::createWithFormat("%li",highScores)->getCString();
    auto highscoreLabel2=Label::createWithTTF(highscoreConfig2, score);
    highscoreLabel2->setScale(hightSprite->getBoundingBox().size.height/2/highscoreLabel2->getBoundingBox().size.height);
    highscoreLabel2->setAnchorPoint(Vec2(0.5,0));
    highscoreLabel2->setPosition(visibleSize.width*7/8,hightSprite->getBoundingBox().origin.y);
    highscoreLabel2->setColor(Color3B(255,255,255));
    highscoreLabel2->setTag(M_TAG_HIGH_SCORE);
    mLayerBg->addChild(highscoreLabel2,2);


    
    TTFConfig configReset(M_FONT_CLEARSANS,20);
    auto resetButton=Label::createWithTTF(configReset,"RESTART");
    resetButton->setColor(Color3B(225,125,0));
    auto menuLabel=MenuItemLabel::create(resetButton, CC_CALLBACK_0(MainLayer::restartGame, this,1));
    menuLabel->setAnchorPoint(Vec2(0,0));
    auto menu=Menu::create(menuLabel, NULL);
    menu->setPosition(Vec2(sprite->getBoundingBox().origin.x+20,sprite->getBoundingBox().size.height+sprite->getBoundingBox().origin.y+10));
    mLayerBg->addChild(menu);
    
    width=(visibleSize.width-65)/sizeX;
    mSpriteScale=width/sprite->getContentSize().width*4;
    for (size_t i=0; i<sizeX; i++) {
        mRePosition[i].resize(sizeY);
        mSprites[i].resize(sizeY);
        for (size_t j=0; j<sizeY; j++) {
            Sprite *spriteItem=Sprite::create(M_IMG_BGB);
            spriteItem->setColor(Color3B(205, 194, 181));
            spriteItem->setScale(mSpriteScale);
            spriteItem->setPosition(Vec2(sprite->getBoundingBox().origin.x+5*(i+1)+(2*i+1)*width/2,sprite->getBoundingBox().origin.y+5*(j+1)+(2*j+1)*width/2));
            mLayerBg->addChild(spriteItem,M_ZORDER_spriteItem);
            mRePosition[i][j]=spriteItem->getPosition();
        }
        
    }
    
    srand((unsigned)time( NULL ));
    newGame();
//    buildGameOver();
}


#pragma mark - 游戏开始
void MainLayer::newGame()
{
    grid=Grid(sizeX, sizeY);
    highScore = getHighScore();
    if (score >= highScore) {
        highScore = score;
        recordHighScore();
    }
    score = 0;
    maxScore=2048;
    auto scoreLabel2=(Label *)mLayerBg->getChildByTag(M_TAG_SCORE);
    scoreLabel2->setString(__String::createWithFormat("%li",score)->getCString());
    
    auto highLabel=(Label *)mLayerBg->getChildByTag(M_TAG_HIGH_SCORE);
    highLabel->setString(__String::createWithFormat("%li",highScore)->getCString());

    won = false;
    lose = false;
    addStartTiles();

}
void MainLayer::addStartTiles()
{
    for (int xx = 0; xx < startTiles; xx++) {
        addRandomTile();
    }

}

void MainLayer::restartGame(int flag)
{
    Node *node=this->getChildByTag(M_TAG_Exit);
    if (node) {
        Action *action=Sequence::create(FadeOut::create(0.5f),CallFuncN::create(CC_CALLBACK_0(MainLayer::removeNode, this,node)), NULL);
        node->runAction(action);
        return ;
    }

    Node *gameOver=this->getChildByTag(M_TAG_GAME_OVER);
    if (gameOver) {
        if (flag!=2) {
            
            return;
        }
        removeNode(gameOver);
    }
    
    Node *gameSuccess=this->getChildByTag(M_TAG_GAME_SUCCESS);
    if (gameSuccess) {
        if (flag!=3) {
            
            return;
        }
        removeNode(gameSuccess);
    }

    for (std::vector<std::vector<ProgressTimer *>>::iterator iter=mSprites.begin();iter!=mSprites.end();iter++) {
        for (std::vector<ProgressTimer *>::iterator itter=(*iter).begin(); itter!=(*iter).end(); itter++) {
            if((*itter))
            {
            (*itter)->removeFromParentAndCleanup(true);
            }
            CC_SAFE_RELEASE_NULL(*itter);
        }
    }

    newGame();
}
void MainLayer::createSprite(TileOfCell *tile)
{

    tile->print();
    ProgressTo *to = ProgressTo::create(0.2, 100);
    Sprite *spriteItem=Sprite::create(M_IMG_BGB);
    spriteItem->setScale(mSpriteScale);
    spriteItem->setCascadeColorEnabled(false);
    
    ProgressTimer *middle = ProgressTimer::create(spriteItem);
    middle->setCascadeColorEnabled(false);
    middle->setType(ProgressTimer::Type::BAR);
    middle->setMidpoint(Vec2(0.5f, 0.5f));
    middle->setBarChangeRate(Vec2(0.3,0.3));
    middle->setScale(mSpriteScale);
    middle->setTag(M_TAG_SPRITES);
    middle->setPosition(mRePosition[tile->getX()][tile->getY()]);
    Vec2 vec=middle->convertToNodeSpace(middle->convertToWorldSpace(Vec2(0, 0)));
    
    LayerColor *mLayer=LayerColor::create(mBgColors[tile->getValue()], middle->getContentSize().width, middle->getContentSize().height);
    mLayer->setPosition(vec+Vec2(middle->getContentSize().width/2, middle->getContentSize().height/2));
    mLayer->ignoreAnchorPointForPosition(false);
    mLayer->setCascadeColorEnabled(false);
    mLayer->setTag(M_TAG_LAYERBG);
    middle->addChild(mLayer,M_ZORDER_layerbg);
    
    TTFConfig config(M_FONT_CLEARSANS,40);
    auto label=Label::createWithTTF(config, __String::createWithFormat("%i",tile->getValue())->getCString());
    label->setPosition(vec+Vec2(middle->getContentSize().width/2, middle->getContentSize().height/2));
    label->setColor(mFontColors[tile->getValue()]);
    label->setTag(M_TAG_LABEL);
    label->setCascadeColorEnabled(false);
    middle->addChild(label,M_ZORDER_label);
    
    middle->runAction(to);
    mLayerBg->addChild(middle,M_ZORDER_middle);
    middle->retain();
    mSprites[tile->getX()][tile->getY()]=middle;

}

#pragma mark - RandomTile
void MainLayer::addRandomTile()
{
    if (grid.isCellsAvailable()) {
        addRandomTile(grid.randomAvailableCell());
    }
}
void MainLayer::addRandomTile(Cell * cell)
{
    int value=(CCRANDOM_0_1())<0.9?2:4;
    TileOfCell *tile=new TileOfCell(cell, value);
    tile->autorelease();
//    tile->retain();
    grid.insertTile(tile);
    createSprite(tile);
}

#pragma mark - Score
long MainLayer::getHighScore()
{

    highScore=UserDefault::getInstance()->getIntegerForKey(M_FILE_KEY_HIGHT_SCORE, 0);
    return highScore;
}

void MainLayer::recordHighScore()
{
    UserDefault::getInstance()->setIntegerForKey(M_FILE_KEY_HIGHT_SCORE, highScore);
}


float MainLayer::pathMoved()
{

    return (touchVec.x - startVec.x) * (touchVec.x - startVec.x) + (touchVec.y - startVec.y) * (touchVec.y - startVec.y);
}


bool MainLayer::move (int direction)
{

    if(!mqueue.empty())
    {
        return false;
    }

    saveState();
    if (lose||won) {
        return false;
    }
    
    Cell *vector=map[direction];
    std::vector<int> buildX,buildY;
    buildTraversalsX(vector,buildX);
    buildTraversalsY(vector, buildY);
    bool moved = false;
    prepareTiles();
    tempScore=0;
    for (auto xx:buildX) {
        
        for (auto yy: buildY) {
            
            TileOfCell * tile=grid.getCellContent(xx,yy);
            
            if (tile) {
                std::vector<Vec2> mCell=findFarthestPosition(xx, yy, vector);
                TileOfCell *next=grid.getCellContent(mCell[1].x,mCell[1].y);
                if (next != nullptr && next->getValue() == tile->getValue() && next->getMergedFrom().size()== 0)//合并
                {
                
                    moved=true;
                    TileOfCell *merged=new TileOfCell(mCell[1].x,mCell[1].y,tile->getValue()*2);
                    merged->autorelease();

                    next->retain();
                    tile->retain();
                    std::vector<Vec2> temp={Vec2(tile->x, tile->y),Vec2(next->x, next->y)};
                    merged->setMergedFrom(temp);
                    merged->setPreviousPosition(tile->getX(),tile->getY());
                    grid.insertTile(merged);
                    grid.removeTile(tile);

                    if (mSprites[merged->getPreviousPosition()->getX()][merged->getPreviousPosition()->getY()]) {
                        log("===========1111=====");
                        mSprites[merged->getPreviousPosition()->getX()][merged->getPreviousPosition()->getY()]->runAction(MoveTo::create(0.15f, mRePosition[merged->getX()][merged->getY()]));
                        
                        std::vector<Vec2> mVec;
                        mVec.push_back(Vec2(merged->getX(), merged->getY()));
                        mVec.push_back(Vec2(merged->getPreviousPosition()->getX(),merged->getPreviousPosition()->getY()));
                        mVec.push_back(Vec2(0,-1));

                        mqueue.push(mVec);
                    }


                    score+=merged->getValue();
                    tempScore+=merged->getValue();
                    highScore=MAX(score, highScore);
                    recordHighScore();
                    
                    auto scoreLabel2=(Label *)mLayerBg->getChildByTag(M_TAG_SCORE);
                    scoreLabel2->setString(__String::createWithFormat("%li",score)->getCString());
                    
                    auto highLabel=(Label *)mLayerBg->getChildByTag(M_TAG_HIGH_SCORE);
                    highLabel->setString(__String::createWithFormat("%li",highScore)->getCString());
                    
                    if (merged->getValue()==maxScore) {
                        won=true;
                    }
                }else
                {
                
                    if (!positionsEqual(tile, mCell[0])) {//移动
                        log("===========22222=========");
                        moved=true;
                        moveTile(tile, mCell[0]);
                        
                        if (mSprites[tile->getPreviousPosition()->getX()][tile->getPreviousPosition()->getY()]) {
                            mSprites[tile->getPreviousPosition()->getX()][tile->getPreviousPosition()->getY()]->runAction(MoveTo::create(0.15f, mRePosition[mCell[0].x][mCell[0].y]));

                            std::vector<Vec2> mVec;
                            mVec.push_back(Vec2(mCell[0].x, mCell[0].y));
                            mVec.push_back(Vec2(tile->getPreviousPosition()->getX(),tile->getPreviousPosition()->getY()));
                            mVec.push_back(Vec2(0,-2));

                            mqueue.push(mVec);

                        }

                    }
                    
                }
                
                
                
            }

        }
        if (xx==buildX[buildX.size()-1]&&moved) {
            if (tempScore!=0) {
                auto scoreSprite=(Sprite *)mLayerBg->getChildByTag(M_TAG_SCORE_SPRITE);
                TTFConfig scoreConfig(M_FONT_CLEARSANS,30);
                Label *tempLabel=(Label *)mLayerBg->getChildByTag(M_TAG_TEMP);
                if (tempLabel) {
                    tempLabel->setString(__String::createWithFormat("+%li",tempScore)->getCString());
                }else
                {
                   tempLabel=Label::createWithTTF(scoreConfig, __String::createWithFormat("+%li",tempScore)->getCString());
                    tempLabel->setColor(Color3B::RED);
                    mLayerBg->addChild(tempLabel, M_TAG_TEMP);
                }
                tempLabel->setPosition(scoreSprite->getPosition());
                tempLabel->setTag(M_TAG_TEMP);
                Action *actionTemp=Sequence::create(FadeIn::create(0.1f),MoveBy::create(0.3f, Vec2(0,10)),FadeOut::create(0.1), NULL);
                tempLabel->runAction(actionTemp);
                

            }
            
            Action *action=
            Sequence::create(DelayTime::create(0.2f),CallFuncN::create(CC_CALLBACK_0(MainLayer::removeSprites, this)), NULL);
            this->runAction(action);

        }
    }
    if (won) {
        buildSuccess();
    }
    if (!movesAvailable()) {
        lose = true;
        buildGameOver();

    }


}

void MainLayer::removeSprites()
{
    while (!mqueue.empty()) {

        std::vector<Vec2> mtemp=mqueue.front();
        if (mSprites[mtemp[0].x][mtemp[0].y]) {
            log("===========555,4======%f==%f=",mtemp[0].x,mtemp[0].y);
            mSprites[mtemp[0].x][mtemp[0].y]->removeFromParentAndCleanup(true);
            CC_SAFE_RELEASE_NULL(mSprites[mtemp[0].x][mtemp[0].y]);
        }
        
        mSprites[mtemp[0].x][mtemp[0].y]=mSprites[mtemp[1].x][mtemp[1].y];
        mSprites[mtemp[1].x][mtemp[1].y]=nullptr;
        
        if (mtemp[2].y==-1) {
            if (grid.field[mtemp[0].x][mtemp[0].y]&&mSprites[mtemp[0].x][mtemp[0].y]) {
                int value=grid.field[mtemp[0].x][mtemp[0].y]->getValue();
                Label  *label=(Label *)mSprites[mtemp[0].x][mtemp[0].y]->getChildByTag(M_TAG_LABEL);
                label->setColor(mFontColors[value]);
                label->setString(__String::createWithFormat("%i",value)->getCString());
                LayerColor *mLayer=(LayerColor *)mSprites[mtemp[0].x][mtemp[0].y]->getChildByTag(M_TAG_LAYERBG);
                
                mLayer->setColor(Color3B(mBgColors[value]));
                
            }
        }
        mqueue.pop();
    }
    
    addRandomTile();
}

bool MainLayer::movesAvailable()
{

    return grid.isCellsAvailable()||tileMatchesAvailable();
}

bool MainLayer::tileMatchesAvailable()
{

    for (size_t i=0; i!=sizeX; i++) {
        for (size_t j=0; j!=sizeY; j++) {
            TileOfCell *tile=grid.getCellContent(i, j);
            if (tile) {
                for (size_t direction=0; direction!=4; direction++) {
                    Cell *cell=map[direction];
                    TileOfCell *other=grid.getCellContent(i+cell->getX(), j+cell->getY());
                    if (other!=nullptr&&other->getValue()==tile->getValue()) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool MainLayer::positionsEqual(TileOfCell *first, Vec2 second)
{
    
    if (first) {
        return first->getX() == second.x && first->getY() == second.y;

    }else
    {
    
        return false;
    }
}
void MainLayer::moveTile(TileOfCell *tile,  Vec2 cell)
{
    grid.field[tile->getX()][tile->getY()]=nullptr;
    CC_SAFE_RELEASE_NULL(grid.field[cell.x][cell.y]);
    grid.field[cell.x][cell.y]=tile;
    tile->setPreviousPosition(tile->getX(),tile->getY());
    tile->updatePosition(cell);

}
std::vector<Vec2> MainLayer::findFarthestPosition(int xx,int yy, Cell *vector)
{
    Vec2 previous=Vec2(-1, -1);
    Vec2 nextCell=Vec2(xx, yy);
    do {
        previous = nextCell;
        nextCell=Vec2(previous.x+vector->getX(),previous.y+vector->getY());

    } while (grid.isCellWithinBounds(nextCell.x,nextCell.y) && grid.isCellAvailable(nextCell.x,nextCell.y));

    std::vector<Vec2> mCell={previous,nextCell};
    return mCell;
}
void MainLayer::saveState()
{
    grid.saveTiles();
    lastScore=score;
}

void MainLayer::buildTraversalsX(Cell *vector,std::vector<int> &vec)
{
    for (size_t i=0; i!=sizeX; i++) {
        {
            vec.push_back(i);
        }

    }
    std::vector<int> temp;
    if (vector->getX()==1) {
        
        for (std::vector<int>::reverse_iterator iter=vec.rbegin();iter!=vec.rend();iter++) {
            temp.push_back(*iter);
        }
        vec=temp;
    }
    
}
void  MainLayer::buildTraversalsY(Cell *vector,std::vector<int> &vec)
{

    for (size_t i=0; i!=sizeY; i++) {

            vec.push_back(i);
    }
    std::vector<int> temp;
    if (vector->getY()==1) {
        for (std::vector<int>::reverse_iterator iter=vec.rbegin();iter!=vec.rend();iter++) {
            temp.push_back(*iter);
        }
        vec=temp;
    }


}

void MainLayer::prepareTiles()
{
    
    for (size_t i=0; i!=sizeX; i++) {
        for (size_t j=0; j!=sizeY; j++) {
            TileOfCell *tile=grid.field[i][j];

            if (grid.isCellOccupied(tile)) {
                std::vector<Vec2> temp={};
                tile->setMergedFrom(temp);
                tile->savePosition();
            }

        }
    }
}

#pragma mark - onTouch
bool MainLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
    
    touchVec=touch->getLocation();
    preVec=touchVec;
    startVec=touchVec;
    lastdx=0.0f;
    lastdy=0.0f;
    moved=false;
    log("+onTouchBegan++++touchvec+x:%f++y:%f+++",touchVec.x,touchVec.y);
    return true;
}
void MainLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
    Node *node=this->getChildByTag(M_TAG_Exit);
    if (node) {
        Action *action=Sequence::create(FadeOut::create(0.2f),CallFuncN::create(CC_CALLBACK_0(MainLayer::removeNode, this,node)), NULL);
        node->runAction(action);
        return ;
    }
    
    Node *gameOver=this->getChildByTag(M_TAG_GAME_OVER);
    if (gameOver) {
        return;
    }
    
    Node *gamesuccess=this->getChildByTag(M_TAG_GAME_SUCCESS);
    if (gamesuccess) {
        return;
    }
    touchVec=touch->getLocation();
    log("+onTouchMoved++++touchvec+x:%f++y:%f+++",touchVec.x,touchVec.y);
    if (!won&&!lose) {

        float dx=touchVec.x-preVec.x;

        //horizonal
        if (fabsf(lastdx + dx)<(fabsf(lastdx)+fabsf(dx))&&fabsf(touchVec.x-startVec.x)>(SWIPE_MIN_DISTANCE))
        {

            startVec=touchVec;
            lastdx=dx;
            previousDirection=veryLastDirection;
            
        }
        if (lastdx==0.0f) {
            lastdx=dx;
        }

        if (!moved && pathMoved() > (SWIPE_MIN_DISTANCE) * (SWIPE_MIN_DISTANCE))
        {
        
            if (((dx>=(SWIPE_THRESHOLD_VELOCITY)&&previousDirection==1)||(touchVec.x-startVec.x)>=(MOVE_THRESHOLD))&&previousDirection%5!=0) {
                
                log("+onTouchMoved++++move111++");
                moved=true;
                previousDirection = previousDirection*5;
                veryLastDirection=5;
                move(1);
            }else if(((dx<=-(SWIPE_THRESHOLD_VELOCITY)&&previousDirection==1)||(touchVec.x-startVec.x)<=-(MOVE_THRESHOLD))&&previousDirection%7!=0)
            {
            
                log("+onTouchMoved++++move333++");
                moved=true;
                previousDirection=previousDirection*7;
                veryLastDirection=7;
                move(3);
            }
        }
        
        float dy=touchVec.y-preVec.y;

        if (fabsf(lastdy+dy)<(fabsf(lastdy)+fabsf(dy))&&fabsf(touchVec.y-startVec.y)>(SWIPE_MIN_DISTANCE)) {
            startVec=touchVec;
            lastdy=dy;
            previousDirection=veryLastDirection;
        }
        if (lastdy==0) {
            lastdy=dy;
        }
        if (!moved && pathMoved() > (SWIPE_MIN_DISTANCE) * (SWIPE_MIN_DISTANCE))
        {
            if (((dy>=(SWIPE_THRESHOLD_VELOCITY)&&previousDirection==1)||(touchVec.y-startVec.y)>=(MOVE_THRESHOLD))&&previousDirection%3!=0) {
                log("+onTouchMoved++++move000++");
                moved=true;
                previousDirection = previousDirection*3;
                veryLastDirection=3;
                move(0);
            }else if(((dy<=-(SWIPE_THRESHOLD_VELOCITY)&&previousDirection==1)||(touchVec.y-startVec.y)<=-(MOVE_THRESHOLD))&&previousDirection%2!=0)
            {
                log("+onTouchMoved++++move222++");
                moved=true;
                previousDirection=previousDirection*2;
                veryLastDirection=2;
                move(2);
            }
        }
        
        if (moved) {
            startVec=touchVec;
        }
        
    }
    preVec=startVec;
}
void MainLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
    touchVec=touch->getLocation();
    previousDirection=1;
    veryLastDirection=1;
    log("+onTouchEnded++++touchvec+x:%f++y:%f+++",touchVec.x,touchVec.y);


}

void MainLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{

    if (keyCode==EventKeyboard::KeyCode::KEY_ESCAPE) {
        
        Node *gameOver=this->getChildByTag(M_TAG_GAME_OVER);
        if (gameOver) {
            removeNode(gameOver);
            lose=false;
            return;
        }
        Node *gameSuccess=this->getChildByTag(M_TAG_GAME_SUCCESS);
        if (gameSuccess) {
            restartGame(3);
            return;
        }
        Node *node=this->getChildByTag(M_TAG_Exit);
        if (node) {
            Action *action=Sequence::create(FadeOut::create(0.5f),CallFuncN::create(CC_CALLBACK_0(MainLayer::removeNode, this,node)), NULL);
            node->runAction(action);
            return ;
        }
        
        buildExitLayer();
    }
}

void MainLayer::buildExitLayer()
{

    Size visibleSize = Director::getInstance()->getVisibleSize();
    LayerColor *mLayerExit=LayerColor::create(Color4B(255, 255, 255, 180), visibleSize.width, visibleSize.height*3/4);
    mLayerExit->setPosition(0, 0);
    mLayerExit->setTag(M_TAG_Exit);

    
    TTFConfig config(M_FONT_CLEARSANS,50);
    auto label=Label::createWithTTF(config, "Leave game?");
    label->setColor(Color3B(100,100,100));
    
    
    TTFConfig exitconfig(M_FONT_CLEARSANS,40);
    auto exit=Label::createWithTTF(exitconfig, "Exit");
    exit->setColor(Color3B(100,100,100));

    auto nope=Label::createWithTTF(exitconfig, "Nope");
    nope->setColor(Color3B(100,100,100));

    MenuItemLabel *menuLabel=MenuItemLabel::create(label);
    menuLabel->setEnabled(false);
    menuLabel->setPosition(0, 0);
    MenuItemLabel *exitLabel=MenuItemLabel::create(exit,CC_CALLBACK_0(MainLayer::exitApp, this));
    exitLabel->setPosition(-visibleSize.width/4,menuLabel->getPositionY()-menuLabel->getContentSize().height-30);
    MenuItemLabel *nopeLabel=MenuItemLabel::create(nope,CC_CALLBACK_0(MainLayer::isNodeCreate, this,2));
    nopeLabel->setPosition(visibleSize.width/4,menuLabel->getPositionY()-menuLabel->getContentSize().height-30);
    
    Menu *menu=Menu::create(menuLabel, exitLabel,nopeLabel,NULL);
    mLayerExit->addChild(menu);
    this->addChild(mLayerExit, M_TAG_Exit);
}
void MainLayer::exitApp()
{
    Director::getInstance()->end();
    
}
void MainLayer::isNodeCreate(int flag)
{
    
    if (flag==2) {
        Node *node=this->getChildByTag(M_TAG_Exit);
        if (node) {
            Action *action=Sequence::create(FadeOut::create(0.2f),CallFuncN::create(CC_CALLBACK_0(MainLayer::removeNode, this,node)), NULL);
            node->runAction(action);
            return ;
        }
    }
   
}

void MainLayer::buildGameOver()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    LayerColor *mLayerGameOver=LayerColor::create(Color4B(255, 255, 255, 180), visibleSize.width, visibleSize.height*3/4);
    mLayerGameOver->setPosition(0, 0);
    mLayerGameOver->setTag(M_TAG_GAME_OVER);
    
    TTFConfig config(M_FONT_CLEARSANS,45);
    auto label=Label::createWithTTF(config, "Game Over");
    label->setColor(Color3B(100,100,100));
    
    TTFConfig scoreconfig(M_FONT_CLEARSANS,20);
    auto scoreLabel=Label::createWithTTF(scoreconfig, __String::createWithFormat("Your score %li points",score)->getCString());
    scoreLabel->setColor(Color3B(100,100,100));
    int length=scoreLabel->getStringLength();
    for (size_t i=10; i<length-6; i++) {
        Sprite * temp=scoreLabel->getLetter(i);
        temp->setColor(Color3B(240, 204, 90));
    }
    TTFConfig tryAgainconfig(M_FONT_CLEARSANS,30);
    auto tryAgain=Label::createWithTTF(tryAgainconfig, "Try Again");
    tryAgain->setColor(Color3B(100,100,100));
    
    MenuItemLabel *menuLabel=MenuItemLabel::create(label);
    menuLabel->setEnabled(false);
    menuLabel->setPosition(0, 0);
    MenuItemLabel *scoreLabelMenu=MenuItemLabel::create(scoreLabel);
    scoreLabelMenu->setEnabled(false);
    scoreLabelMenu->setPosition(0,menuLabel->getPositionY()-menuLabel->getContentSize().height/2-10);
    MenuItemLabel *tryAgainLabel=MenuItemLabel::create(tryAgain,CC_CALLBACK_0(MainLayer::restartGame, this,2));
    tryAgainLabel->setPosition(0,scoreLabelMenu->getPositionY()-scoreLabelMenu->getContentSize().height-25);
    
    Menu *menu=Menu::create(menuLabel, scoreLabelMenu,tryAgainLabel,NULL);
    mLayerGameOver->addChild(menu);
    this->addChild(mLayerGameOver, M_TAG_GAME_OVER);

}

void MainLayer::removeNode(Node *node)
{

    if (node) {
        node->removeFromParentAndCleanup(true);
    }
    
}

void MainLayer::buildSuccess()
{

    Size visibleSize = Director::getInstance()->getVisibleSize();
    LayerColor *mLayerExit=LayerColor::create(Color4B(255, 255, 255, 180), visibleSize.width, visibleSize.height*3/4);
    mLayerExit->setPosition(0, 0);
    mLayerExit->setTag(M_TAG_GAME_SUCCESS);
    
    
    TTFConfig config(M_FONT_CLEARSANS,40);
    auto label=Label::createWithTTF(config, "You are Win !");
    label->setColor(Color3B(100,100,100));
    
    
    TTFConfig exitconfig(M_FONT_CLEARSANS,35);
    auto exit=Label::createWithTTF(exitconfig, "CONT");
    exit->setColor(Color3B(100,100,100));
    
    auto nope=Label::createWithTTF(exitconfig, "Nope");
    nope->setColor(Color3B(100,100,100));
    
    MenuItemLabel *menuLabel=MenuItemLabel::create(label);
    menuLabel->setEnabled(false);
    menuLabel->setPosition(0, 0);
    MenuItemLabel *exitLabel=MenuItemLabel::create(exit,CC_CALLBACK_0(MainLayer::changeMaxScore, this));
    exitLabel->setPosition(-visibleSize.width/4,menuLabel->getPositionY()-menuLabel->getContentSize().height-30);
    MenuItemLabel *nopeLabel=MenuItemLabel::create(nope,CC_CALLBACK_0(MainLayer::restartGame, this,3));
    nopeLabel->setPosition(visibleSize.width/4,menuLabel->getPositionY()-menuLabel->getContentSize().height-30);
    
    Menu *menu=Menu::create(menuLabel, exitLabel,nopeLabel,NULL);
    mLayerExit->addChild(menu);
    this->addChild(mLayerExit, M_TAG_GAME_SUCCESS);
}

void MainLayer::changeMaxScore()
{
    maxScore*=2;
    won=false;
    Node *node=this->getChildByTag(M_TAG_GAME_SUCCESS);
    if (node) {
    
        Action *action=Sequence::create(FadeOut::create(0.2f),CallFuncN::create(CC_CALLBACK_0(MainLayer::removeNode, this,node)), NULL);
        node->runAction(action);
    }

}
