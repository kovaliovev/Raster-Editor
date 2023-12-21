#include "framework.h"
#include "resource.h"
#include "Editor.h"
#include <fstream>

Shape* Editor::p_shapes[ARRAY_OF_SHAPES_SIZE];
int Editor::currIndex = 0;
Editor* Editor::p_instance = NULL;

void Editor::ClearShapes() {
    for (int i = 0; i < ARRAY_OF_SHAPES_SIZE; i++)
    {
        if (!p_shapes[i]) break;
        p_shapes[i] = NULL;
    }
    currIndex = 0;
}

Editor* Editor::GetInstance()
{
	if (!p_instance)
		p_instance = new Editor();
	return p_instance;
}

Editor::~Editor()
{
    for (int i = 0; i < ARRAY_OF_SHAPES_SIZE; i++)
    {
        delete p_shapes[i];
    }
    delete[] p_shapes;
}

void Editor::Start(HWND hWnd, LPARAM lParam, Shape* shape) {
	currShape = shape;
    currTool = NULL;
	toolbar.OnToolChoose(hWnd, lParam);
}

void Editor::StartTool(HWND hWnd, LPARAM lParam, Tool* tool) {
    currTool = tool;
    currShape = NULL;
    currTool->SetColor(currColor);
    currTool->SetThick(currThick);
    toolbar.OnToolChoose(hWnd, lParam);
}

void Editor::OnLBdown(HWND hWnd) {
    if (currTool) currTool->OnLBdown(hWnd);

	if (!currShape) return;
	isDrawing = true;
	GetCursorPos(&point);
	ScreenToClient(hWnd, &point);
	p_shapes[currIndex] = currShape->CreateNew();
	p_shapes[currIndex]->SetStartCords(point.x, point.y);
	p_shapes[currIndex]->SetEndCords(point.x, point.y);

    p_shapes[currIndex]->SetColor(currColor);
    p_shapes[currIndex]->SetThick(currThick);
}

void Editor::OnLBup(HWND hWnd) {
    if (currTool) currTool->OnLBup(hWnd);

	if (!(isDrawing && currShape)) return;

	isDrawing = false;
	GetCursorPos(&point);
	ScreenToClient(hWnd, &point);
	p_shapes[currIndex]->SetEndCords(point.x, point.y);

	currIndex++;
	InvalidateRect(hWnd, NULL, FALSE);
}

void Editor::OnMouseMove(HWND hWnd) {
    if (currTool) currTool->OnMouseMove(hWnd);

	if (!(isDrawing && currShape)) return;

	p_shapes[currIndex]->DrawRubberTrack(hWnd);

	GetCursorPos(&point);
	ScreenToClient(hWnd, &point);

	p_shapes[currIndex]->SetEndCords(point.x, point.y);
	p_shapes[currIndex]->DrawRubberTrack(hWnd);
}

void Editor::OnPaint(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
	for (int i = 0; i < ARRAY_OF_SHAPES_SIZE; i++)
	{
		if (p_shapes[i]) p_shapes[i]->Show(hdc);
	}
	EndPaint(hWnd, &ps);
}

void Editor::OnCreate(HWND hWnd, HINSTANCE hInst) {
	toolbar.OnCreate(hWnd, hInst);
}

void Editor::OnSize(HWND hWnd) {
	toolbar.OnSize(hWnd);
}

void Editor::OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	toolbar.OnNotify(hWnd, wParam, lParam);
}

void Editor::OnColorChoose(COLORREF new_color) {
    currColor = new_color;
    if(currTool) currTool->SetColor(new_color);
};

void Editor::OnThickChoose(HWND hWnd, int wmId) {
    if (wmId == ID_TOOL_THICK_REDUCE && currThick > 1) currThick -= 5;
    else if (wmId == ID_TOOL_THICK_INCREASE && currThick < 50) currThick += 5;
    
    if(currTool) currTool->SetThick(currThick);
}

void Editor::OnFileNew(HWND hWnd) {
	ClearShapes();
	file.OnNew(hWnd);
}

void Editor::OnFileSaveAs(HWND hWnd, LPCWSTR filePath) {
	file.OnSaveAs(hWnd, filePath);
};

void Editor::OnFileSave(HWND hWnd) {
	file.OnSave(hWnd);
};
void Editor::OnFileOpen(HWND hWnd, LPCWSTR filePath) {
	ClearShapes();
	file.OnOpen(hWnd, filePath);
};