#include "colorpicker.h"

ColorPicker::ColorPicker(QColor color, Canvas* canvas) : Tool(color, canvas)
{

}

ColorPicker::~ColorPicker()
{

}

void ColorPicker::perform(int x, int y)
{
    brushColor = getCurrentFrame()->getPixel(x,y);
}
