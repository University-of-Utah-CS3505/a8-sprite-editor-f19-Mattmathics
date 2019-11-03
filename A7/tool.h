#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QColor>

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

    void setCurrentFrame(Frame* frame);
    void setBrushColor(QColor color);
    QColor getBrushColor();

protected:
    Tool(QColor color, Frame* frame);

    Frame* currentFrame;
    QColor brushColor;
};

#endif // TOOL_H
