#include "frame.h"

#include <QDebug>

using namespace std;
using json = nlohmann::json;

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

void Frame::captureHistory(QString jobName)
{
    History history(image.copy(), jobName);
    redoUndoManager.addHistory(history);
}

void Frame::redo()
{
    QImage temp = redoUndoManager.redo();
    image.swap(temp);
}

void Frame::undo()
{
    QImage temp = redoUndoManager.undo();
    image.swap(temp);
}

std::string Frame::toJson()
{
    json j;
    vector<vector<vector<int>>> pixels;

    for(int x = 0; x < sizeX; x++)
    {
        vector<vector<int>> temp;

        for(int y = 0; y < sizeY; y++)
        {
            vector<int> colors;

            QColor color = this->getPixel(x, y);
            colors.push_back(color.red());
            colors.push_back(color.green());
            colors.push_back(color.blue());
            colors.push_back(color.alpha());

            temp.push_back(colors);
        }

        pixels.push_back(temp);
    }

    j["pixels"] = pixels;

    return j.dump();
}

Frame Frame::fromJson(QString jsonString, int sizeX, int sizeY)
{
    auto j = json::parse(jsonString.toStdString());

    vector<vector<vector<int>>> pixels = j["pixels"].get<vector<vector<vector<int>>> >();

    qDebug() << "Load pixels..." << endl;

    Frame frame(sizeX, sizeY);
    for (int x = 0; x < sizeX; x++)
    {
        for(int y = 0; y < sizeY; y++)
        {
            vector<int> rgba = pixels.at(x).at(y);
            QColor color(rgba[0], rgba[1], rgba[2], rgba[3]);

            frame.setPixel(x, y, color);
        }
    }

    return frame;
}

int Frame::getSizeX()
{
	return sizeX;
}

int Frame::getSizeY()
{
	return sizeY;
}
