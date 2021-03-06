// P1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "P1.h"
#include <commctrl.h>
#include <commdlg.h>

#define MAX_LOADSTRING 100
#define ID_TOOLBAR		1000	// ID of the toolbar
#define IMAGE_WIDTH     20

#define IMAGE_HEIGHT    18
#define BUTTON_WIDTH    0
#define BUTTON_HEIGHT   0
#define TOOL_TIP_MAX_LEN 32

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR szChildWindowClass[MAX_LOADSTRING];
WCHAR szChildTile[MAX_LOADSTRING];
HWND hWndMDIClient;
HWND hToolBar;

int drawPrevId;
int count = 1;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	DrawWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	MDICloseProc(HWND, LPARAM);

// Toolbar Function
void doFileToolbar(HWND);
void doDrawToolbar(HWND);
void doWindowToolbar(HWND);
void doToolbarNotifyHandle(LPARAM);

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
    LoadStringW(hInstance, IDC_P1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_P1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateMDISysAccel(hWndMDIClient,&msg) && !TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_P1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_P1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	ATOM a = RegisterClassExW(&wcex);

	// Child Draw Window
	WNDCLASS drawMDI;
	wcex.cbSize = sizeof(WNDCLASS);
	drawMDI.style = CS_HREDRAW | CS_VREDRAW;
	drawMDI.lpfnWndProc = DrawWndProc;
	drawMDI.cbClsExtra = 0;
	drawMDI.cbWndExtra = 8;
	drawMDI.hInstance = hInst;
	drawMDI.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_P1));
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


void OnCreateMessageFrameWin(HWND hWnd)
{
	HMENU hMenu = GetMenu(hWnd);
	GetMenuState(hMenu, ID_DRAW_LINE, MF_BYCOMMAND);
	CheckMenuItem(hMenu, ID_DRAW_LINE, MF_CHECKED);
	drawPrevId = ID_DRAW_LINE;
	CLIENTCREATESTRUCT client;
	client.hWindowMenu = GetSubMenu(GetMenu(hWnd), 4);
	client.idFirstChild = 50000;
	hWndMDIClient = CreateWindowW(L"MDICLIENT", (LPCTSTR)NULL,
		WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL, 0, 0, 0, 0,
		hWnd, (HMENU)NULL, hInst, (LPVOID)&client);
	ShowWindow(hWndMDIClient, SW_SHOW);
	doFileToolbar(hWnd);
}

void OnFileNewMenu(HWND hWnd)
{
	WCHAR buf[100];
	wsprintf(buf, L"Noname %d.drw", count);
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
	count++;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		OnCreateMessageFrameWin(hWnd);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
			HMENU hMenu = GetMenu(hWnd);
			int flag;
            // Parse the menu selections:
			switch (wmId)
			{
			case ID_FILE_NEW:
				OnFileNewMenu(hWnd);
				break;
			case ID_FILE_OPEN:
			{
				/*MSG msg = { 0 };
				while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}*/
				MessageBox(hWnd, L"Bạn đã chọn Open", L"Thông báo", MB_OK);

			}
				break;
			case ID_FILE_SAVE:
				MessageBox(hWnd, L"Bạn đã chọn Save", L"Thông báo", MB_OK);
				break;
			case ID_DRAW_COLOR:
			{
				CHOOSECOLOR cc;
				COLORREF acrCtrl[16];
				DWORD rgbCurrent = RGB(255, 0, 0);
				ZeroMemory(&cc, sizeof(CHOOSECOLOR));
				cc.lStructSize = sizeof(CHOOSECOLOR);
				cc.hwndOwner = hWnd;
				cc.lpCustColors = (LPDWORD)acrCtrl;
				cc.rgbResult = rgbCurrent;
				cc.Flags = CC_FULLOPEN | CC_RGBINIT;
				if (ChooseColor(&cc))
				{
					HBRUSH hbrush;
					hbrush = CreateSolidBrush(cc.rgbResult);
				}
				/*else
					ErrorHandler();*/
			}
			break;
			case ID_DRAW_LINE:
			case ID_DRAW_RECTANGLE:
			case ID_DRAW_ELLIPSE:
			case ID_DRAW_TEXT:
			case ID_DRAW_SELECTOBJECT:
			{
				int flag;
				if (GetMenuState(hMenu, drawPrevId, MF_BYCOMMAND) & MF_CHECKED)
					CheckMenuItem(hMenu, drawPrevId, MF_UNCHECKED | MF_BYCOMMAND);
				if (GetMenuState(hMenu, wmId, MF_BYCOMMAND) & MF_CHECKED)
					flag = MF_UNCHECKED;
				else
				{
					drawPrevId = wmId;
					flag = MF_CHECKED;
				}
				CheckMenuItem(hMenu, wmId, flag | MF_BYCOMMAND);
			}
			break;
			case ID_WINDOW_TIDE:
				SendMessage(hWndMDIClient, WM_MDITILE, MDITILE_SKIPDISABLED, 0L);
				break;
			case ID_WINDOW_CASCADE:
				SendMessage(hWndMDIClient, WM_MDICASCADE, MDITILE_SKIPDISABLED, 0L);
				break;
			case ID_WINDOW_CLOSEALL:
				EnumChildWindows(hWndMDIClient, (WNDENUMPROC)MDICloseProc, 0L);
				break;
			case ID_TOOLBAR_FILE:
				doFileToolbar(hWnd);
				break;
			case ID_TOOLBAR_DRAW:
				doDrawToolbar(hWnd);
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
		h = HIWORD(lParam) ;
		MoveWindow(hWndMDIClient, 0, TOOL_TIP_MAX_LEN , w, h, TRUE);
		break;
	/*case WM_NOTIFY:
		doToolbarNotifyHandle(lParam);
		break;*/
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
        return DefFrameProc(hWnd, hWndMDIClient,message, wParam, lParam);
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

LRESULT CALLBACK DrawWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWndMDIClient, &ps);
		//TODO: Add any drawing code that uses hdc here
		EndPaint(hWndMDIClient, &ps);
	}
	break;
	case WM_DESTROY:
		count--;
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

