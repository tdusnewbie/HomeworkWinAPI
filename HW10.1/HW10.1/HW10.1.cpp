// HW10.1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "HW10.1.h"
#include <commdlg.h>
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

bool zoomed = false;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_HW101, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HW101));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HW101));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_HW101);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

void LoadRCBitmap(HBITMAP &hBitmap)
{
	hBitmap = LoadBitmap(hInst, (LPCWSTR)IDB_BITMAP1);
}

void OpenFileBitmap(HWND hWnd,HBITMAP &hBitmap)
{
	OPENFILENAME of;
	TCHAR szFile[256];
	TCHAR szFilter[] = L"Bitmap(*.bmp)\0*.bmp\0Icon(*.ico)\0*.ico";
	
	szFile[0] = '\0';
	ZeroMemory(&of, sizeof(OPENFILENAME));
	of.lStructSize = sizeof(OPENFILENAME);
	of.hwndOwner = hWnd;
	of.lpstrFilter = szFilter;
	of.nFilterIndex = 0;
	of.lpstrFile = szFile;
	of.nMaxFile = sizeof(szFile);
	of.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&of))
		hBitmap = (HBITMAP)LoadImage(hInst, of.lpstrFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void PaintBitmap(HWND hWnd, HDC hdc,HBITMAP hBitmap)
{
	if (hBitmap != NULL)
	{
		HDC memDC = CreateCompatibleDC(hdc);
		BITMAP bmp;
		SelectObject(memDC, hBitmap);
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		if (zoomed)
		{
			RECT r;
			GetClientRect(hWnd, &r);
			HBITMAP hZoomBitmap = CreateCompatibleBitmap(hdc, r.right - r.left, r.bottom - r.top);
			HDC memZoomDC = CreateCompatibleDC(hdc);
			SelectObject(memZoomDC, hZoomBitmap);
			StretchBlt(memZoomDC, 0, 0, r.right - r.left, r.bottom - r.top, memDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			BitBlt(hdc, 0, 0, r.right - r.left, r.bottom - r.top, memZoomDC, 0, 0, SRCCOPY);
			DeleteDC(memZoomDC);
			DeleteObject(hZoomBitmap);
			zoomed = false;
		}
		else 
			BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, memDC, 0, 0, SRCCOPY);
		DeleteDC(memDC);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		static HBITMAP hBitmap;
		static HMENU hMenu;
	case WM_CREATE:
	{
		hMenu = GetMenu(hWnd);
		EnableMenuItem(hMenu, 1, MF_DISABLED | MF_BYPOSITION);
	}
	break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_FILE_LOADRCBITMAP:
				LoadRCBitmap(hBitmap);
				InvalidateRect(hWnd, NULL, true);
				break;
			case ID_FILE_LOADFILEBITMAP:
				OpenFileBitmap(hWnd,hBitmap);
				InvalidateRect(hWnd, NULL, true);
				break;
			case ID_VIEW_FITTOWINDOW:
				zoomed = true;
				InvalidateRect(hWnd, NULL, true);
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			if (hBitmap != NULL)
			{
				hMenu = GetMenu(hWnd);
				EnableMenuItem(hMenu, 1, MF_ENABLED | MF_BYPOSITION);
			}
            // TODO: Add any drawing code that uses hdc here...
			PaintBitmap(hWnd, hdc, hBitmap);
            EndPaint(hWnd, &ps);
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

// Message handler for about box.
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
