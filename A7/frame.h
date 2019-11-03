#ifndef FRAME_H
#define FRAME_H

#include <QObject>
#include <QImage>
#include <QColor>

#include "json.hpp"
#include "redoundomanager.h"

class Frame
{
public:
    Frame(int sizeX, int sizeY, QImage *copyPixels = nullptr);

    ~Frame();

    QColor getPixel(int x, int y);
    void setPixel(int x, int y, QColor color);

    std::string toJson();
    static Frame fromJson(QString jsonString, int sizeX, int sizeY);

    QImage getImage();

	int getSizeX();
	int	getSizeY();

    void captureHistory(QString jobName);

    void redo();
    void undo();

    RedoUndoManager redoUndoManager;

private:
    QImage image;
    int sizeX;
    int sizeY;
};

#endif // FRAME_H
