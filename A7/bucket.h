#ifndef BUCKET_H
#define BUCKET_H

#include <QDebug>

#include "tool.h"

class Bucket : public Tool
{
public:
    Bucket(QColor color, Frame* frame);
    ~Bucket() override;

    void perform(int x, int y) override;
private:
    void recursivePerform(int x, int y);
    QColor changingColor;
};

#endif // BUCKET_H

