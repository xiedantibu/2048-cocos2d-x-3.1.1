//
//  Tile.cpp
//  2048
//
//  Created by agile on 14-6-25.
//
//

#include "Tile.h"

void TileOfCell::savePosition()
{

    if (previousPosition) {
        previousPosition->setX(this->getX());
        previousPosition->setY(this->getY());
        previousPosition->marked=this->marked;
    }else
    {
    
       previousPosition=new Cell(this->getX(),this->getY());
       previousPosition->marked=this->marked;
    }
}

void TileOfCell::updatePosition(Vec2 cell)
{
    this->setX(cell.x);
    this->setY(cell.y);
}

std::vector<Vec2> TileOfCell::getMergedFrom()
{

    return mergedFrom;
}
void TileOfCell::setMergedFrom(std::vector<Vec2> tile)
{
    mergedFrom = tile;
}
Cell * TileOfCell::getPreviousPosition()
{
    return previousPosition;
}

void TileOfCell::setPreviousPosition(Cell *c)
{

    previousPosition=c;

}

void TileOfCell::setPreviousPosition(int x,int y)
{

    if (previousPosition) {
        previousPosition->setX(x);
        previousPosition->setY(y);
    }else
    {
        
        previousPosition=new Cell(x,y);
    }
}
