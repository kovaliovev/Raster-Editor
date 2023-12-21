#pragma once

class Tool {
private:
	POINT point;
	bool isDrawing = false;
	COLORREF tool_color;
	int tool_thick;
	int prev_x;
	int prev_y;
	int curr_x;
	int curr_y;
public:
	void OnLBdown(HWND);
	void OnLBup(HWND);
	void OnMouseMove(HWND);
	void virtual SetColor(COLORREF);
	void SetThick(int);
};