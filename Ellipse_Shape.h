#pragma once
#include "Shape.h"

class EllipseShape : public virtual Shape {
public:
	EllipseShape();
	void Show(HDC);
	void DrawRubberTrack(HWND);
	Shape* CreateNew();
};