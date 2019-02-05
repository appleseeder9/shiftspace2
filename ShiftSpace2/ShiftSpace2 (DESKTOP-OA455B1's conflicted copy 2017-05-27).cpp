// ShiftSpace2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ShiftSpace2.h"
#include "windows.h"
#include "shellapi.h"
#include "VersionHelpers.h"

#ifdef UNICODE
#define stringcopy wcscpy_s
#else
#define stringcopy strcpy
#endif

#define ID_TRAY_APP_ICON                50000
#define ID_TRAY_EXIT_CONTEXT_MENU_ITEM  30000
#define WM_TRAYICON ( WM_USER + 1 )

UINT WM_TASKBARCREATED = 0;
HWND hWnd;
HMENU g_menu;
NOTIFYICONDATA g_notifyIconData;

#define MAX_LOADSTRING		100
#define HOT_KEY_ID_HANGUL	10001
#define HOT_KEY_ID_HANJA	10002
#define KO					      0x4120412
#define WM_INPUTLANGCHANGEREQUEST       0x0050
#define WM_INPUTLANGCHANGE              0x0051

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




void InitNotifyIconData(HINSTANCE hInstance)
{
	memset(&g_notifyIconData, 0, sizeof(NOTIFYICONDATA));

	g_notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	g_notifyIconData.hWnd = hWnd;
	g_notifyIconData.uID = ID_TRAY_APP_ICON;
	g_notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	g_notifyIconData.uCallbackMessage = WM_TRAYICON;
	g_notifyIconData.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHIFTSPACE2));
	stringcopy(g_notifyIconData.szTip, TEXT("Shift+Space Korean IME. Right click to exit"));
}


void MyScreen(int MyWidth, int MyHeight)
{
	DEVMODE newSettings;
	EnumDisplaySettings(NULL, 0, &newSettings);
	//newSettings.dmSize = sizeof(newSettings);
	newSettings.dmPelsWidth = MyWidth;
	newSettings.dmPelsHeight = MyHeight;
	newSettings.dmBitsPerPel = 32;
	newSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	ChangeDisplaySettings(&newSettings, CDS_FULLSCREEN);
}

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
    LoadStringW(hInstance, IDC_SHIFTSPACE2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SHIFTSPACE2));
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHIFTSPACE2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}

//   FUNCTION: InitInstance(HINSTANCE, int)
//   PURPOSE: Saves instance handle and creates main window

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   HWND hPrev = NULL;
   if (hPrev = FindWindow(szWindowClass, TEXT("ShiftSpace2"))) {
	   MessageBox(NULL, TEXT("Another instance alredy running!"), TEXT("Warning"), MB_OK);
	   return 0;
   }
  
   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      100, 100, 100, 100, nullptr, nullptr, hInstance, nullptr);

   RegisterHotKey(hWnd, HOT_KEY_ID_HANGUL, MOD_SHIFT | MOD_NOREPEAT, VK_SPACE);
   RegisterHotKey(hWnd, HOT_KEY_ID_HANJA, MOD_CONTROL | MOD_NOREPEAT, VK_SPACE);
   g_menu = CreatePopupMenu();
   AppendMenu(g_menu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, TEXT("Exit"));
   InitNotifyIconData(hInstance);
   Shell_NotifyIcon(NIM_ADD, &g_notifyIconData);

   //   TCHAR szMyString[10] = { 0 }; // Have enough storage in character array.
 //  _stprintf(szMyString, TEXT("0x%08x"), osvi.dwMajorVersion);
   
 //  MessageBox(NULL, (szMyString), _T("Language Triggered"), MB_OK | MB_SYSTEMMODAL);

   if (!hWnd)
   {
      return FALSE;
   }

   //ShowWindow(hWnd, nCmdShow);
   ShowWindow(hWnd, WS_EX_TOOLWINDOW);
   UpdateWindow(hWnd);
   
   //ShowWindow(hWnd, SW_SHOWMINIMIZED);
   PostMessage(hWnd, WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
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

	case WM_TRAYICON:
	{
		if (lParam == WM_RBUTTONDOWN)
		{
			POINT curPoint;
			GetCursorPos(&curPoint);
			UINT clicked = TrackPopupMenu(g_menu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, 0, hWnd, NULL);
			if (clicked == ID_TRAY_EXIT_CONTEXT_MENU_ITEM)
			{
				Shell_NotifyIcon(NIM_DELETE, &g_notifyIconData);
				PostQuitMessage(0);
			}

		}
	}
	break;

	case WM_HOTKEY:
	{
			int id = (int)wParam;
			HWND handle = GetForegroundWindow();
			DWORD dwThread =  GetWindowThreadProcessId(handle, NULL);
			HKL lang = GetKeyboardLayout(dwThread);
			switch (id)
			{ 
			case HOT_KEY_ID_HANGUL:
				if (LOWORD(lang)!=0x412)
				{
					//PostMessage(handle, WM_INPUTLANGCHANGEREQUEST, 0, KO);
					if (IsWindows8OrGreater())
					{
						keybd_event(VK_LWIN, 0xf2, KEYEVENTF_EXTENDEDKEY | 0, 0);
						keybd_event(VK_SPACE, 0xf2, KEYEVENTF_EXTENDEDKEY | 0, 0);
						keybd_event(VK_SPACE, 0xf2, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
						keybd_event(VK_LWIN, 0xf2, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
					}
					else
					{
						//PostMessage(handle, WM_INPUTLANGCHANGEREQUEST, 0, KO);
						PostMessage(HWND_BROADCAST, WM_INPUTLANGCHANGEREQUEST, 0, KO);
					}
				}
				//keybd_event(VK_HANGUL, 0xf2, KEYEVENTF_EXTENDEDKEY | 0, 0);
				keybd_event(VK_HANGUL, 0xf2, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
				break;
			case HOT_KEY_ID_HANJA:
				if (LOWORD(lang) != 0x412)
				{
					//PostMessage(handle, WM_INPUTLANGCHANGEREQUEST, 0, KO);
					if (IsWindows8OrGreater())
					{
						keybd_event(VK_LWIN, 0xf2, KEYEVENTF_EXTENDEDKEY | 0, 0);
						keybd_event(VK_SPACE, 0xf2, KEYEVENTF_EXTENDEDKEY | 0, 0);
						keybd_event(VK_SPACE, 0xf2, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
						keybd_event(VK_LWIN, 0xf2, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
					}
					else
					{
						//PostMessage(handle, WM_INPUTLANGCHANGEREQUEST, 0, KO);
						PostMessage(HWND_BROADCAST, WM_INPUTLANGCHANGEREQUEST, 0, KO);
					}
				}
				//keybd_event(VK_HANJA, 0xf2, KEYEVENTF_EXTENDEDKEY | 0, 0);
				keybd_event(VK_HANJA, 0xf2, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
				break;
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
