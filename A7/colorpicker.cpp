#include "colorpicker.h"

ColorPicker::ColorPicker(QColor color, Frame* frame) : Tool(color, frame)
{

}

ColorPicker::~ColorPicker()
{

}

void ColorPicker::perform(int x, int y)
{
    brushColor = currentFrame->getPixel(x,y);
}
