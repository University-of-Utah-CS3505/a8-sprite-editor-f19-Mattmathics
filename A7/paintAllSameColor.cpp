#include "paintAllSameColor.h"

PaintAllSameColor::PaintAllSameColor(QColor color, Canvas* canvas) : Tool(color, canvas)
{

}

PaintAllSameColor::~PaintAllSameColor()
{

}

void PaintAllSameColor::perform(int x, int y)
{
    QColor paintAllSame = getCurrentFrame()->getPixel(x, y);
    int frameSizeX = getCurrentFrame() -> getSizeX();
    int frameSizeY = getCurrentFrame() -> getSizeY();

    //go through all the pixels on the frame to find ones of the same color
	for (int x = 0; x < frameSizeX; x++)
    {
		for (int y = 0; y < frameSizeY; y++)
		{
            if (getCurrentFrame()->getPixel(x, y) == paintAllSame)
			{
                getCurrentFrame()->setPixel(x, y, brushColor);
			}
		}
	}
}
