// HW4.1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "HW4.1.h"
#include <fstream>
#include <iostream>

using namespace std;

#define MAX_LOADSTRING 100
#define MAX_NAME 100


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Info(HWND, UINT, WPARAM, LPARAM);

struct InfoData
{
	WCHAR Ten[MAX_NAME];
	BOOLEAN Phai;
	WCHAR Bomon[MAX_NAME];
};

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
    LoadStringW(hInstance, IDC_HW41, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HW41));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HW41));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_HW41);
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
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case ID_INSERT_INFO:
				INT_PTR returnPTR;
				returnPTR = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Info);
				if (returnPTR != IDCLOSE)
				{
					InfoData *tmp = (InfoData*)returnPTR;
					wofstream file;
					file.open("info.txt");
					file << tmp->Ten << endl;
					file << tmp->Phai << endl;
					file << tmp->Bomon << endl;
					file.close();
				}
				delete (InfoData*)returnPTR;
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

INT_PTR CALLBACK Info(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND combo;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
		EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);
		SetFocus(GetDlgItem(hDlg, IDC_EDIT1));
		combo = GetDlgItem(hDlg, IDC_LIST1);
		SendMessage(combo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("Khoa Hoc May Tinh")));
		SendMessage(combo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("Cong Nghe Phan Mem")));
		SendMessage(combo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("He Thong Thong Tin")));
		SendMessage(combo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("Mang May Tinh")));
		SendMessage(combo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("Cong Nghe Tri Thuc")));
		SendMessage(combo, CB_SETCURSEL, 0, 0);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_EDIT1:
			if (HIWORD(wParam) == EN_CHANGE)
			{
				LRESULT nTextLen = SendMessage((HWND)lParam, WM_GETTEXTLENGTH, 0, 0L);
				EnableWindow(GetDlgItem(hDlg, IDOK), (nTextLen > 0) ? TRUE : FALSE);
			}
			break;
		case IDCLOSE:
			EndDialog(hDlg, IDCLOSE);
			break;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
		case IDOK:
			struct InfoData *info = new InfoData;
			GetDlgItemText(hDlg, IDC_EDIT1, info->Ten, MAX_NAME);
			info->Phai = IsDlgButtonChecked(hDlg, IDC_RADIO1);
			GetDlgItemText(hDlg, IDC_LIST1, info->Bomon, MAX_NAME);
			EndDialog(hDlg, IDOK);
			break;
		}
		return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}

