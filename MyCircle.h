#pragma once
#include "myshape.h"

class MyCircle :public MyShape
{
public:
	DECLARE_SERIAL(MyCircle)
	MyCircle(void);
	~MyCircle(void);
	void innerDraw(CDC *dc);
	bool MyCircle::inHit(CPoint point);
};

