// Project1.cpp : Defines the entry point for the application.
//
#pragma comment(lib,"ComCtl32.lib")
#include "stdafx.h"
#include "Project1.h"
#include "Object.h"
#include <commctrl.h>
#include <commdlg.h>
#include <fstream>
#include <string>

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
WCHAR inputText[256];
HWND hWndMDIClient,hFrame,hChild;
HWND hToolBar;
HWND editBox;
HDC hdc, memDC;
HBITMAP hBitmap;
HPEN hPen;
HFONT hFont;
RECT r;
COLORREF chooseColor;
LOGFONT chooseFont;
OBJECT *objectType;

int drawPrevId;
int _count = 1;
bool Moving = false;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	TextBox(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	DrawWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	MDICloseProc(HWND, LPARAM);

//Toolbar Function
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
    LoadStringW(hInstance, IDC_PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT1));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassExW(&wcex);

	wcex.cbSize = sizeof(WNDCLASSEXW);

	wcex.lpfnWndProc	= DrawWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra = 8;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"MDI_DRAW_CHILD";

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

void CreateMDIClientWindows(HWND hWnd)
{
	hFrame = hWnd;
	HMENU hMenu = GetMenu(hWnd);
	GetMenuState(hMenu, ID_DRAW_LINE, MF_BYCOMMAND);
	CheckMenuItem(hMenu, ID_DRAW_LINE, MF_CHECKED);
	drawPrevId = ID_DRAW_LINE;
	CLIENTCREATESTRUCT client;
	client.hWindowMenu = GetSubMenu(GetMenu(hWnd), 4);
	client.idFirstChild = 50000;
	hWndMDIClient = CreateWindowW(L"MDICLIENT", (LPCTSTR)NULL,
		WS_CHILD | WS_CLIPCHILDREN |WS_VSCROLL | WS_HSCROLL, 0, 0, 0, 0,
		hWnd, (HMENU)NULL, hInst, (LPVOID)&client);
	ShowWindow(hWndMDIClient, SW_SHOW);
	doFileToolbar(hWnd);
}

void CreateChildWindow(HWND hWnd)
{
	MDICREATESTRUCT drawMDI;
	ZeroMemory(&drawMDI, sizeof(MDICREATESTRUCT));
	WCHAR buf[100];
	wsprintf(buf, L"Noname %d.drw", _count);
	drawMDI.cx = CW_USEDEFAULT;
	drawMDI.cy = CW_USEDEFAULT;
	drawMDI.x = CW_USEDEFAULT;
	drawMDI.y = CW_USEDEFAULT;
	drawMDI.hOwner = hInst;
	drawMDI.lParam = NULL;
	drawMDI.szClass = L"MDI_DRAW_CHILD";
	drawMDI.szTitle = buf;
	drawMDI.style = 0;
	SendMessage(hWndMDIClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&drawMDI);
	_count++;
}

void OnDrawColorMenu(HWND hWnd)
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
		chooseColor = cc.rgbResult;
}

void OnDrawFontMenu(HWND hWnd)
{
	CHOOSEFONT cf;
	LOGFONT lf;
	ZeroMemory(&cf, sizeof(CHOOSEFONT));
	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.hwndOwner = hWnd;
	cf.lpLogFont = &lf;
	cf.Flags = CF_SCREENFONTS | CF_EFFECTS;
	if (ChooseFont(&cf))
		chooseFont = lf;
}

