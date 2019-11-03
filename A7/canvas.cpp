#include "canvas.h"

#include <QDebug>

using json = nlohmann::json;

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

Canvas::Canvas(const Canvas &copyCanvas)
{
    this->sizeX = copyCanvas.sizeX;
    this->sizeY = copyCanvas.sizeY;
    this->index = copyCanvas.index;
    this->frames = QVector<Frame>(copyCanvas.frames);
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

Frame* Canvas::moveFrame(int index)
{
    this->index = index;
    return getFrame(index);
}

Frame* Canvas::getFrame(int index)
{
    return const_cast<Frame*>(&frames.at(index));
}

Frame* Canvas::getCurrentFrame()
{
    return getFrame(currentIndex());
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
    auto j = json::parse(jsonString.toStdString());

    int sizeX = j["width"].get<int>();
    int sizeY = j["height"].get<int>();
    int frameSize = j["frameSize"].get<int>();

    std::vector<std::string> framesVector = j["frames"].get<std::vector<std::string>>();
    QVector<Frame> frames;

    foreach (std::string json, framesVector) {
        Frame frame = Frame::fromJson(QString::fromStdString(json), sizeX, sizeY);
        frames.push_back(frame);
    }

    return Canvas(sizeX, sizeY, &frames);
}

std::string Canvas::toJson()
{
    json j;

    j["frameSize"] = frames.size();
    j["width"] = sizeX;
    j["height"] = sizeY;

    std::vector<std::string> framesVector;

    for(int index = 0; index < frames.size(); index++)
    {
        framesVector.push_back(frames[index].toJson());
    }

    j["frames"] = framesVector;
    return j.dump();
}



