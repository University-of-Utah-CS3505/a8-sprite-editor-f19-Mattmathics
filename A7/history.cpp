#include "history.h"

History::History(QImage pixelMap, QString jobName): pixelMap(pixelMap), jobName(jobName)
{

}

QImage History::getPixelMap()
{
    return pixelMap;
}

QString History::getJobName()
{
    return jobName;
}
