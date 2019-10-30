#ifndef CANVAS_H
#define CANVAS_H

#include <QObject>
#include <QVector>

#include "frame.h"

/**
 * The Canvas class
 *
 * @author Kyungyoon Kim
 */
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
    Canvas(int sizeX, int sizeY, QVector<Frame> *frames = nullptr);

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

    Frame* moveFrame(int index);
    Frame* getFrame(int index);
    Frame* getCurrentFrame();

    void addFrame();

    void addFrame(Frame frame);

    int sizeFrame();

    int currentIndex();

    int getWidth();
    int getHeight();

private:
    QVector<Frame> frames;
    int sizeX;
    int sizeY;
    int index;

};

#endif // CANVAS_H
