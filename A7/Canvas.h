#ifndef CANVAS_H
#define CANVAS_H

#include <QObject>
#include <QVector>

#include "frame.h"

class Canvas
{
public:

    /**
     * Create Canvas Object
     *
     * @param sizeX x size of canvas
     * @param sizeY y size of canvas
     * @param frames all frames
     */
    Canvas(int sizeX, int sizeY, QVector<Frame> frames);

    /**
     * Create canvas object from JSON string.
     *
     * This method is static method.
     *
     * @param jsonString JSON string
     * @return Canvas object
     */
    static Canvas fromJson(QString jsonString);

    QString toJson();

    void update();

    void moveFrame();

    Frame getFrame(int index);

    void addFrame();

    void addFrame(Frame frame);

private:
    QVector<Frame> frames;
    int sizeX;
    int sizeY;
    int currentIndex;

};

#endif // CANVAS_H
