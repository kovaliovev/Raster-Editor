#pragma once
#include "Shape.h"

class RectangleShape : public virtual Shape {
public:
	RectangleShape();
	void Show(HDC);
	void DrawRubberTrack(HWND);
	Shape* CreateNew();
};