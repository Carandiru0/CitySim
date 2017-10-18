// CityLauncher.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "atlstr.h"
#include "CityLauncher.h"

#include <curl/curl.h>
#include <string>

#define MAX_LOADSTRING 256

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HBITMAP bg;
HFONT font_gui;
CURL *curl;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

// Methods
void create_font(HFONT &font, HWND hwnd, LPWSTR name, int size);
void paint_image(HDC hdc, int image);
void create_panel(HWND hwnd, int x, int y, int w, int h);
HWND create_textbox(HWND hwnd, int x, int y, int w, int h);
HWND create_passbox(HWND hwnd, int x, int y, int w, int h);
void create_button(HWND hwnd, int x, int y, int w, int h);
void get_response(CURL *c, std::string url, std::string *str, std::string post);
size_t get_data(void *contents, size_t size, size_t nmemb, std::string *s);

HWND usr, pwd;

std::string wstrtostr(const std::wstring &wstr) {
	std::string strTo;
	char *szTo = new char[wstr.length() + 1];
	szTo[wstr.size()] = '\0';
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
	strTo = szTo;
	delete[] szTo;
	return strTo;
}

const wchar_t *GetWC(const char *c) {
	size_t len = 0;
	wchar_t *wc = new wchar_t[(strlen(c) + 1) * 2];

	mbstowcs_s(&len, wc, (strlen(c) + 1) * 2, c, _TRUNCATE);

	return wc;
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
    LoadStringW(hInstance, IDC_CITYLAUNCHER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CITYLAUNCHER));
    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CITYLAUNCHER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU, CW_USEDEFAULT, 0, 640, 480, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
      return FALSE;

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HBRUSH bWhite = CreateSolidBrush(RGB(255, 255, 255));
	static HBRUSH bGrey = CreateSolidBrush(RGB(242, 242, 242));
	
	switch (message) {
		case WM_CREATE: {
				curl = curl_easy_init();

				create_font(font_gui, hWnd, L"Consolas", 16);

				bg = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BGIMG));

				if (!bg)
					MessageBox(hWnd, L"Cannot load bg", L"Error", MB_ICONERROR);

				create_panel(hWnd, 142, 160, 355, 216);
				usr = create_textbox(hWnd, 172, 192, 295, 40);
				pwd = create_passbox(hWnd, 172, 252, 295, 40);
				create_button(hWnd, 172, 312, 295, 40);
			}

			break;

		case WM_PAINT: {
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);

				paint_image(hdc, IDB_BGIMG);

				EndPaint(hWnd, &ps);
			}

			break;

		case WM_CTLCOLORSTATIC: {
			if (GetDlgCtrlID((HWND)lParam) == IDC_PANEL1)
				return (INT_PTR)bWhite;
		}

			break;

		case WM_CTLCOLOREDIT: {
			if (GetDlgCtrlID((HWND)lParam) == IDC_TEXT_USR) {
				SetBkColor((HDC)wParam, RGB(242, 242, 242));
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(40, 40, 40));

				return (LRESULT)bGrey;
			}
		}

			break;

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDC_BTN_LOGIN:
					std::string data;
					std::string postdata;
					LPWSTR utxt, ptxt;
					
					utxt = new TCHAR[50], ptxt = new TCHAR[50];
					
					GetWindowText(usr, utxt, 50);
					GetWindowText(pwd, ptxt, 50);

					postdata = "usr=";
					postdata += wstrtostr(utxt);
					postdata += "&pwd=";
					postdata += wstrtostr(ptxt);

					get_response(curl, "localhost/city/api.php?login", &data, postdata);

					if(data.substr(0, 2) != "OK")
						MessageBox(hWnd, GetWC(data.c_str()), L"Error", MB_OK | MB_ICONINFORMATION);
					else {
						std::string sessid = data.substr(3, data.length() - 3);
						std::string cmd = "-sess " + sessid;

						STARTUPINFOA si;
						PROCESS_INFORMATION pi;

						ZeroMemory(&si, sizeof(si));
						si.cb = sizeof(si);
						ZeroMemory(&pi, sizeof(pi));

						// start the program up
						CreateProcessA("2D-City-Sim.exe", (LPSTR)cmd.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

						CloseHandle(pi.hProcess);
						CloseHandle(pi.hThread);

						exit(0);
					}

					break;
			}
		}

			break;

		case WM_DESTROY:
			curl_easy_cleanup(curl);
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

