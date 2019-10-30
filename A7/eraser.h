#ifndef ERASER_H
#define ERASER_H

#include <QDebug>

#include "tool.h"

class Eraser : public Tool
{
public:
    Eraser(Frame* frame);
    ~Eraser() override;

    void perform(int x, int y) override;
};

#endif // ERASER_H
