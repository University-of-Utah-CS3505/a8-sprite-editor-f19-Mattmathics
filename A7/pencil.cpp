#include "pencil.h"

Pencil::Pencil(QColor color, Frame* frame) : Tool(color, frame)
{

}

Pencil::~Pencil()
{
    resetStrokes();
}

void Pencil::perform(int x, int y)
{
    if(brushStrokes[x][y]) {
        return;
    }
    brushStrokes[x][y] = true;
    QColor backColor = currentFrame->getPixel(x,y);
    if(backColor == QColor(0,0,0,0)){
        currentFrame->setPixel(x, y, brushColor);
    }
    else {
        int brushAlpha = brushColor.alpha();
        double oneMinus = 1.0-((double)brushAlpha/255.0);
        int outA = (double)brushColor.alpha() + ((double)backColor.alpha()*oneMinus);
        if(outA>255) {
            outA = 255;
        }
        int outR = (brushColor.red()*brushColor.alpha()+backColor.red()*backColor.alpha()*oneMinus)/outA;
        if(outR>255) {
            outR = 255;
        }
        int outG = (brushColor.green()*brushColor.alpha()+backColor.green()*backColor.alpha()*oneMinus)/outA;
        if(outG>255) {
            outG = 255;
        }
        int outB = (brushColor.blue()*brushColor.alpha()+backColor.blue()*backColor.alpha()*oneMinus)/outA;
        if(outB>255) {
            outB = 255;
        }
        currentFrame->setPixel(x,y, QColor(outR, outG, outB, outA));
    }
}

void Pencil::resetStrokes(){
    for (int i = 0; i < currentFrame->getSizeX(); i++) {
        for (int j = 0; j < currentFrame->getSizeY(); j++) {
            brushStrokes[i][j] = false;
        }
    }
}
