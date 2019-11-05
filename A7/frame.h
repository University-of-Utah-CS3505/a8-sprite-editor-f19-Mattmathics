#ifndef FRAME_H
#define FRAME_H

#include <QObject>
#include <QImage>
#include <QColor>
#include <QQueue>
#include <QVector>

#include "json.hpp"

class Frame
{
public:
    Frame(int sizeX, int sizeY, QImage *copyPixels = nullptr);

    /* METHOD */
    void captureHistory();
    void redo();
    void undo();
    bool isRedoable();
    bool isUndoable();

    std::string toJson();
    static Frame fromJson(QString jsonString, int sizeX, int sizeY);

    /* GETTER, SETTER */
    QColor getPixel(int x, int y);
    void setPixel(int x, int y, QColor color);

    QImage getImage();

	int getSizeX();
    int	getSizeY();

private:
    QImage image;
    int sizeX;
    int sizeY;

    int historyIndex = 0;
    QVector<QImage> histories;
};

#endif // FRAME_H
