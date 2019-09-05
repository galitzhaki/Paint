#include "stdafx.h"
#include "MySquare.h"


MySquare::MySquare()
{
}


MySquare::~MySquare()
{
}

void MySquare::innerDraw(CDC *dc) 
{
	int width = end.x - start.x;

	int height = end.y - start.y;
	int w = (width > height) ? height : width;
	if (w < 0)
		w = (width < height) ? height : width;
	dc->Rectangle(start.x, start.y, start.x + w, start.y + w);
}