#include "framework.h"
#include "Resource.h"
#include "File.h"

#include <string>
using namespace std;

void File::OnSaveAs(HWND hWnd, LPCWSTR filePath)
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

    GetDIBits(hdcWindow, hbmImage, 0, (UINT)bmpImage.bmHeight, lpbitmap, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    hFile = CreateFile(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

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

void File::OnSave(HWND hWnd) {
    OnSaveAs(hWnd, L"output.bmp");
}

void resizeWindow(HWND hWnd, int height, int width, bool isNew) {
    RECT rc;
    GetWindowRect(hWnd, &rc);
    MoveWindow(hWnd, rc.left, rc.top, width, height + TOOLBAR_HEIGHT, isNew);
}

void File::OnOpen(HWND hWnd, LPCWSTR filePath)
{
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

void File::OnNew(HWND hWnd) {
    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_INPUT_FORM_NEW), hWnd, (DLGPROC)DlgProc);
}