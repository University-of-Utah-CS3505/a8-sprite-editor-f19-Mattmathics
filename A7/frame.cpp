#include "frame.h"

Frame::Frame(int sizeX, int sizeY, QImage *copyPixels)
{
    this->sizeX = sizeX;
    this->sizeY = sizeY;

    // If copy obejct is null, make new image object.
    if (copyPixels == nullptr)
       image = QImage(sizeX, sizeY, QImage::Format_ARGB32);
    else
       image = copyPixels->copy();
}

Frame::~Frame()
{

}

QColor Frame::getPixel(int x, int y)
{
    return QColor(image.pixel(x, y));
}

void Frame::setPixel(int x, int y, QColor color)
{
    image.setPixel(x, y, color.rgba());
}

QImage Frame::getImage()
{
    return image;
}