void OnFileOpenMenu(HWND hWnd)
{
	OPENFILENAME of;
	TCHAR szFile[256];
	TCHAR szName[MAX_LOADSTRING];
	TCHAR szFilter[] = TEXT("Draw file\0*.drw\0Text file\0*.txt\0");

	szFile[0] = '\0';
	szName[0] = '\0';

	ZeroMemory(&of, sizeof(OPENFILENAME));
	of.lStructSize = sizeof(OPENFILENAME);
	of.hwndOwner = hWnd;
	of.lpstrFilter = szFilter;
	of.nFilterIndex = 0;
	of.lpstrFile = szFile;
	of.lpstrFileTitle = szName;
	of.nMaxFileTitle = sizeof(szName);
	of.nMaxFile = sizeof(szFile);
	of.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&of))
	{
		CreateChildWindow(hWnd);
		CHILD_WND_DATA *wndData = (CHILD_WND_DATA*)GetWindowLongPtr(hChild, 0);
		ifstream readFile;
		readFile.open(of.lpstrFile, ios::binary | ios::in);
		int size,type,lenString;
		bool saved;
		POINT p1, p2;
		COLORREF color;
		LOGFONT font;
		WCHAR *input;
		readFile.read((char*)&size, sizeof(int));
		readFile.read((char*)&saved, sizeof(bool));
		for (int i = 0; i < size; i++)
		{
			readFile.read((char*)&type, sizeof(int));
			switch (type)
			{
				case 0:
				{
					readFile.read((char*)&p1, sizeof(POINT));
					readFile.read((char*)&p2, sizeof(POINT));
					readFile.read((char*)&color, sizeof(COLORREF));
					OBJECT *object = new LINE(p1, p2, color);
					wndData->Object.push_back(object);
				}
				break;
				case 1:
				{
					readFile.read((char*)&p1, sizeof(POINT));
					readFile.read((char*)&p2, sizeof(POINT));
					readFile.read((char*)&color, sizeof(COLORREF));
					OBJECT *object = new RECTANGLE(p1, p2, color);
					wndData->Object.push_back(object);
				}
				break;
				case 2:
				{
					readFile.read((char*)&p1, sizeof(POINT));
					readFile.read((char*)&p2, sizeof(POINT));
					readFile.read((char*)&color, sizeof(COLORREF));
					OBJECT *object = new ELLIPSE(p1, p2, color);
					wndData->Object.push_back(object);
				}
				break;
				case 3:
				{
					readFile.read((char*)&p1, sizeof(POINT));
					readFile.read((char*)&color, sizeof(COLORREF));
					readFile.read((char*)&font, sizeof(LOGFONT));
					readFile.read((char*)&lenString, sizeof(int));
					input = new WCHAR[lenString];
					input[0] = '\0';
					readFile.read((char*)input, lenString * sizeof(WCHAR));
					OBJECT *object = new TEXTBOX(p1, input, color, font);
					wndData->Object.push_back(object);
				}
				break;
			}
		}
	}

}

void SaveFile(TCHAR *nameFile,CHILD_WND_DATA *wndData)
{
	ofstream writeFile;
	writeFile.open(nameFile, ios::binary | ios::out);
	bool saved = wndData->isSave;
	int size = wndData->Object.size();
	writeFile.write((char*)&saved, sizeof(bool));
	writeFile.write((char*)&size, sizeof(int));
	for (int i = 0; i < size; i++)
	{
		wndData->Object.at(i)->WriteFile(writeFile);
	}
	writeFile.close();
}

