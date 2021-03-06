// HW13.1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "HW13.1.h"
#include <mmsystem.h>
#include <Windows.h>
#include <commdlg.h>
#include <CommCtrl.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR szFileName[256];
MCIDEVICEID mciDevID = -1;

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
    LoadStringW(hInstance, IDC_HW131, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HW131));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HW131));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_HW131);
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


void doOpenMedia(HWND hWnd)
{
	OPENFILENAME of;
	TCHAR szFile[256];
	TCHAR szFilter[] = L"AVI file\0*.avi\0Wave File\0*.wav\0MP3 File\0*.mp3\0";

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
	{
		/*wcscpy_s(szFileName,of.lpstrFile);
		MCI_OPEN_PARMS mciOpen;
		mciOpen.lpstrDeviceType = L"waveaudio";
		mciOpen.lpstrElementName = of.lpstrFile;
		MCIERROR mciError = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpen);
		audioID = mciOpen.wDeviceID;*/
		HMENU hMenu = GetMenu(hWnd);
		EnableMenuItem(hMenu, ID_PLAY, MF_ENABLED | MF_BYCOMMAND);
		EnableMenuItem(hMenu, ID_PAUSE, MF_ENABLED | MF_BYCOMMAND);
		EnableMenuItem(hMenu, ID_RESUME, MF_ENABLED | MF_BYCOMMAND);
		EnableMenuItem(hMenu, ID_STOP, MF_ENABLED | MF_BYCOMMAND);
		EnableMenuItem(hMenu, ID_VOLUME, MF_ENABLED | MF_BYCOMMAND);
		EnableMenuItem(hMenu, ID_VOLUME32777, MF_ENABLED | MF_BYCOMMAND);
		wsprintf(szFileName, szFile);
	}
}

void playMedia(HWND hWnd)
{
	mciSendCommand(mciDevID, MCI_CLOSE, 0, 0);
	MCI_OPEN_PARMS mciOpen;
	MCIERROR dwReturn;
	mciOpen.lpstrDeviceType = L"mpegvideo";
	mciOpen.lpstrElementName = szFileName;
	if (dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,(DWORD_PTR)&mciOpen))
	{
		WCHAR szError[128];
		mciGetErrorString(dwReturn, szError, 128);
		MessageBox(hWnd, szError, L"Error", MB_OK);
		return;
	}
	mciDevID = mciOpen.wDeviceID;

	MCI_PLAY_PARMS mciPlay;
	mciPlay.dwCallback = (unsigned long)hWnd;
	
	DWORD volume;
	waveOutGetVolume(NULL, &volume);
	volume = volume | volume >> 16;
	waveOutSetVolume(NULL, volume);
	if (dwReturn = mciSendCommand(mciDevID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mciPlay))
	{
		WCHAR szError[128];
		mciSendCommand(mciDevID, MCI_CLOSE, 0, 0L);
		mciGetErrorString(dwReturn, szError, 128);
		MessageBox(hWnd, szError, L"Error", MB_OK);
		return;
	}
}

void pauseMedia()
{
	if (mciDevID)
	{
		mciSendCommand(mciDevID, MCI_PAUSE, 0, 0);
	}
}

void resumMedia()
{
	if (mciDevID)
	{
		mciSendCommand(mciDevID, MCI_RESUME, 0, 0);
	}
}

void stopMedia()
{
	if (mciDevID)
	{
		mciSendCommand(mciDevID, MCI_CLOSE, 0, 0);
	}
}

void volumeUpMedia()
{
	if (mciDevID)
	{
		DWORD volume;
		waveOutGetVolume(NULL, &volume);
		if (LOWORD(volume) + 3000 >= 0xFFFF)
		{
			volume = 0;
			volume |= 0xFFFF;
			volume = volume | volume << 16;
		}
		else
		{
			volume += 1000;
			volume = volume | volume << 16;
		}
		waveOutSetVolume(NULL,volume);
	}
}
void volumeDownMedia()
{
	if (mciDevID)
	{
		DWORD volume;
		waveOutGetVolume(NULL,&volume);
		if (LOWORD(volume)  - 3000 < 0x0000)
		{
			volume = 0x0000;
			volume = volume | volume << 16;
		}
		else
		{
			volume -= 1000;
			volume =volume << 16 | LOWORD(volume);
		}
		waveOutSetVolume(NULL, volume);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HMENU hMenu = GetMenu(hWnd);
    switch (message)
    {
	case WM_CREATE:
	{
		szFileName[0] = '\0';
		EnableMenuItem(hMenu, ID_PLAY, MF_DISABLED | MF_BYCOMMAND);
		EnableMenuItem(hMenu, ID_PAUSE, MF_DISABLED | MF_BYCOMMAND);
		EnableMenuItem(hMenu,ID_RESUME, MF_DISABLED | MF_BYCOMMAND);
		EnableMenuItem(hMenu, ID_STOP, MF_DISABLED | MF_BYCOMMAND);
		EnableMenuItem(hMenu, ID_VOLUME, MF_DISABLED | MF_BYCOMMAND);
		EnableMenuItem(hMenu,ID_VOLUME32777, MF_DISABLED | MF_BYCOMMAND);
	}
	break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_FILE_OPEN:
				doOpenMedia(hWnd);
				break;
			case ID_PLAY:
				playMedia(hWnd);
				break;
			case ID_PAUSE:
				pauseMedia();
				break;
			case ID_RESUME:
				resumMedia();
				break;
			case ID_STOP:
				stopMedia();
				break;
			case ID_VOLUME:
				volumeUpMedia();
				break;
			case ID_VOLUME32777:
				volumeDownMedia();
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
	//case MM_MCINOTIFY:
	//	// Dong Multi-media device
	//	mciSendCommand(lParam, MCI_CLOSE, 0, 0L);
	//	return 0;
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
