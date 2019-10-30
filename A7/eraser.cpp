#include "eraser.h"

Eraser::Eraser(Frame* frame): Tool(QColor(0, 0, 0, 0), frame)
{

}

Eraser::~Eraser()
{

}

void Eraser::perform(int x, int y)
{
    qDebug() << "Eraser : " << brushColor;
    currentFrame->setPixel(x, y, brushColor);
}
