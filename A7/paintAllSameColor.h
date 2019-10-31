#ifndef PAINTALLSAMECOLOR_H
#define PAINTALLSAMECOLOR_H

#include "tool.h"

class PaintAllSameColor : public Tool
{
public:
    PaintAllSameColor(QColor color, Frame* frame);
    ~PaintAllSameColor() override;

    void perform(int x, int y) override;
};

#endif // PAINTALLSAMECOLOR_H
