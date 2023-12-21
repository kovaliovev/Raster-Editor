#pragma once

class Shape {
protected:
	int shape_x_1;
	int shape_y_1;
	int shape_x_2;
	int shape_y_2;
	COLORREF shape_color = NULL;
	int shape_thick = 1;
public:
	virtual ~Shape();
	void SetStartCords(int, int);
	void SetEndCords(int, int);
	void SetColor(COLORREF);
	void SetThick(int);
	virtual void Show(HDC) = 0;
	virtual void DrawRubberTrack(HWND) = 0;
	virtual Shape* CreateNew() = 0;
};