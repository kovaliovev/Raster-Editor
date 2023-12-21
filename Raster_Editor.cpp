#include "framework.h"
#include "commdlg.h"
#include "Raster_Editor.h"
#include "Editor.h"
#include "Point_Shape.h"
#include "Line_Shape.h"
#include "Rectangle_Shape.h"
#include "Ellipse_Shape.h"
#include "Pencil.h"
#include "Rubber.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
COLORREF customColors[16] = { 0 };

Editor* editor = Editor::GetInstance();

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RASTEREDITOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RASTEREDITOR));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_RASTEREDITOR));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_RASTEREDITOR);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LPWSTR GetFileName(WCHAR* szFileName, bool isSaving)
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = (LPCWSTR)L"BMP Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = (LPWSTR)szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = (LPCWSTR)L"bmp";

    if (isSaving) GetSaveFileName(&ofn);
    else GetOpenFileName(&ofn);

    return szFileName;
}

void CallColorSelection(HWND hWnd, COLORREF& color, COLORREF customColors[]) {
    CHOOSECOLOR cc;

    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hWnd;
    cc.lpCustColors = customColors;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    cc.rgbResult = color;

    if (ChooseColor(&cc)) {
        color = cc.rgbResult;
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{   
    switch (message)
    {
    case WM_CREATE:
        editor->OnCreate(hWnd, hInst);
        break;
    case WM_SIZE:
        editor->OnSize(hWnd);
        break;
    case WM_NOTIFY:
        editor->OnNotify(hWnd, wParam, lParam);
        break;
    case WM_LBUTTONDOWN:
        editor->OnLBdown(hWnd);
        break;
    case WM_LBUTTONUP:
        editor->OnLBup(hWnd);
        break;
    case WM_MOUSEMOVE:
        editor->OnMouseMove(hWnd);
        break;
    case WM_PAINT:
        editor->OnPaint(hWnd);
        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case ID_TOOL_PENCIL:
            editor->StartTool(hWnd, ID_TOOL_PENCIL, new Pencil);
            break;
        case ID_TOOL_RUBBER:
            editor->StartTool(hWnd, ID_TOOL_RUBBER, new Rubber);
            break;
        case ID_TOOL_POINT:
            editor->Start(hWnd, ID_TOOL_POINT, new PointShape);
            break;
        case ID_TOOL_LINE:
            editor->Start(hWnd, ID_TOOL_LINE, new LineShape);
            break;
        case ID_TOOL_RECTANGLE:
            editor->Start(hWnd, ID_TOOL_RECTANGLE, new RectangleShape);
            break;
        case ID_TOOL_ELLIPSE:
            editor->Start(hWnd, ID_TOOL_ELLIPSE, new EllipseShape);
            break;
        case ID_TOOL_COLOR_CHOOSE:
        {
            COLORREF selectedColor;
            CallColorSelection(hWnd, selectedColor, customColors);
            editor->OnColorChoose(selectedColor);
            break;
        }
        case ID_TOOL_THICK_REDUCE:
        case ID_TOOL_THICK_INCREASE:
            editor->OnThickChoose(hWnd, wmId);
            break;
        case ID_FILE_NEW:
            editor->OnFileNew(hWnd);
            break;
        case ID_FILE_SAVE:
            editor->OnFileSave(hWnd);
            break;
        case ID_FILE_SAVE_AS:
        {
            WCHAR szFileName[MAX_PATH] = {};
            LPWSTR path = GetFileName(szFileName, TRUE);
            if (path) editor->OnFileSaveAs(hWnd, path);
            break;
        }
        case ID_FILE_OPEN:
        {
            WCHAR szFileName[MAX_PATH] = {};
            LPWSTR path = GetFileName(szFileName, FALSE);
            if (path) editor->OnFileOpen(hWnd, path);
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}