void doFileToolbar(HWND hWnd)
{
	InitCommonControls();
	TBBUTTON tbButton[] = 
	{
		{STD_FILENEW, ID_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON,0,0},
		{STD_FILEOPEN,ID_FILE_OPEN,TBSTATE_ENABLED, TBSTYLE_BUTTON,0,0},
		{STD_FILESAVE,ID_FILE_SAVE,TBSTATE_ENABLED, TBSTYLE_BUTTON,0,0}
	};
	hToolBar = CreateToolbarEx(hWnd,
		WS_CHILD | WS_VISIBLE | CCS_ADJUSTABLE | TBSTYLE_TOOLTIPS,
		ID_TOOLBAR,
		sizeof(tbButton) / sizeof(TBBUTTON),
		HINST_COMMCTRL,
		0,
		tbButton,
		sizeof(tbButton) / sizeof(TBBUTTON),
		BUTTON_WIDTH,
		BUTTON_HEIGHT,
		IMAGE_WIDTH, 
		IMAGE_HEIGHT,
		sizeof(TBBUTTON));
	int flag;
	if (GetMenuState(GetMenu(hWnd), ID_TOOLBAR_FILE, MF_BYCOMMAND) & MF_CHECKED)
		flag = MF_UNCHECKED;
	else
		flag = MF_CHECKED;
	CheckMenuItem(GetMenu(hWnd), ID_TOOLBAR_FILE, flag | MF_BYCOMMAND);
}

void doDrawToolbar(HWND hWnd)
{
	TBBUTTON tbButton[] =
	{
		{0,0,TBSTATE_ENABLED,TBSTYLE_SEP,0,0},
		{0, ID_DRAW_LINE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{1, ID_DRAW_RECTANGLE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{2,ID_DRAW_ELLIPSE,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
		{3,ID_DRAW_TEXT,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0}
	};

	TBADDBITMAP tbBitmap = { hInst, IDB_BITMAP3 };
	int index = SendMessage(hToolBar, TB_ADDBITMAP, (WPARAM)sizeof(tbBitmap) / sizeof(TBADDBITMAP),
		(LPARAM)(LPTBADDBITMAP)&tbBitmap);
	tbButton[1].iBitmap += index;
	tbButton[2].iBitmap += index;
	tbButton[3].iBitmap += index;
	tbButton[4].iBitmap += index;

	SendMessage(hToolBar, TB_ADDBUTTONS, (WPARAM)sizeof(tbButton) / sizeof(TBBUTTON),
		(LPARAM)(LPTBBUTTON)&tbButton);
	int flag;
	if (GetMenuState(GetMenu(hWnd), ID_TOOLBAR_DRAW, MF_BYCOMMAND) & MF_CHECKED)
		flag = MF_UNCHECKED;
	else
		flag = MF_CHECKED;
	CheckMenuItem(GetMenu(hWnd), ID_TOOLBAR_DRAW, flag | MF_BYCOMMAND);
}

void doToolbarNotifyHandle(LPARAM	lParam)
{
	LPTOOLTIPTEXT   lpToolTipText;
	TCHAR			szToolTipText[TOOL_TIP_MAX_LEN]; 	// ToolTipText, loaded from Stringtable resource

														// lParam: address of TOOLTIPTEXT struct
	lpToolTipText = (LPTOOLTIPTEXT)lParam;

	if (lpToolTipText->hdr.code == TTN_NEEDTEXT)
	{
		// hdr.iFrom: ID cua ToolBar button -> ID cua ToolTipText string
		LoadString(hInst, lpToolTipText->hdr.idFrom, szToolTipText, TOOL_TIP_MAX_LEN);

		lpToolTipText->lpszText = szToolTipText;
	}
}