#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QColor>

#include "canvas.h"
#include "frame.h"

/**
 * Tool abstract/interface class
 *
 *
 */
class Tool
{
public:
    virtual ~Tool();
    virtual void perform(int x, int y) = 0;

    void setCurrentCanvas(Canvas* canvas);
    void setBrushColor(QColor color);
    QColor getBrushColor();

protected:
    Tool(QColor color, Canvas* canvas);

    Frame* getCurrentFrame();
    Canvas* currentCanvas;

    QColor brushColor;
};

#endif // TOOL_H
