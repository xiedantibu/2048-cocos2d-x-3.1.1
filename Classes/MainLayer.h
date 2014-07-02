//
// MainLayer.h
// 2048
//
// Created by agile on 14-6-23.
//
//

#ifndef ___048__MainLayer__
#define ___048__MainLayer__

#include "cocos2d.h"
#include "Grid.h"
USING_NS_CC;

class MainLayer : public Layer
{
public:
    MainLayer();
    ~MainLayer();
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MainLayer);
    
    void newGame();
    long getHighScore();
    void recordHighScore();
    void addRandomTile(Cell *cell);
    void addRandomTile();
    void addStartTiles();
    void createSprite(TileOfCell *tile);
    float pathMoved();
    bool move (int direction);
    void saveState();
    void buildTraversalsX(Cell *vector,std::vector<int> &);
    void buildTraversalsY(Cell *vector,std::vector<int> &);
    void prepareTiles();
    std::vector<Vec2> findFarthestPosition(int xx,int yy, Cell *vector);
    void moveTile(TileOfCell *tile,  Vec2 cell);
    bool positionsEqual(TileOfCell *first, Vec2 second);
    bool tileMatchesAvailable();
    bool movesAvailable();
    void buildExitLayer();
    void removeNode(Node *node);
    void exitApp();
    void isNodeCreate(int flag);
    void restartGame(int flag);
    void removeSprites();
    void buildGameOver();
    void buildSuccess();
    void changeMaxScore();
private:
    void initData();
    float width=0.0f;
    LayerColor * mLayerBg;
    float mSpriteScale=1.0f;
    int sizeX=0;
    int sizeY=0;
    std::vector<std::vector<Vec2>> mRePosition;
    std::vector<std::vector<ProgressTimer *>> mSprites;
    std::map<int,Color4B> mBgColors;
    std::map<int,Color3B> mFontColors;
    Grid grid;
    long score = 0;
    long lastScore = 0;
    long highScore = 0;
    long tempScore=0;
    bool won = false;
    bool lose = false;
    bool isMove=false;
    const int startTiles = 2;//开始显示几个Tiles
    Vec2 touchVec;
    Vec2 preVec;
    Vec2 startVec;
    float lastdx=0.0f;
    float lastdy=0.0f;
    int previousDirection = 1;
    int veryLastDirection = 1;
    int maxScore=2048;
    bool moved = false;

    int flag=0;
    std::queue<std::vector<Vec2>> mqueue;
    Cell *map[4]={new Cell(0, 1)/**上滑*/,new Cell(1, 0)/**右滑*/,new Cell(0, -1)/**下滑*/,new Cell(-1, 0)/**左滑*/};
};

#endif /* defined(___048__MainLayer__) */