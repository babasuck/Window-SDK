/*
	Window Internal
	By Mantissa 15.04.2023
*/
#include <Windows.h>
#include "window.h"
#include "GraphicsRenderer.h"
#include "ControlPrivate.h"
#include "stdio.h"

typedef struct Window {
	ATOM aWnd;
	HWND hWnd;
	HINSTANCE hInstance;
	PWSTR windowName;
	UINT w;
	UINT h;
	struct GraphicsRenderer *graphicsRenderer;
	Window_keyboard_handler keyboardHandler;
	HBRUSH hbrBg;
	Control* controlsList[MAX_CONTROLS];
	UINT controlsCount;
	WNDPROC wndProc;
	RECT *clientRect;
	BOOL isVisible;
	BOOL isResizeble;
} Window;

// Internal functions
LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL __window_register(Window* window);
BOOL __window_create(Window* window);

// Return the Window object
Window* window_create_object() {
	return (Window*)malloc(sizeof(Window));
}

/*
* Constructor fow window object, create window
*/
void window_ctor(Window* window, HINSTANCE hInstance, PWSTR windowName, int w, int h) {
	window->hInstance = hInstance;
	window->windowName = windowName;
	window->wndProc = (WNDPROC)WndProc;
	window->controlsCount = 0;
	window->w = w;
	window->h = h;
	window->keyboardHandler = NULL;
	window->isResizeble = TRUE;
	if (window->clientRect = (RECT*)malloc(sizeof(RECT))) {
		GetClientRect(window->hWnd, window->clientRect);
	};
	window->graphicsRenderer = GraphicsRenderer_create_object();
	window->clientRect = (RECT*)malloc(sizeof(RECT));
	__window_create(window);
	GraphicsRenderer_ctor(window->graphicsRenderer, window, window->hbrBg);
}

// Destructor for Window object
void window_dtor(Window* window) {
	GraphicsRenderer_dtor(window->graphicsRenderer);
	free(window->clientRect);
	free(window);
	ExitProcess(1);
}

// Register a window
BOOL __window_register(Window* window) {
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpszClassName = window->windowName;
	wndClass.hInstance = window->hInstance;
	wndClass.lpfnWndProc = window->wndProc;
	HBRUSH bgBrush = CreateSolidBrush(RGB(61, 64, 74));
	window->hbrBg = bgBrush;
	wndClass.hbrBackground = bgBrush;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	ATOM atom = RegisterClassEx(&wndClass);
	if (atom == INVALID_ATOM) {
		MessageBox(0, L"Error while register window class.", L"Fatal error", MB_OK);
		return FALSE;
	}
	window->aWnd = atom;
	return TRUE;
}

// Call the CreateWindowEx function
BOOL __window_create(Window* window) {
	if (!__window_register(window)) {
		return FALSE;
	} //| WS_CLIPCHILDREN
	HWND hWnd = CreateWindowEx(0 , window->windowName, window->windowName, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
	window->w, window->h, 0, 0, window->hInstance, window);
	if (hWnd == NULL) { 
		MessageBox(0, L"Error while create window.", L"Fatal error", MB_OK);
		return FALSE;
	}
	return TRUE;
}

// WndProc
LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Window* window = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	switch (msg) {
		case WM_CREATE:
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			window = pCreate->lpCreateParams;
			window->hWnd = hWnd;
			SetWindowLongPtr(window->hWnd, GWLP_USERDATA, (LONG_PTR)window);
			return 0;
		}
		case WM_DESTROY:
			window_dtor(window);
			return 0;
		case WM_PAINT: 
		{
			PAINTSTRUCT ps = { 0 };
			HDC hdc = BeginPaint(hWnd, &ps);
			BitBlt(hdc, 0, 0, window->clientRect->right, window->clientRect->bottom, GraphicsRenderer_getBackDC(window->graphicsRenderer), 0, 0, SRCCOPY);
			EndPaint(hWnd, &ps);
			return 0;
		}
		case WM_COMMAND:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				for (UINT i = 0; i < window->controlsCount; i++) {
					Control* control = (Control*)window->controlsList[i];
					if (control->hWnd == (HWND)lParam) {
						Button* button = (Button*)control;
						if (button->onClick) {
							button->onClick(window, button);
						}
						return 0;
					}
				}
			}
		}
		case WM_KEYDOWN:
			if (window->keyboardHandler) {
				window->keyboardHandler(window, wParam);
			}
			return 0;
		case WM_SIZE: {
			GetClientRect(window->hWnd, window->clientRect);
			GraphicsRenderer_setClientChange(window->graphicsRenderer);
			return 0;
		}
		case WM_ERASEBKGND: {
			return 1;
		}
		case WM_CTLCOLORSTATIC: {
			HDC hdcStatic = (HDC)wParam;
			for (UINT i = 0; i < window->controlsCount; i++) {
				if (window->controlsList[i]->hWnd == (HWND)lParam) {
					StaticWindow* staticWindow = (StaticWindow*)window->controlsList[i];
					SetTextColor(hdcStatic, staticWindow->textColor);
					if (staticWindow->isTransparent) {
						SetBkMode(hdcStatic, TRANSPARENT);
						return (INT_PTR)GetStockObject(NULL_BRUSH);
					}
					else {
						SetBkColor(hdcStatic, staticWindow->bkColor);
					}
				}
			}
			return (INT_PTR)GetStockObject(NULL_BRUSH);
		}
		default:
			return(DefWindowProcW(hWnd, msg, wParam, lParam));
	}
}

// Start WndProc routine
void window_start_message_handling(Window *window) {
	MSG msg = { 0 };
	while (GetMessage(&msg, window->hWnd, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

}

// Send to graphicsRenderer a message to start Render routine
void window_start_rendering(Window* window) {
	GraphicsRenderer_startRender(window->graphicsRenderer);
}

void window_update_controls(Window* window) {
	for (int i = 0; i < window->controlsCount; i++) {
		if (window->controlsList[i] != NULL) {
			InvalidateRect(Control_get_HWND(window->controlsList[i]), NULL, 1);
		}
	}
}

void window_add_control(Window* window, Control* control) {
	window->controlsList[window->controlsCount++] = control;
}

// Geters

HWND window_getHWND(Window* window) {
	return window->hWnd;
}

RECT *window_getClientRect(Window* window) {
	return window->clientRect;
}

HINSTANCE window_getHInstance(Window* window) {
	return window->hInstance;
}

struct Control* window_getControl_byID(Window* window, UINT id) {
	for (UINT i = 0; i < window->controlsCount; i++) {
		if (window->controlsList[i]->controlId == id) {
			return window->controlsList[i];
		}
	}
	return NULL;
}

struct GraphicsRenderer* window_get_graphicsRenderer(Window* window) {
	return window->graphicsRenderer;
}

// Seters

void window_setBgColor(Window* window, COLORREF color) {
	DeleteObject(window->hbrBg);
	window->hbrBg = CreateSolidBrush(color);
	GraphicsRenderer_setBgBrush(window->graphicsRenderer, window->hbrBg);
}

void window_set_KeyboardHandler(Window* window, Window_keyboard_handler handler) {
	window->keyboardHandler = handler;
}