#pragma once
#include "Shape.h"

class LineShape : public virtual Shape {
public:
	LineShape();
	void Show(HDC);
	void DrawRubberTrack(HWND);
	Shape* CreateNew();
};