void create_font(HFONT &font, HWND hwnd, LPWSTR name, int size) {
	HDC hdc;
	HFONT fnt;
	long height;
	
	hdc = GetDC(hwnd);
	height = -MulDiv(size, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	ReleaseDC(hwnd, hdc);

	fnt = CreateFont(height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, name);

	if (fnt) {
		DeleteObject(font);
		font = fnt;
	} else
		MessageBox(hwnd, L"Font creation failed!", L"Error", MB_OK | MB_ICONEXCLAMATION);
}

void paint_image(HDC hdc, int image) {
	BITMAP b_bg;
	HDC m_bg = CreateCompatibleDC(hdc);
	HBITMAP h_bg = (HBITMAP)SelectObject(m_bg, bg);

	GetObject(bg, sizeof(b_bg), &b_bg);

	BitBlt(hdc, 0, 0, b_bg.bmWidth, b_bg.bmHeight, m_bg, 0, 0, SRCCOPY);

	SelectObject(m_bg, h_bg);
	DeleteDC(m_bg);
}

void create_panel(HWND hwnd, int x, int y, int w, int h) {
	HWND panel = CreateWindowEx(WS_EX_CLIENTEDGE, L"Static", L"", WS_CHILD | WS_VISIBLE, x, y, w, h, hwnd, (HMENU)IDC_PANEL1, hInst, nullptr);

	if(!panel)
		MessageBox(hwnd, L"Could not create panel..", L"Error", MB_OK | MB_ICONERROR);
}

HWND create_textbox(HWND hwnd, int x, int y, int w, int h) {
	HWND control = CreateWindowExW(WS_EX_CLIENTEDGE, L"Edit", L"Username", WS_CHILD | WS_VISIBLE | WS_TABSTOP, x, y, w, h, hwnd, (HMENU)IDC_TEXT_USR, hInst, nullptr);
	
	if (!control)
		MessageBox(hwnd, L"Could not create edit box.", L"Error", MB_OK | MB_ICONERROR);

	SendMessage(control, WM_SETFONT, (WPARAM)font_gui, MAKELPARAM(FALSE, 0));

	return control;
}

HWND create_passbox(HWND hwnd, int x, int y, int w, int h) {
	HWND control = CreateWindowExW(WS_EX_CLIENTEDGE, L"Edit", L"Password", WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_PASSWORD, x, y, w, h, hwnd, (HMENU)IDC_TEXT_PWD, hInst, nullptr);

	if (!control)
		MessageBox(hwnd, L"Could not create edit box.", L"Error", MB_OK | MB_ICONERROR);

	SendMessage(control, WM_SETFONT, (WPARAM)font_gui, MAKELPARAM(FALSE, 0));

	return control;
}

void create_button(HWND hwnd, int x, int y, int w, int h) {
	HWND btn = CreateWindowExW(WS_EX_CLIENTEDGE, L"Button", L"Login", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, x, y, w, h, hwnd, (HMENU)IDC_BTN_LOGIN, hInst, nullptr);

	SendMessage(btn, WM_SETFONT, (WPARAM)font_gui, MAKELPARAM(FALSE, 0));
}

void get_response(CURL *c, std::string url, std::string *str, std::string post) {
	curl_easy_setopt(c, CURLOPT_URL, url.c_str());
	curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, get_data);
	curl_easy_setopt(c, CURLOPT_FOLLOWLOCATION, true);
	curl_easy_setopt(c, CURLOPT_WRITEDATA, str);
	curl_easy_setopt(c, CURLOPT_POSTFIELDS, post.c_str());
	curl_easy_perform(c);
}

size_t get_data(void *contents, size_t size, size_t nmemb, std::string *s) {
	size_t newLength = size*nmemb;
	size_t oldLength = s->size();

	try {
		s->resize(oldLength + newLength);
	} catch (std::bad_alloc &e) {
		UNREFERENCED_PARAMETER(e);
		return 0;
	}

	std::copy((char*)contents, (char*)contents + newLength, s->begin() + oldLength);
	return size * nmemb;
}