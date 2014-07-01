//
//  Grid.cpp
//  2048
//
//  Created by agile on 14-6-24.
//
//

#include "Grid.h"

Grid::Grid(int sizeX, int sizeY):field(sizeX,std::vector<TileOfCell*>(sizeY)),lastField(sizeX,std::vector<TileOfCell*>(sizeY))
{

    this->sizeX=sizeX;
    this->sizeY=sizeY;
    for (size_t i=0; i!=sizeX; i++) {
        for (size_t j=0; j!=sizeY; j++) {
            field[i][j]=nullptr;
            lastField[i][j]=nullptr;
        }
    }
    
}

Cell * Grid::randomAvailableCell()
{
    std::vector<Cell *> availableCells=getAvailableCells();
//    srand( (unsigned)time( NULL ) );
    if (availableCells.size()>=1) {
        return availableCells[rand()%availableCells.size()];
    }else
    {
    
        return nullptr;
    }
    
}

std::vector<Cell *> Grid::getAvailableCells()
{
    std::vector<Cell *> availableCells;
    for (size_t i=0; i!=field.size(); i++) {
        for (size_t j=0; j!=field.size(); j++) {
            if (field[i][j]==nullptr) {
                Cell *temp=new Cell(i,j);
                temp->autorelease();
                availableCells.push_back(temp);
            }
            
        }
    }
    return availableCells;
    
}

bool Grid::isCellsAvailable()
{

    return (getAvailableCells().size() >= 1);
}

bool Grid::isCellAvailable(TileOfCell *cell)
{
    return !isCellOccupied(cell);
}

bool Grid::isCellAvailable(int x,int y)
{
    return !isCellOccupied(x,y);
}


bool Grid::isCellWithinBounds(Cell cell)
{
    return  0 <= cell.getX() && cell.getX() < field.size()
    && 0 <= cell.getY() && cell.getY() < field[0].size();
}


bool Grid::isCellWithinBounds(int x,int y)
{

    return 0 <= x && x < field.size()
    && 0 <= y && y < field[0].size();

}

TileOfCell * Grid::getCellContent(int x, int y)
{
    if (isCellWithinBounds(x, y)) {
        return field[x][y];
    } else {
        return nullptr;
    }
}

TileOfCell * Grid::getCellContent(TileOfCell *cell)
{

    if (cell) {
        int x=cell->getX();
        int y=cell->getY();
        if (isCellWithinBounds(x, y))
        {

            return field[x][y];
        } else {
            return nullptr;
        }
    }else
    {
        return nullptr;
    }
    
}

bool Grid::isCellOccupied(TileOfCell *cell)
{
    if (cell)
    {
        return ( getCellContent(cell)!= nullptr);

    }else
    {
        return false;
    }
}
    

bool Grid::isCellOccupied(int x, int y)
{
    return (getCellContent(x,y)!=nullptr);
}

void Grid::insertTile(TileOfCell *tile)
{
    if (tile) {
        CC_SAFE_RETAIN(tile);
        CC_SAFE_RELEASE(field[tile->getX()][tile->getY()]);
        field[tile->getX()][tile->getY()]=tile;
        field[tile->getX()][tile->getY()]->setX(tile->getX());
        field[tile->getX()][tile->getY()]->setY(tile->getY());
        field[tile->getX()][tile->getY()]->setMergedFrom(tile->getMergedFrom());
        field[tile->getX()][tile->getY()]->setPreviousPosition(tile->getPreviousPosition());
    }
    
}

void Grid::removeTile(TileOfCell *tile)
{

    if (tile) {
        CC_SAFE_RELEASE(field[tile->getX()][tile->getY()]);
        field[tile->getX()][tile->getY()]=nullptr;
    }
}

void Grid::removeTile(int x,int y)
{
    
    if (isCellWithinBounds(x, y)) {
        CC_SAFE_RELEASE_NULL(field[x][y]);
    }
}

void Grid::saveTiles()
{
    canRevert = true;
    for (size_t xx = 0; xx != field.size(); xx++) {
        for (size_t yy = 0; yy < field.size(); yy++) {
            if (field[xx][yy] == nullptr) {
                lastField[xx][yy] = nullptr;
            } else {
                TileOfCell *tile=new TileOfCell(xx, yy, field[xx][yy]->getValue());
                tile->autorelease();
                tile->retain();
                CC_SAFE_RELEASE_NULL(lastField[xx][xx]);
                lastField[xx][yy] = tile;
            }
        }
    }
}

void Grid::revertTiles()
{
    canRevert = false;
    for (size_t xx = 0; xx < lastField.size(); xx++) {
        for (size_t yy = 0; yy < lastField.size(); yy++) {
            if (lastField[xx][yy] == nullptr) {
                field[xx][yy] = nullptr;
            } else {
                TileOfCell *tile=new TileOfCell(xx, yy, lastField[xx][yy]->getValue());
                tile->autorelease();
                tile->retain();
                CC_SAFE_RELEASE_NULL(field[xx][xx]);
                field[xx][yy] = tile;
            }
        }
    }
}

