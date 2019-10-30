#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>
#include <QFile>

#include "frame.h"
#include "canvas.h"

/**
 * Project Manager Static Class
 *
 * @author Kyungyoon Kim
 */
class ProjectManager
{
public:

    static bool saveAsPng(Frame* frame, QString uri);

    static bool saveAsGif(Canvas* canvas, QString uri);

    static bool saveProject(Canvas* canvas, QString uri);

private:
    ProjectManager() {}
};

#endif // PROJECTMANAGER_H
