#include "frame.h"

Frame::Frame(int sizeX, int sizeY, QImage *copyPixels)
{
    this->sizeX = sizeX;
    this->sizeY = sizeY;

    // If copy obejct is null, make new image object.
    if (copyPixels == nullptr)
    {
        image = QImage(sizeX, sizeY, QImage::Format_ARGB32);

        //Set all pixels to inital color
        QColor initalColor(0, 0, 0, 0);

        for(int x = 0; x < sizeX; x++)
            for(int y = 0; y < sizeY; y++)
                setPixel(x, y, initalColor);
    }
    else
       image = copyPixels->copy();
}

Frame::~Frame()
{

}

QColor Frame::getPixel(int x, int y)
{
    return QColor::fromRgba(image.pixel(x,y));
}

void Frame::setPixel(int x, int y, QColor color)
{
    image.setPixel(x, y, color.rgba());
}

QImage Frame::getImage()
{
    return image;
}

std::string Frame::toJson()
{
    json j;
    std::vector<std::vector<int>> pixels;

    for(int x = 0; x < sizeX; x++)
        for(int y = 0; y < sizeY; y++)
        {
            std::vector<int> colors;

            QColor color = this->getPixel(x, y);
            colors.push_back(color.red());
            colors.push_back(color.green());
            colors.push_back(color.blue());
            colors.push_back(color.alpha());

            pixels.push_back(colors);
        }

    j["pixels"] = pixels;

    return j.dump();
}

Frame Frame::fromJson(QString jsonString)
{
    // TODO
}

int Frame::getSizeX()
{
	return sizeX;
}

int Frame::getSizeY()
{
	return sizeY;
}
