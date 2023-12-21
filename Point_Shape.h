#pragma once
#include "Shape.h"

class PointShape : public Shape {
public:
	PointShape();
	void Show(HDC);
	void DrawRubberTrack(HWND);
	Shape* CreateNew();
};