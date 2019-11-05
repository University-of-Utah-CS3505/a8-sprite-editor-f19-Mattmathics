#include "colorpicker.h"

ColorPicker::ColorPicker(QColor color, Canvas* canvas) : Tool(color, canvas)
{

}

ColorPicker::~ColorPicker()
{

}

void ColorPicker::perform(int x, int y)
{
    //Prevent pick color that alpha level is 0.
    if(getCurrentFrame()->getPixel(x,y).alpha() != 0)
        brushColor = getCurrentFrame()->getPixel(x,y);
}
