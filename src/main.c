#if !defined(_WIN32) && !defined(_WIN64)
#pragma message ("Windows only")
#endif
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <Windows.h>
#include <stdbool.h>
#define ERRORMSG(msg) MessageBoxA(0, msg, WIN_NAME, 0)

#define WIN_NAME "AntiIdle"
#define WIN_WIDTH 250
#define WIN_HEIGHT 150
#define BTN_STR_START "Start"
#define BTN_STR_STOP "Stop"
#define BTN_CMD 69
#define TIMER 420

bool bStarted = false;

LONG_PTR __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

int main() {

	WNDCLASSEXA wclsx = {0};
	wclsx.cbSize = sizeof(WNDCLASSEXA);
	wclsx.lpfnWndProc = WndProc;
	wclsx.lpszClassName = WIN_NAME;
	wclsx.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wclsx.hInstance = GetModuleHandle(0);
	wclsx.hIcon = wclsx.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wclsx.hCursor = LoadCursor(0, IDC_ARROW);
	wclsx.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassExA(&wclsx)) {
		ERRORMSG("Error registering class");
		return -1;
	}
	
	HWND window = CreateWindowExA(0, WIN_NAME, WIN_NAME, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		GetSystemMetrics(SM_CXSCREEN) / 2 - WIN_WIDTH / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - WIN_HEIGHT / 2, WIN_WIDTH, WIN_HEIGHT, 0, 0, wclsx.hInstance, 0);
	if (!window) {
		ERRORMSG("Error creating window");
		return -2;
	}

	ShowWindow(window, SW_SHOW);
	UpdateWindow(window);

	HWND btn = CreateWindowExA(0, "button", BTN_STR_START, WS_VISIBLE | WS_CHILD | BS_CENTER, 0,0,WIN_WIDTH-15,WIN_HEIGHT-35, window, (HMENU)BTN_CMD, wclsx.hInstance, 0);

	MSG msg = { 0 };
	
	while (GetMessageA(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return 0;
}

LONG_PTR __stdcall WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_COMMAND:
		if (HIWORD(wparam) == BN_CLICKED && LOWORD(wparam) == BTN_CMD)
			if (!bStarted) {
				SetTimer(hwnd, TIMER, 1000, 0);
				SetWindowTextA((HWND)lparam, BTN_STR_STOP);
				bStarted = true;
			}
			else {
				KillTimer(hwnd, TIMER);
				SetWindowTextA((HWND)lparam, BTN_STR_START);
				bStarted = false;
			}
				
		break;
	case WM_TIMER:
		POINT curPos = {0};
		GetCursorPos(&curPos);
		SetCursorPos(curPos.x+1, curPos.y+1);
		Sleep(100);
		SetCursorPos(curPos.x, curPos.y);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProcA(hwnd, msg, wparam, lparam);
	}
}
