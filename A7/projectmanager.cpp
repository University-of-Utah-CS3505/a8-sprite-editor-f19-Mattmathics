#include "projectmanager.h"

#include <QDebug>

bool ProjectManager::saveAsPng(Frame *frame, QString uri)
{
    QFile file(uri);
    return frame->getImage().save(file.fileName(), "PNG");
}

bool ProjectManager::saveAsGif(Canvas *canvas, QString uri)
{

}

bool ProjectManager::saveProject(Canvas *canvas, QString uri)
{
    QFile file(uri);
    QString jsonString = QString::fromStdString(canvas->toJson());

    qDebug() << "Save file to : " << uri << endl << jsonString << endl;

    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << jsonString << endl;

        return true;
    }

    return false;
}

Canvas ProjectManager::openProject(QString uri)
{
    QFile file(uri);

    qDebug() << "Load project from : " << uri << endl;

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        QString line = in.readLine();
        file.close();

         qDebug() << "JSOM :: " << line << endl;

        return Canvas::fromJson(line);
    }

    qDebug() << "Load Project Failed" << endl;
    throw false;
}