void OnFileSaveMenu(HWND hWnd)
{
	OPENFILENAME of;
	TCHAR szFile[256] ;
	TCHAR szName[MAX_LOADSTRING];
	TCHAR szFilter[] = TEXT("Draw file\0*.drw\0Text file\0*.txt\0");

	szFile[0] = '\0';
	szName[0] = '\0';

	CHILD_WND_DATA *wndData = (CHILD_WND_DATA*)GetWindowLongPtr(hChild, 0);
	if (wndData->isSave)
	{
		GetWindowText(hChild, szFile, 256);
		SaveFile(szFile, wndData);
	}
	else
	{
		ZeroMemory(&of, sizeof(OPENFILENAME));
		of.lStructSize = sizeof(OPENFILENAME);
		of.hwndOwner = hWnd;
		of.lpstrFilter = szFilter;
		of.nFilterIndex = 0;
		of.lpstrFile = szFile;
		of.lpstrFileTitle = szName;
		of.nMaxFile = sizeof(szFile);
		of.nMaxFileTitle = sizeof(szName);
		of.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		if (GetSaveFileName(&of))
		{
			wndData->isSave = true;
			SaveFile(of.lpstrFile, wndData);
			SetWindowText(hChild, szName);
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		CreateMDIClientWindows(hWnd);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
			HMENU hMenu = GetMenu(hWnd);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_FILE_NEW:
				CreateChildWindow(hWnd);
				break;
			case ID_FILE_OPEN:
				OnFileOpenMenu(hWnd);
				break;
			case ID_FILE_SAVE:
				OnFileSaveMenu(hWnd);
				break;
			case ID_DRAW_COLOR:
				OnDrawColorMenu(hWnd);
				break;
			case ID_DRAW_FONT:
				OnDrawFontMenu(hWnd);
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
				return DefFrameProc(hWnd, hWndMDIClient, message, wParam, lParam);
            }
        }
        break;
	case WM_SIZE:
		UINT w, h;	
		w = LOWORD(lParam);
		h = HIWORD(lParam);
		MoveWindow(hWndMDIClient, 0, TOOL_TIP_MAX_LEN, w, h, TRUE);
		//return DefFrameProc(hWnd, hWndMDIClient, message, wParam, lParam);
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_NOTIFY:
		doToolbarNotifyHandle(lParam);
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefFrameProc(hWnd, hWndMDIClient, message, wParam, lParam);
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

void doFileToolbar(HWND hWnd)
{
	InitCommonControls();
	TBBUTTON tbButton[] =
	{
		{ STD_FILENEW, ID_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON,0,0 },
	{ STD_FILEOPEN,ID_FILE_OPEN,TBSTATE_ENABLED, TBSTYLE_BUTTON,0,0 },
	{ STD_FILESAVE,ID_FILE_SAVE,TBSTATE_ENABLED, TBSTYLE_BUTTON,0,0 }
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
		{ 0,0,TBSTATE_ENABLED,TBSTYLE_SEP,0,0 },
		{ 0, ID_DRAW_LINE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ 1, ID_DRAW_RECTANGLE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ 2,ID_DRAW_ELLIPSE,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0 },
		{ 3,ID_DRAW_TEXT,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0 }
	};

	TBADDBITMAP tbBitmap = { hInst, IDB_BITMAP1 };
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

void InitChildWindow(HWND hWnd)
{
	hChild = hWnd;
	CHILD_WND_DATA *wndData;
	wndData = new CHILD_WND_DATA;
	ZeroMemory(wndData, sizeof(CHILD_WND_DATA));
	wndData->isSave = false;
	chooseColor = RGB(0, 0, 0);
	HFONT hFont = (HFONT)GetStockObject(SYSTEM_FONT);
	GetObject(hFont, sizeof(LOGFONT), (LPSTR)&chooseFont);
	SetLastError(0);
	if (SetWindowLongPtr(hWnd, 0, (LONG_PTR)wndData))
		if (GetLastError() != 0)
			MessageBox(hWnd, L"Cannot init data pointer of window", L"Error", MB_OK);
}

void ReDrawBitmap(HWND hWnd)
{
	CHILD_WND_DATA *wndData = (CHILD_WND_DATA*)GetWindowLongPtr(hWnd, 0);
	for (int i = wndData->Object.size() - 1; i >= 0; i--)
	{
		wndData->Object.at(i)->Draw(hdc);
	}
}

void SaveObject(HWND hWnd,POINT p1, POINT p2, OBJECT *objectType)
{
	CHILD_WND_DATA *wndData = (CHILD_WND_DATA*)GetWindowLongPtr(hWnd,0);
	wndData->Object.push_back(objectType);
}

void doDrawObject(HWND hWnd,HDC hdc, POINT &p1, POINT &p2)
{
	SetROP2(hdc, R2_MASKPEN);
	HMENU hMenu = GetMenu(hFrame);
	switch (drawPrevId)
	{
		case ID_DRAW_LINE:
		{
			LINE line(p1, p2,chooseColor);
			line.Draw(hdc);
			objectType = new LINE(p1, p2, chooseColor);
			objectType->type = 0;
		}
		break;
		case ID_DRAW_RECTANGLE:
		{
			RECTANGLE rec(p1, p2, chooseColor);
			rec.color = chooseColor;
			rec.Draw(hdc);
			objectType = new RECTANGLE(p1,p2,chooseColor);
			objectType->type = 1;

		}
		break;
		case ID_DRAW_ELLIPSE:
		{
			ELLIPSE el(p1, p2,chooseColor);
			el.color = chooseColor;
			el.Draw(hdc);
			objectType = new ELLIPSE(p1, p2, chooseColor);
			objectType->type = 2;

		}
		break;
	}
}

void ShowDialogText(HWND hWnd, POINT &p1)
{
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, TextBox);
	TEXTBOX text(p1,inputText, chooseColor, chooseFont);
	text.Draw(hdc);
	objectType = new TEXTBOX(p1,inputText,chooseColor, chooseFont);
	objectType->type = 3;
	SaveObject(hWnd, p1, p1, objectType);
}

void OnLeftButtonDown(POINT &p1, POINT &p2, LPARAM lParam)
{
	p1.x = p2.x = LOWORD(lParam);
	p1.y = p2.y = HIWORD(lParam);
	SelectObject(memDC, hBitmap);
	if (drawPrevId == ID_DRAW_TEXT)
	{
		ShowDialogText(hChild, p1);
	}
}

void OnMouseMove(HWND hWnd,POINT &p1, POINT &p2, LPARAM lParam)
{
	Moving = true;
	SelectObject(memDC, hBitmap);
	FillRect(memDC, &r, (HBRUSH)GetStockObject(WHITE_BRUSH));
	SelectObject(memDC, hPen);

	p2.x = LOWORD(lParam);
	p2.y = HIWORD(lParam);

	doDrawObject(hWnd,memDC, p1, p2);
	BitBlt(hdc, 0, 0, r.right - r.left, r.bottom - r.top, memDC, 0, 0, SRCCOPY);
	SetROP2(memDC, R2_MASKPEN);
	ReDrawBitmap(hWnd);

}

void OnLeftButtonUp(HWND hWnd, POINT &p1, POINT &p2)
{
	if (Moving == true)
	{
		Moving = false;
		DeleteDC(memDC);
		DeleteObject(hBitmap);
		ReleaseDC(hWnd, hdc);
		SaveObject(hWnd,p1, p2, objectType);
	}
}

LRESULT CALLBACK DrawWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static POINT po1, po2;
	static bool flag = false;
	GetClientRect(hWnd, &r);
	hdc = GetDC(hWnd);
	memDC = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, r.right - r.left, r.bottom - r.top);
	switch (message)
	{
	case WM_CREATE:
		InitChildWindow(hWnd);
		break;
	case WM_ERASEBKGND:
		return 0;
	case WM_SIZE:
		return DefMDIChildProc(hWnd, message, wParam, lParam);
	case WM_COMMAND:
		return DefMDIChildProc(hWnd, message, wParam, lParam);
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		SelectObject(memDC, hBitmap);
		FillRect(memDC, &r, (HBRUSH)GetStockObject(WHITE_BRUSH));
		SetROP2(memDC, R2_MASKPEN);
		BitBlt(hdc, 0, 0, r.right - r.left, r.bottom - r.top, memDC, 0, 0, SRCCOPY);
		ReDrawBitmap(hWnd);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_ACTIVATE:
		hChild = hWnd;
		break;
	case WM_LBUTTONDOWN:
	{
		OnLeftButtonDown(po1, po2, lParam);		
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (!(wParam & MK_LBUTTON))
			break;
		OnMouseMove(hWnd, po1, po2, lParam);
	}
	break;
	case WM_LBUTTONUP:
		OnLeftButtonUp(hWnd, po1, po2);
		break;
	case WM_DESTROY:
	{
		_count--;
		CHILD_WND_DATA *wndData =(CHILD_WND_DATA *) GetWindowLongPtr(hWnd, 0);
		wndData->Object.clear();
		delete wndData;
	}
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
INT_PTR CALLBACK TextBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hDlg, IDC_EDIT1, L"Input Text Here");
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
			case IDCANCEL:
				EndDialog(hDlg,wmId);
				return (INT_PTR)TRUE;
			case IDOK:
				GetDlgItemText(hDlg, IDC_EDIT1, inputText, 256);
				EndDialog(hDlg, wmId);
				return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}