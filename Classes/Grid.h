//
//  Grid.h
//  2048
//
//  Created by agile on 14-6-24.
//
//

#ifndef ___048__Grid__
#define ___048__Grid__

#include <iostream>
#include "cocos2d.h"
#include "Tile.h"

USING_NS_CC;

class Grid:public Ref
{
public:
    std::vector<std::vector<TileOfCell*> > field;
    std::vector<std::vector<TileOfCell*> > lastField;
    bool canRevert = false;
    int sizeX=0;
    int sizeY=0;
public:
    Grid(int sizeX, int sizeY);
    Grid()=default;
    Cell * randomAvailableCell();
    std::vector<Cell *> getAvailableCells();
    bool isCellsAvailable();
    bool isCellAvailable(TileOfCell *cell);
    bool isCellAvailable(int x,int y);
    bool isCellWithinBounds(Cell cell);
    bool isCellWithinBounds(int x,int y);
    TileOfCell * getCellContent(int x, int y);
    TileOfCell * getCellContent(TileOfCell * cell);
    bool isCellOccupied(TileOfCell * cell);
    bool isCellOccupied(int x, int y);
    void insertTile(TileOfCell *tile);
    void removeTile(TileOfCell *tile);
    void removeTile(int x, int y);
    void saveTiles();
    void revertTiles();
};

#endif /* defined(___048__Grid__) */
