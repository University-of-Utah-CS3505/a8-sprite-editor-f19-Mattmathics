#ifndef PENCIL_H
#define PENCIL_H

#include "tool.h"

class Pencil : public Tool
{
public:
    Pencil(QColor color, Frame* frame);
    ~Pencil() override;

    void perform(int x, int y) override;
    void resetStrokes();

private:
    bool **brushStrokes;
};

#endif // PENCIL_H
