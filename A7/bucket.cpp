#include "bucket.h"
#include <QColor>

Bucket::Bucket(QColor color, Frame* frame) : Tool(color, frame)
{

}

Bucket::~Bucket()
{

}

void Bucket::perform(int x, int y)
{
    changingColor = currentFrame->getPixel(x,y);

    if(brushColor == changingColor) {
        return;
    }

   recursivePerform(x,y);
}
void Bucket::recursivePerform(int x, int y){

    QColor checking;
    currentFrame->setPixel(x, y, brushColor);

    if(x<currentFrame->getSizeX()-1 && x>-1){
        checking = currentFrame->getPixel(x+1, y);//right
        if(checking == changingColor) {
            recursivePerform(x+1, y);
        }
    }

    if(x>0){
        checking = currentFrame->getPixel(x-1, y);//left
        if(checking == changingColor) {
            recursivePerform(x-1, y);
        }
    }

    if(y<currentFrame->getSizeY()-1) {
        checking = currentFrame->getPixel(x, y+1);//up
        if(checking == changingColor){
            recursivePerform(x, y+1);
        }
    }

    if(y>0){
        checking = currentFrame->getPixel(x, y-1);//down
        if(checking == changingColor) {
            recursivePerform(x, y-1);
        }
    }

}
