#pragma once
#pragma comment(lib, "comctl32.lib")

class Toolbar {
protected:
	HWND hwndToolbar = NULL;
	LPARAM currentlParam = NULL;
public:
	Toolbar();
	void OnCreate(HWND, HINSTANCE);
	void OnSize(HWND);
	void OnToolChoose(HWND, LPARAM);
	void OnNotify(HWND, WPARAM, LPARAM);
};