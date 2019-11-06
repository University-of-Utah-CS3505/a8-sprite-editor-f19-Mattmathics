#ifndef BUCKET_H
#define BUCKET_H

#include <QDebug>

#include "tool.h"

class Bucket : public Tool
{
public:
    Bucket(QColor color, Canvas* canvas);
    ~Bucket() override;

    void perform(int x, int y) override;
    bool compareColor(QColor *left, QColor *right);

private:
    void recursivePerform(int x, int y);
    QColor changingColor;
};

#endif // BUCKET_H

