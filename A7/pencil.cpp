#include "pencil.h"

Pencil::Pencil(QColor color, Frame* frame) : Tool(color, frame)
{

}

Pencil::~Pencil()
{

}

void Pencil::perform(int x, int y)
{
    currentFrame->setPixel(x, y, brushColor);
}
