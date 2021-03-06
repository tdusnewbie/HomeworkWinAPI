// HW12.1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "HW12.1.h"
#include "string.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

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
    LoadStringW(hInstance, IDC_HW121, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HW121));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HW121));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_HW121);
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


void doPasteAnSiText(HWND hWnd)
{
	char *szText;
	if (OpenClipboard(hWnd))
	{
		HANDLE hData = GetClipboardData(CF_TEXT);
		if (hData)
		{
			char *pData = (char*)GlobalLock(hData);
			szText = new char[strlen(pData) + 1];
			szText = pData;
			GlobalUnlock(hData);
			RECT rec;
			GetClientRect(hWnd, &rec);
			TCHAR *text;
			text = new TCHAR[strlen(szText) + 1];
			for (int i = 0; i < strlen(szText); i++)
				text[i] = (TCHAR)szText[i];
			text[strlen(szText)] = '\0';
			HDC hdc = GetDC(hWnd);
			DrawText(hdc, text, lstrlen(text), &rec, DT_BOTTOM);
			ReleaseDC(hWnd, hdc);

			CloseClipboard();
		}
		else
		{
			MessageBox(hWnd, L"Nothing in Clipboard", L"Notice", MB_OK);
		}
		CloseClipboard();
	}
}

void doPasteUnicodeText(HWND hWnd)
{
	WCHAR *szText;
	if (OpenClipboard(hWnd))
	{
		HANDLE hData = GetClipboardData(CF_UNICODETEXT);
		if (hData)
		{
			WCHAR* pData = (WCHAR*)GlobalLock(hData);
			szText = new WCHAR[lstrlen(pData) + 1];
			lstrcpy(szText, pData);
			GlobalUnlock(hData);
			RECT rec;
			GetClientRect(hWnd, &rec);
			TCHAR *text;
			text = new TCHAR[lstrlen(szText) + 1];
			for (int i = 0; i < lstrlen(szText); i++)
				text[i] = (TCHAR)szText[i];
			text[lstrlen(szText)] = '\0';
			HDC hdc = GetDC(hWnd);
			DrawText(hdc, text, lstrlen(text), &rec, DT_BOTTOM);
			ReleaseDC(hWnd, hdc);
		}
		else
			MessageBox(hWnd, L"Nothing in CLipboard", L"Notice", MB_OK);
		CloseClipboard();
	}
}

void doPasteBitmap(HWND hWnd)
{
	if (OpenClipboard(hWnd))
	{
		HBITMAP hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
		if (hBitmap)
		{
			HDC hdc, memDC;
			BITMAP bmp;
			hdc = GetDC(hWnd);
			memDC = CreateCompatibleDC(hdc);
			SelectObject(memDC, hBitmap);
			GetObject(hBitmap, sizeof(BITMAP), &bmp);
			BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, memDC, 0, 0, SRCCOPY);
		}
		else
			MessageBox(hWnd, L"Nothing in Clipboard", L"Notice", MB_OK);
		CloseClipboard();
	}
	//MessageBox(hWnd, L"You have clicked at Paste BitMap", L"Notice", MB_OK);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_PASTEANSITEXT:
				doPasteAnSiText(hWnd);
				break;
			case ID_PASTEUNICODETEXT:
				doPasteUnicodeText(hWnd);
				break;
			case ID_PASTEBITMAP:
				doPasteBitmap(hWnd);
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
            // TODO: Add any drawing code that uses hdc here...
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
