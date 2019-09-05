#pragma once
class MyShape : public CObject
{
public:
	DECLARE_SERIAL(MyShape)
	MyShape(void);
	virtual ~MyShape(void);

	//setters
	void setStart(CPoint p) { start = p; }				// Set StartMousePoint
	void setEnd(CPoint p) { end = p; }					// Set EndMousePoint	

	//getters
	CPoint getStart() const { return start; }			// Get StartMousePoint
	CPoint getEnd() const { return end; }				// Get EndMousePoint

	COLORREF getBg() const { return bgColor; }			// Get Color of Shape
	void setBg(COLORREF c) { bgColor = c; }				// Set Color of Shape
	
	void draw(CDC *dc);							       // Shape Drawing fundtion
	virtual void innerDraw(CDC *dc) {}
	void Serialize(CArchive& archive);					// Save & Open

	bool inHit(CPoint point);							// Check whether mouse click this shape, or not
	void move(CPoint point);							// Move Shape as Mouse pointer
protected:
	CPoint start, end;									// Position of Start and End point
	COLORREF bgColor;									// Color of Shape
};
