#include "framework.h"
#include "resource.h"
#include "Point_Shape.h"

PointShape::PointShape() {}

void PointShape::Show(HDC hdc) {
	HPEN hPen = CreatePen(PS_SOLID, shape_thick, shape_color);
	HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);

	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(shape_color);
	HBRUSH hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);
	
	Ellipse(hdc, shape_x_2 - 3, shape_y_2 - 3, shape_x_2 + 3, shape_y_2 + 3);

	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
	SelectObject(hdc, hBrushOld);
	DeleteObject(hBrush);
}

void PointShape::DrawRubberTrack(HWND hWnd) {
	HDC hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOTXORPEN);

	HPEN hPen = CreatePen(PS_DASH, shape_thick, shape_color);
	HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);

	Ellipse(hdc, shape_x_2 - 3, shape_y_2 - 3, shape_x_2 + 3, shape_y_2 + 3);

	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
	ReleaseDC(hWnd, hdc);
};

Shape* PointShape::CreateNew() {
	return new PointShape;
};