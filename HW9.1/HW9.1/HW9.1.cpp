// HW9.1.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Windowsx.h>

#define EXPORT __declspec(dllexport)
HHOOK hHook = NULL;
HINSTANCE hinstLib;

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	static bool check=1;
	static BYTE keyboard[255];
	if (nCode < 0|| nCode != HC_ACTION) // không xử lý message 
		return CallNextHookEx(hHook, nCode, wParam, lParam);
	// xử lý message: Ctrl + Shift +L
	if (wParam == WM_KEYDOWN)
	{
		keyboard[((KBDLLHOOKSTRUCT*)lParam)->vkCode] = 1;
	}
	if (wParam == WM_KEYUP)
	{
		keyboard[((KBDLLHOOKSTRUCT*)lParam)->vkCode] = 0;
	}
	//Ctrl+Shift+U
	if (keyboard[162] && keyboard[160] && keyboard[85])
		check = 0;
	if (check)
		return 1;

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

EXPORT void _doInstallHook(HWND hWnd)
{
	if (hHook != NULL) return;

	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardHookProc, hinstLib, 0);
	if (hHook)
		MessageBox(hWnd, L"Setup hook successfully", L"Result", MB_OK);
	else
		MessageBox(hWnd, L"Setup hook fail", L"Result", MB_OK);
}

EXPORT void _doRemoveHook(HWND hWnd)
{
	if (hHook == NULL) return;
	UnhookWindowsHookEx(hHook);
	hHook = NULL;
	MessageBox(hWnd, L"Remove hook successfully", L"Result", MB_OK);
}

//EXPORTS void dllSampleFunction(HWND hWnd, WCHAR *msg)
//{
//	MessageBox(hWnd, msg, L"Notice", MB_OK);
//}