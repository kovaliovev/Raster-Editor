#include "framework.h"
#include "resource.h"
#include "Ellipse_Shape.h"

EllipseShape::EllipseShape() {}

void EllipseShape::Show(HDC hdc) {
	HPEN hPen = CreatePen(PS_SOLID, shape_thick, COL_BLACK);
	HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);

	HBRUSH hBrush = NULL;
	HBRUSH hBrushOld = NULL;
	if(shape_color)
	{
		hBrush = (HBRUSH)CreateSolidBrush(shape_color);
		hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);
	}

	Ellipse(hdc, shape_x_1, shape_y_1, shape_x_2, shape_y_2);

	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
	SelectObject(hdc, hBrushOld);
	DeleteObject(hBrush);
}

void EllipseShape::DrawRubberTrack(HWND hWnd) {
	HDC hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOTXORPEN);

	HPEN hPen = CreatePen(PS_DASH, shape_thick, COL_BLACK);
	HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);

	Ellipse(hdc, shape_x_1, shape_y_1, shape_x_2, shape_y_2);

	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
	ReleaseDC(hWnd, hdc);
};

Shape* EllipseShape::CreateNew() {
	return new EllipseShape;
};