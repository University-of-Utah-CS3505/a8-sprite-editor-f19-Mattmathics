#include "canvas.h"

Canvas::Canvas(int sizeX, int sizeY, QVector<Frame> frames)
{
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->frames = frames;
}
