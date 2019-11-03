#ifndef COLORPICKER_H
#define COLORPICKER_H

#include "tool.h"

class ColorPicker : public Tool
{
public:
    ColorPicker(QColor color, Canvas* canvas);
    ~ColorPicker() override;

    void perform(int x, int y) override;
};

#endif // COLORPICKER_H
