//
//  Tile.h
//  2048
//
//  Created by agile on 14-6-25.
//
//

#ifndef ___048__Tile__
#define ___048__Tile__

#include <iostream>
#include "Cell.h"
#include "cocos2d.h"
USING_NS_CC;

class TileOfCell :public Cell
{

private:
    Cell *previousPosition;
    std::vector<Vec2> mergedFrom;
    int value;
public:
    TileOfCell(int x,int y,int value):Cell(x,y),value(value)
    {
        previousPosition=new Cell(-1,-1);
    }
    TileOfCell(Cell *cell,int value):Cell(cell->getX(),cell->getY()),value(value)
    {
        previousPosition=new Cell(-1,-1);

    }
    TileOfCell():Cell(-1,-1),value(-1){
        previousPosition=new Cell(-1,-1);
    }
    TileOfCell(TileOfCell &):Cell(-1,-1),value(-1){
    
        previousPosition=new Cell(-1,-1);

    }
    ~TileOfCell(){
    
        CC_SAFE_RELEASE_NULL(previousPosition);

    }
    
    inline void setValue(int v){value=v;}
    inline int getValue(){return value;}
    void savePosition();
    void updatePosition(Vec2 cell);
    std::vector<Vec2> getMergedFrom();
    void setMergedFrom(std::vector<Vec2> tile);
    Cell * getPreviousPosition();
    void setPreviousPosition(Cell * previousPosition);
    void setPreviousPosition(int x,int y);

};

#endif /* defined(___048__Tile__) */
