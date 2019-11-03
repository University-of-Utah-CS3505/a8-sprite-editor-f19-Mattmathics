#include "paintAllSameColor.h"

PaintAllSameColor::PaintAllSameColor(QColor color, Frame* frame) : Tool(color, frame)
{

}

PaintAllSameColor::~PaintAllSameColor()
{

}

void PaintAllSameColor::perform(int x, int y)
{
	QColor paintAllSame = currentFrame->getPixel(x, y);
    int frameSizeX = currentFrame -> getSizeX();
    int frameSizeY = currentFrame -> getSizeY();
	for (int x = 0; x < frameSizeX; x++)
    {
		for (int y = 0; y < frameSizeY; y++)
		{
    		if (currentFrame->getPixel(x, y) == paintAllSame)
			{
				currentFrame->setPixel(x, y, currentBrushColor);
			}
		}
	}
}
