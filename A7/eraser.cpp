#include "eraser.h"

Eraser::Eraser(Canvas* canvas): Tool(QColor(0, 0, 0, 0), canvas)
{

}

Eraser::~Eraser()
{

}

void Eraser::perform(int x, int y)
{
    getCurrentFrame()->setPixel(x, y, brushColor);
}
