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

    if(x<getCurrentFrame()->getSizeX()-1 && x>-1){
        checking = getCurrentFrame()->getPixel(x+1, y);//right
        if(checking == changingColor) {
            recursivePerform(x+1, y);
        }
    }

    if(x>0){
        checking = getCurrentFrame()->getPixel(x-1, y);//left
        if(checking == changingColor) {
            recursivePerform(x-1, y);
        }
    }

    if(y<getCurrentFrame()->getSizeY()-1) {
        checking = getCurrentFrame()->getPixel(x, y+1);//up
        if(checking == changingColor){
            recursivePerform(x, y+1);
        }
    }

    if(y>0){
        checking = getCurrentFrame()->getPixel(x, y-1);//down
        if(checking == changingColor) {
            recursivePerform(x, y-1);
        }
    }

}
