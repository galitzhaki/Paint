#pragma once
#include "MyRectangle.h"
class MySquare :public MyRectangle
{
public:
	MySquare();
	~MySquare();

	void innerDraw(CDC *dc);
};

