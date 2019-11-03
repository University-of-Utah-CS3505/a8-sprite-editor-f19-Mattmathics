#include "tool.h"

Tool::Tool(QColor color, Frame* frame)
{
    if (frame == nullptr)
    {
        // TODO: throw error.
    }

    currentFrame = frame;
    brushColor = color;

}

Tool::~Tool()
{

}

void Tool::setCurrentFrame(Frame *frame)
{
    currentFrame = frame;
}

void Tool::setBrushColor(QColor color)
{
    brushColor = color;
}

QColor Tool::getBrushColor()
{
    return brushColor;
}
