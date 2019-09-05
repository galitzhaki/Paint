#include "StdAfx.h"
#include "MyShape.h"

IMPLEMENT_SERIAL(MyShape, CObject, 1)

MyShape::MyShape(void)
{
}


MyShape::~MyShape(void)
{
}

void MyShape::Serialize( CArchive& archive )								// Function for Save and Open
{
    CObject::Serialize( archive );

    if(archive.IsStoring())		
		archive << start.x << start.y << end.x << end.y << bgColor;			// Save Shapes
    else	
        archive >> start.x >> start.y >> end.x >> end.y >> bgColor;			// Open Shapes
}

void MyShape::draw(CDC *dc)
{
	CBrush brush(bgColor);
	CBrush *old = dc->SelectObject(&brush);
	innerDraw(dc);
	dc->SelectObject(old);
}

bool MyShape::inHit(CPoint point)
{
	// Whether mouse click this shape or not,
	// if clicked this shape, this return true
	// else return false

	int minx,maxx,miny,maxy;;
	if(start.x < end.x)	minx = start.x; else minx = end.x;
	if(start.x > end.x)	maxx = start.x; else maxx = end.x;
	if(start.y < end.y)	miny = start.y; else miny = end.y;
	if(start.y > end.y)	maxy = start.y; else maxy = end.y;

	if (point.x < minx || point.y < miny || point.x > maxx || point.y > maxy) return false;

	return true;
}

void MyShape::move(CPoint point)
{
	//When selected this shape, moving as Mouse Position
	start += point;
	end += point;
}