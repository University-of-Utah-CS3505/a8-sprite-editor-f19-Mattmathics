#include "canvas.h"

Canvas::Canvas(int sizeX, int sizeY, QVector<Frame> *frames)
{
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->index = 0;

    if (frames == nullptr)
        this->frames = QVector<Frame>();
    else
        this->frames = QVector<Frame>(*frames);

    // If frame vector is empty, add new frame to avoid error.
    if (this->frames.size() == 0)
    {
        this->frames.push_back(Frame(sizeX, sizeY));
    }
}

void Canvas::addFrame()
{
    addFrame(Frame(sizeX, sizeY));
}

void Canvas::addFrame(Frame frame)
{
    frames.push_back(frame);
}

int Canvas::sizeFrame()
{
    return frames.size();
}

int Canvas::currentIndex()
{
    return index;
}

const Frame* Canvas::moveFrame(int index)
{
    this->index = index;
    return getFrame(index);
}

const Frame* Canvas::getFrame(int index)
{
    return &frames.at(index);
}

int Canvas::getWidth()
{
    return sizeX;
}

int Canvas::getHeight()
{
    return sizeY;
}

Canvas Canvas::fromJson(QString jsonString)
{
    // TODO
}

QString Canvas::toJson()
{
    // TODO
}



