#include "bucket.h"
#include <QColor>

Bucket::Bucket(QColor color, Canvas* canvas) : Tool(color, canvas)
{

}

Bucket::~Bucket()
{

}

void Bucket::perform(int x, int y)
{
    changingColor = getCurrentFrame()->getPixel(x,y);

    if(brushColor == changingColor) {
        return;
    }

   recursivePerform(x,y);
}

void Bucket::recursivePerform(int x, int y){

    QColor checking;
    getCurrentFrame()->setPixel(x, y, brushColor);

    //recursively color adjacent pixels of the same color to brushColor
    if(x<getCurrentFrame()->getSizeX()-1 && x>-1){
        //check pixel to the right
        checking = getCurrentFrame()->getPixel(x+1, y);
        if(checking == changingColor) {
            recursivePerform(x+1, y);
        }
    }

    if(x>0){
        //left
        checking = getCurrentFrame()->getPixel(x-1, y);
        if(checking == changingColor) {
            recursivePerform(x-1, y);
        }
    }

    if(y<getCurrentFrame()->getSizeY()-1) {
        //up
        checking = getCurrentFrame()->getPixel(x, y+1);
        if(checking == changingColor){
            recursivePerform(x, y+1);
        }
    }

    if(y>0){
        //down
        checking = getCurrentFrame()->getPixel(x, y-1);
        if(checking == changingColor) {
            recursivePerform(x, y-1);
        }
    }

}
