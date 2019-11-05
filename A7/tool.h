#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QColor>

#include "canvas.h"
#include "frame.h"

/**
 * Tool abstract class
 *
 * @author Kyungyoon Kim
 */
class Tool
{
public:
    virtual ~Tool();

    /* METHOD */
    virtual void perform(int x, int y) = 0;

    /* GETTER, SETTER */
    void setCurrentCanvas(Canvas* canvas);
    void setBrushColor(QColor color);
    QColor getBrushColor();

protected:
    Tool(QColor color, Canvas* canvas);

    /* GETTER, SETTER */
    Frame* getCurrentFrame();

    /* VARS */
    Canvas* currentCanvas;
    QColor brushColor;
};

#endif // TOOL_H
