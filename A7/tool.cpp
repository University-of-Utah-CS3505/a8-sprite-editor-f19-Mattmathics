#include "tool.h"

Tool::Tool(QColor color, Canvas *canvas)
{
    if (canvas == nullptr)
    {
        // TODO: throw error.
    }

    currentCanvas = canvas;
    brushColor = color;

}

Tool::~Tool()
{

}

void Tool::setCurrentCanvas(Canvas *canvas)
{
    currentCanvas = canvas;
}

void Tool::setBrushColor(QColor color)
{
    brushColor = color;
}

Frame* Tool::getCurrentFrame()
{
    return currentCanvas->getCurrentFrame();
}

QColor Tool::getBrushColor()
{
    return brushColor;
}
