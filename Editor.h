#pragma once
#include "Shape.h"
#include "Toolbar.h"
#include "Pencil.h"

#define ARRAY_OF_SHAPES_SIZE 99999
#define TOOLBAR_HEIGHT 37

class Editor {
private:
	static Editor* p_instance;
	Editor() {}
	Editor(const Editor&);
	Editor& operator=(Editor&);
	static Shape* p_shapes[ARRAY_OF_SHAPES_SIZE];
	static int currIndex;
	Shape* currShape;
	Tool* currTool;
	COLORREF currColor = NULL;
	int currThick = 1;
	bool isDrawing = false;
	Toolbar toolbar;
	POINT point;
	void ClearShapes();
	int windowHeight;
	int windowWidth;
public:
	~Editor();
	static Editor* GetInstance();
	void Start(HWND, LPARAM, Shape*);
	void StartTool(HWND, LPARAM, Tool*);
	void OnLBdown(HWND);
	void OnLBup(HWND);
	void OnMouseMove(HWND);
	void OnPaint(HWND);
	void OnCreate(HWND, HINSTANCE);
	void OnSize(HWND);
	void OnNotify(HWND, WPARAM, LPARAM);
	void OnColorChoose(COLORREF);
	void OnThickChoose(HWND, int);
	void OnFileNew(HWND);
	void OnFileSaveAs(HWND, LPCWSTR);
	void OnFileSave(HWND);
	void OnFileOpen(HWND, LPCWSTR);
};