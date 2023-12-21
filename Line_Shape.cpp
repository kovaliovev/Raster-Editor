#include "framework.h"
#include "resource.h"
#include "Line_Shape.h"

LineShape::LineShape() {}

void LineShape::Show(HDC hdc) {
	HPEN hPenOld, hPen;

	COLORREF color = shape_color ? shape_color : COL_BLACK;

	hPen = CreatePen(PS_SOLID, shape_thick, color);
	hPenOld = (HPEN)SelectObject(hdc, hPen);

	MoveToEx(hdc, shape_x_1, shape_y_1, NULL);
	LineTo(hdc, shape_x_2, shape_y_2);

	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
}

void LineShape::DrawRubberTrack(HWND hWnd) {
	HDC hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOTXORPEN);

	HPEN hPen = CreatePen(PS_DASH, shape_thick, COL_BLACK);
	HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);
	LineShape::Show(hdc);

	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
	ReleaseDC(hWnd, hdc);
};

Shape* LineShape::CreateNew() {
	return new LineShape;
};