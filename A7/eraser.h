#ifndef ERASER_H
#define ERASER_H

#include "tool.h"

class Eraser : public Tool
{
public:
    Eraser(Canvas* canvas);
    ~Eraser() override;

    void perform(int x, int y) override;
};

#endif // ERASER_H
