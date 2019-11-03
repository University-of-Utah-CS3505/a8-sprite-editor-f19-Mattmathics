#ifndef HISTORY_H
#define HISTORY_H

#include <QImage>

class History
{
public:
    History(QImage pixelMap, QString jobName);

    QImage getPixelMap();
    QString getJobName();

private:
    QImage pixelMap;
    QString jobName;
};

#endif // HISTORY_H
