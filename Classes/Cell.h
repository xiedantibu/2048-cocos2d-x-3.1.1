//
//  Cell.h
//  2048
//
//  Created by agile on 14-6-25.
//
//

#ifndef ___048__Cell__
#define ___048__Cell__

#include "cocos2d.h"
USING_NS_CC;

class Cell :public Ref
{
public:
    bool marked=false;
    int x=-1;
    int y=-1;
public:
    Cell(int xx,int yy):x(xx),y(yy){}
    Cell(Cell &c){x=c.getX();y=c.getY();}
    virtual ~Cell(){}
    inline void print(){log("cell++++++x=%i+++++y=%i",x,y);}
    virtual void setX(int xx){x=xx;}
    virtual void setY(int yy){y=yy;}

    virtual int getX(){return x;}
    virtual int getY(){return y;}
    bool operator==(const Cell cell)
    {
        if (this->x==cell.x&&this->y==cell.y) {
            return  true;
        }else
        {
        
            return false;
        }
    }
};

#endif /* defined(___048__Cell__) */
