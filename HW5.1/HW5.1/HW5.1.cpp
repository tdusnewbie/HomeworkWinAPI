// HW5.1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "HW5.1.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hWndMDIClient;
HWND hWndMDIFrame;
int PrevDrawId;
int countText = 0;
int countBitmap = 0;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	EditWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	DrawWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	MDICloseProc(HWND, LPARAM);


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
    LoadStringW(hInstance, IDC_HW51, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HW51));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateMDISysAccel(hWndMDIClient, &msg) && !TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HW51));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_HW51);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	ATOM a = RegisterClassExW(&wcex);

	// Child Draw Window
	WNDCLASS editMDI;
	wcex.cbSize = sizeof(WNDCLASS);
	editMDI.style = CS_HREDRAW | CS_VREDRAW;
	editMDI.lpfnWndProc = EditWndProc;
	editMDI.cbClsExtra = 0;
	editMDI.cbWndExtra = 8;
	editMDI.hInstance = hInst;
	editMDI.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HW51));
	editMDI.hCursor = LoadCursor(nullptr, IDC_CROSS);
	editMDI.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	editMDI.lpszMenuName = NULL;
	editMDI.lpszClassName = L"MDI_EDIT_CHILD";

	RegisterClass(&editMDI);

	// Child Draw Window
	WNDCLASS drawMDI;
	wcex.cbSize = sizeof(WNDCLASS);
	drawMDI.style = CS_HREDRAW | CS_VREDRAW;
	drawMDI.lpfnWndProc = DrawWndProc;
	drawMDI.cbClsExtra = 0;
	drawMDI.cbWndExtra = 8;
	drawMDI.hInstance = hInst;
	drawMDI.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HW51));
	drawMDI.hCursor = LoadCursor(nullptr, IDC_CROSS);
	drawMDI.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	drawMDI.lpszMenuName = NULL;
	drawMDI.lpszClassName = L"MDI_DRAW_CHILD";

	RegisterClass(&drawMDI);
	return a;
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

void OnCreateFrameWindow(HWND hWnd)
{
	CLIENTCREATESTRUCT client;
	hWndMDIFrame = hWnd;
	client.hWindowMenu = GetSubMenu(GetMenu(hWnd), 4);
	client.idFirstChild = 50000;
	hWndMDIClient = CreateWindowW(L"MDICLIENT", (LPCTSTR)NULL,
		WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL, 0, 0, 0, 0,
		hWnd, (HMENU)NULL, hInst, (LPVOID)&client);
	ShowWindow(hWndMDIClient, SW_SHOW);
}

void OnNewBitmapMenu()
{
	WCHAR buf[100];
	wsprintf(buf, L"Bitmap %d", countBitmap);
	MDICREATESTRUCT drawMDI;
	drawMDI.cx = CW_USEDEFAULT;
	drawMDI.cy = CW_USEDEFAULT;
	drawMDI.x = CW_USEDEFAULT;
	drawMDI.y = CW_USEDEFAULT;
	drawMDI.hOwner = hInst;
	drawMDI.lParam = NULL;
	drawMDI.szClass = L"MDI_DRAW_CHILD";
	drawMDI.szTitle = buf;
	drawMDI.style = 0;
	SendMessage(hWndMDIClient, WM_MDICREATE, 0, (LONG)(LPMDICREATESTRUCT)&drawMDI);
	countBitmap++;
}
void OnNewTextMenu()
{
	WCHAR buf[100];
	wsprintf(buf, L"Text %d", countText);
	MDICREATESTRUCT drawMDI;
	drawMDI.cx = CW_USEDEFAULT;
	drawMDI.cy = CW_USEDEFAULT;
	drawMDI.x = CW_USEDEFAULT;
	drawMDI.y = CW_USEDEFAULT;
	drawMDI.hOwner = hInst;
	drawMDI.lParam = NULL;
	drawMDI.szClass = L"MDI_EDIT_CHILD";
	drawMDI.szTitle = buf;
	drawMDI.style = 0;
	SendMessage(hWndMDIClient, WM_MDICREATE, 0, (LONG)(LPMDICREATESTRUCT)&drawMDI);
	countText++;
}

