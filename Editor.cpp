#include "framework.h"
#include "resource.h"
#include "Editor.h"
#include <fstream>
#include <wingdi.h>

#include <string>
using namespace std;

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

void Editor::OnFileSaveAs(HWND hWnd, LPCWSTR filePath)
{
    HDC hdcImage;
    HDC hdcWindow;
    HDC hdcMemDC = NULL;
    HBITMAP hbmImage = NULL;
    BITMAP bmpImage;
    DWORD dwBytesWritten = 0;
    DWORD dwSizeofDIB = 0;
    HANDLE hFile = NULL;
    char* lpbitmap = NULL;
    HANDLE hDIB = NULL;
    DWORD dwBmpSize = 0;

    hdcImage = GetDC(NULL);
    hdcWindow = GetDC(hWnd);

    hdcMemDC = CreateCompatibleDC(hdcWindow);
    if (!hdcMemDC) return;

    RECT rcClient;
    GetClientRect(hWnd, &rcClient);

    hbmImage = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
    if (!hbmImage) return;

    SelectObject(hdcMemDC, hbmImage);

    int imageWidth = rcClient.right - rcClient.left;
    int imageHeight = rcClient.bottom - rcClient.top;

    BitBlt(hdcMemDC, 0, 0, imageWidth, imageHeight, hdcWindow, 0, TOOLBAR_HEIGHT, SRCCOPY);

    GetObject(hbmImage, sizeof(BITMAP), &bmpImage);

    BITMAPFILEHEADER   bmfHeader;
    BITMAPINFOHEADER   bi;

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmpImage.bmWidth;
    bi.biHeight = bmpImage.bmHeight - TOOLBAR_HEIGHT; // to cut off toolbar
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    dwBmpSize = ((bmpImage.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpImage.bmHeight;

    hDIB = GlobalAlloc(GHND, dwBmpSize);
    lpbitmap = (char*)GlobalLock(hDIB);

    GetDIBits(hdcWindow, hbmImage, 0, (UINT)bmpImage.bmHeight, lpbitmap,  (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    hFile = CreateFile(filePath,  GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
    bmfHeader.bfSize = dwSizeofDIB;
    bmfHeader.bfType = 0x4D42; // BM.

    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

    GlobalUnlock(hDIB);
    GlobalFree(hDIB);

    CloseHandle(hFile);

    DeleteObject(hbmImage);
    DeleteObject(hdcMemDC);
    ReleaseDC(NULL, hdcImage);
    ReleaseDC(hWnd, hdcWindow);
 }
    
void Editor::OnFileSave(HWND hWnd) {
   OnFileSaveAs(hWnd, L"output.bmp");
}

void resizeWindow(HWND hWnd, int height, int width, bool isNew) {
    RECT rc;
    GetWindowRect(hWnd, &rc);
    MoveWindow(hWnd, rc.left, rc.top, width, height + TOOLBAR_HEIGHT, isNew);
}

void Editor::OnFileOpen(HWND hWnd, LPCWSTR filePath)
{
    ClearShapes();

    HBITMAP hbmImage = (HBITMAP)LoadImage(NULL, filePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!hbmImage) return;

    HDC hdcWindow = GetDC(hWnd);

    BITMAP bm;
    GetObject(hbmImage, sizeof(BITMAP), &bm);

    HDC hdcMemDC = CreateCompatibleDC(hdcWindow);
    SelectObject(hdcMemDC, hbmImage);

    resizeWindow(hWnd, bm.bmHeight + 60, bm.bmWidth + 20, FALSE);
    BitBlt(hdcWindow, 0, TOOLBAR_HEIGHT, bm.bmWidth, bm.bmHeight, hdcMemDC, 0, 0, SRCCOPY);

    ReleaseDC(hWnd, hdcWindow);
    DeleteDC(hdcMemDC);
    DeleteObject(hbmImage);
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK) {
			int height, width;
			try {
				WCHAR buf[32];
				GetWindowText(GetDlgItem(hDlg, IDC_INPUT_FORM_NEW_HEIGHT), buf, 32);
				height = stoi(buf);
				GetWindowText(GetDlgItem(hDlg, IDC_INPUT_FORM_NEW_WIDTH), buf, 32);
				width = stoi(buf);
			}
			catch (...) {
				MessageBox(hDlg, L"Enter numbers in fields!", L"Type error", MB_OK | MB_ICONERROR);
				break;
			}

			if (height <= 0 || width <= 0) {
				MessageBox(hDlg, L"Entered numbers are invalid!", L"Value error", MB_OK | MB_ICONERROR);
				break;
			}
            
            HWND hWnd = GetParent(hDlg);
            resizeWindow(hWnd, height, width, TRUE);

			EndDialog(hDlg, 1);
			return TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void Editor::OnFileNew(HWND hWnd) {
	DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_INPUT_FORM_NEW), hWnd, (DLGPROC)DlgProc);
    ClearShapes();
}