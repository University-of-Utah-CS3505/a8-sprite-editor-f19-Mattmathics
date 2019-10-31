#ifndef FRAME_H
#define FRAME_H

#include <QObject>
#include <QImage>
#include <QColor>

class Frame
{
public:
    Frame(int sizeX, int sizeY, QImage *copyPixels = nullptr);
    ~Frame();

    QColor getPixel(int x, int y);
    void setPixel(int x, int y, QColor color);

    QString toJson();
    static Frame fromJson(QString jsonString);

    QImage getImage();

	int getSizeX();
	int	getSizeY();

private:
    QImage image;
    int sizeX;
    int sizeY;
};

#endif // FRAME_H