void OnDrawMenu(HMENU hMenu,int wmId)
{
	int flag;
	if ((GetMenuState(hMenu, PrevDrawId, MF_BYCOMMAND) & MF_CHECKED) && PrevDrawId != wmId)
		CheckMenuItem(hMenu, PrevDrawId, MF_UNCHECKED | MF_BYCOMMAND);
	if (GetMenuState(hMenu, wmId, MF_BYCOMMAND) & MF_CHECKED)
		flag = MF_UNCHECKED;
	else
	{
		flag = MF_CHECKED;
		PrevDrawId = wmId;
	}
	CheckMenuItem(hMenu, wmId, flag | MF_BYCOMMAND);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		OnCreateFrameWindow(hWnd);
		break;
    case WM_COMMAND:
        {
			HMENU hMenu = GetMenu(hWnd);
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_FILE_NEWTEXT:
				OnNewTextMenu();
				break;
			case ID_FILE_NEWBITMAP:
				OnNewBitmapMenu();
				break;
			case ID_EDIT_COPY:
				MessageBox(hWnd, L"Bạn đã chọn Copy", L"Thông báo !!! ", MB_ICONINFORMATION | MB_OK);
				break;
			case ID_EDIT_CUT:
				MessageBox(hWnd, L"Bạn đã chọn Cut", L"Thông báo !!! ", MB_ICONINFORMATION | MB_OK);
				break;
			case ID_EDIT_PASTE:
				MessageBox(hWnd, L"Bạn đã chọn Paste", L"Thông báo !!! ", MB_ICONINFORMATION | MB_OK);
				break;
			case ID_DRAW_ELLIPSE:
			case ID_DRAW_LINE:
			case ID_DRAW_RECTANGLE:
			case ID_DRAW_PIXEL:
				OnDrawMenu(hMenu,wmId);
				break;
			case ID_WINDOW_TILEHORIZONTAL:
				SendMessage(hWndMDIClient, WM_MDITILE, MDITILE_HORIZONTAL, 0L);
				OnDrawMenu(hMenu, wmId);
				break;
			case ID_WINDOW_TILEVERTICAL:
				SendMessage(hWndMDIClient, WM_MDITILE, MDITILE_SKIPDISABLED, 0L);
				OnDrawMenu(hMenu, wmId);
				break;
			case ID_WINDOW_CASCADE:
				SendMessage(hWndMDIClient, WM_MDICASCADE, MDITILE_SKIPDISABLED, 0L);
				OnDrawMenu(hMenu, wmId);
				break;
			case ID_WINDOW_CLOSEALL:
				EnumChildWindows(hWndMDIClient, (WNDENUMPROC)MDICloseProc, 0L);
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
	case WM_SIZE:
		UINT w, h;
		w = LOWORD(lParam);
		h = HIWORD(lParam);
		MoveWindow(hWndMDIClient, 0, 0, w, h, TRUE);
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

LRESULT CALLBACK EditWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	/*case WM_MDIACTIVATE:
	{
		HMENU hMenu = GetMenu(hWndMDIFrame);
		EnableMenuItem(hMenu, 3, MF_DISABLED | MF_BYPOSITION);
		EnableMenuItem(hMenu, ID_FORMAT_CHOOSEFONT, MF_ENABLED | MF_BYCOMMAND);
	}*/
		//break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWndMDIClient, &ps);
		//TODO: Add any drawing code that uses hdc here
		EndPaint(hWndMDIClient, &ps);
	}
	break;
	case WM_DESTROY:
		countText--;
		//PostQuitMessage(0);
		break;
	default:
		return DefMDIChildProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK DrawWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	/*case WM_MDIACTIVATE:
	{
		HMENU hMenu = GetMenu(hWndMDIFrame);
		EnableMenuItem(hMenu, 1, MF_DISABLED | MF_BYPOSITION);
		EnableMenuItem(hMenu, 4, MF_ENABLED | MF_BYPOSITION);
		EnableMenuItem(hMenu, ID_FORMAT_CHOOSEFONT, MF_DISABLED | MF_BYCOMMAND);
	}*/
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWndMDIClient, &ps);
		//TODO: Add any drawing code that uses hdc here
		EndPaint(hWndMDIClient, &ps);
	}
	break;
	case WM_DESTROY:
		countBitmap--;
		//PostQuitMessage(0);
		break;
	default:
		return DefMDIChildProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK MDICloseProc(HWND hChildWnd, LPARAM lParam)
{
	SendMessage(hWndMDIClient, WM_MDIDESTROY, (WPARAM)hChildWnd, 0L);
	return 1;
}