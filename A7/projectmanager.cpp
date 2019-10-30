#include "projectmanager.h"

bool ProjectManager::saveAsPng(Frame *frame, QString uri)
{
    QFile file(uri);
    return frame->getImage().save(file.fileName(), "PNG");
}
