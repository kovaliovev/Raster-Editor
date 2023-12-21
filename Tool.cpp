#include "framework.h"
#include "resource.h"
#include "Tool.h"

void Tool::OnLBdown(HWND hWnd) {
	isDrawing = true;
	GetCursorPos(&point);
	ScreenToClient(hWnd, &point);
	prev_x = point.x;
	prev_y = point.y;
};

void Tool::OnMouseMove(HWND hWnd) {
	if (!isDrawing) return;

	HDC hdc = GetDC(hWnd);
	HPEN hPen = CreatePen(PS_DASH, tool_thick, tool_color);
	HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);

	MoveToEx(hdc, prev_x, prev_y, NULL);

	GetCursorPos(&point);
	ScreenToClient(hWnd, &point);

	curr_x = point.x;
	curr_y = point.y;

	LineTo(hdc, curr_x, curr_y);

	prev_x = curr_x;
	prev_y = curr_y;

	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
	ReleaseDC(hWnd, hdc);
}

void Tool::OnLBup(HWND hWnd) {
	isDrawing = false;
}

void Tool::SetColor(COLORREF new_color) {
	tool_color = new_color;
}

void Tool::SetThick(int new_thick) {
	tool_thick = new_thick;
}