#include "framework.h"
#include "resource.h"
#include "Rectangle_Shape.h"

RectangleShape::RectangleShape() {}

void RectangleShape::Show(HDC hdc) {
	HPEN hPen = CreatePen(PS_SOLID, shape_thick, COL_BLACK);
	HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);

	HBRUSH hBrush = NULL;
	HBRUSH hBrushOld = NULL;
	if (shape_color)
	{
		hBrush = (HBRUSH)CreateSolidBrush(shape_color);
		hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);
	}

	Rectangle(hdc, shape_x_1, shape_y_1, shape_x_2, shape_y_2);

	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
	SelectObject(hdc, hBrushOld);
	DeleteObject(hBrush);
}

void RectangleShape::DrawRubberTrack(HWND hWnd) {
	HDC hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOTXORPEN);

	HPEN hPen = CreatePen(PS_DASH, shape_thick, COL_BLACK);
	HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);

	Rectangle(hdc, shape_x_1, shape_y_1, shape_x_2, shape_y_2);

	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
	ReleaseDC(hWnd, hdc);
};

Shape* RectangleShape::CreateNew() {
	return new RectangleShape;
};