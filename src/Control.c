#include <Windows.h>
#include "window.h"
#include "ControlPrivate.h"

// Add control to control list
void window_add_control(struct Window* window, struct Control* control);

// Control Abstract
Control* __Control_create_object() {
	return (Control*)malloc(sizeof(Control));
}

void __Control_ctor(Control* control, struct Window* pWindow, UINT controlID, UINT x, UINT y, UINT w, UINT h) {
	control->window = pWindow;
	control->controlId = controlID;
	control->x = x;
	control->y = y;
	control->w = w;
	control->h = h;
}


HWND Control_get_HWND(Control* control) {
	return control->hWnd;
}
// Seters

void Control_set_isEnabled(Control* control, BOOL is) {
	control->isEnabled = is;
}

void Control_set_isVisible(Control* control, BOOL is) {
	control->isEnabled = is;
}


// Button

Button* Button_create_object() {
	return (Button*)malloc(sizeof(Button));
}

void __Button_create_window(Button* button, PWSTR text) {
	Control* control = (Control*)button;
	HWND hWnd = CreateWindow(L"Button", text, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, control->x, control->y, control->w, control->h,
		window_getHWND(control->window), 0, window_getHInstance(control->window), 0);
	if (hWnd == 0) {
		wchar_t buffer[255];
		wsprintf(buffer, L"Error code - %d", GetLastError());
		MessageBox(0, buffer,
			L"Fatal error", MB_OK | MB_ICONERROR);
		ExitProcess(1);
	}
	control->hWnd = hWnd;
}

void Button_ctor(Button* button, struct Window *pWindow, PWSTR text, UINT controlID, UINT x, UINT y, UINT w, UINT h) {
	__Control_ctor((Control*)button, pWindow, controlID, x, y, w, h);
	__Button_create_window(button, text);
	button->onClick = NULL;
	window_add_control(pWindow, (Control*)button);
}


void Button_set_onClick(Button* button, Button_on_click onClick) {
	button->onClick = onClick;
}

// TextEdit

void __TextEdit_create_window(TextEdit* textEdit, PWSTR text) {
	Control* control = (Control*)textEdit;
	HWND hWnd = CreateWindow(L"EDIT", text, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_LEFT |
		ES_MULTILINE | ES_AUTOVSCROLL, control->x, control->y, control->w, control->h,
		window_getHWND(control->window), 0, window_getHInstance(control->window), 0);
	if (hWnd == 0) {
		MessageBox(0, L"Fatal error",
			L"Fatal error", MB_OK | MB_ICONERROR);
		ExitProcess(1);
	}
	control->hWnd = hWnd;
}

TextEdit* TextEdit_create_object() {
	return (TextEdit*)malloc(sizeof(TextEdit));
}

void TextEdit_ctor(TextEdit* textEdit, struct Window* pWindow, PWSTR text, UINT controlID, UINT x, UINT y, UINT w, UINT h) {
	__Control_ctor((Control*)textEdit, pWindow, controlID, x, y, w, h);
	textEdit->readOnly = FALSE;
	textEdit->isTransparent = FALSE;
	textEdit->textColor = RGB(255, 0, 0);
	__TextEdit_create_window(textEdit, text);
	window_add_control(pWindow, (Control*)textEdit);
}

void TextEdit_setReadOnly(TextEdit* textEdit, BOOL is) {
	Control* control = (Control*)textEdit;
	is ? SendMessage(control->hWnd, EM_SETREADONLY, TRUE, 0) :
		SendMessage(control->hWnd, EM_SETREADONLY, FALSE, 0);
}

// Seters
void TextEdit_setText(TextEdit* textEdit, PWSTR text) {
	Control* control = (Control*)textEdit;
	if (control == NULL) {
		return;
	}
	SetWindowText(control->hWnd, text);
}

void TextEdit_set_isTransparent(TextEdit* textEdit, BOOL is) {
	textEdit->isTransparent = is;
}

// Geters
void TextEdit_getText(TextEdit* textEdit, PWSTR buffer) {
	Control* control = (Control*)textEdit;
	GetWindowText(control->hWnd, buffer, sizeof(buffer));
}

// StaticWindow
void __StaticWindow_create_window(StaticWindow* staticWindow, PWSTR text) {
	Control* control = (Control*)staticWindow;
	HWND hWnd = CreateWindowEx(0, L"STATIC", text, WS_CHILD | WS_VISIBLE | SS_LEFT
		,control->x, control->y, control->w, control->h,
		window_getHWND(control->window), 0, window_getHInstance(control->window), 0);
	if (hWnd == 0) {
		MessageBox(0, L"Fatal error",
			L"Fatal error", MB_OK | MB_ICONERROR);
		ExitProcess(1);
	}
	control->hWnd = hWnd;
}

StaticWindow* StaticWindow_create_object() {
	return (StaticWindow*)malloc(sizeof(StaticWindow));
}

void StaticWindow_ctor(StaticWindow* staticWindow, 
	struct Window* pWindow, PWSTR text, UINT controlID, UINT x, UINT y, UINT w, UINT h) {
	__Control_ctor((Control*)staticWindow, pWindow, controlID, x, y, w, h);
	staticWindow->isTransparent = FALSE;
	staticWindow->bkColor = RGB(0, 0, 0);
	staticWindow->textColor = RGB(0, 0, 0);
	__StaticWindow_create_window(staticWindow, text);
	window_add_control(pWindow, (Control*)staticWindow);
}

// Seters

void StaticWindow_set_isTransparent(StaticWindow* staticWindow, BOOL is) {
	staticWindow->isTransparent = is;
}

void StaticWindow_set_TextColor(StaticWindow* staticWindow, COLORREF color) {
	staticWindow->textColor = color;
}

void StaticWindow_set_BkColor(StaticWindow* staticWindow, COLORREF color) {
	staticWindow->bkColor = color;
}

void StaticWindow_setText(StaticWindow* staticWindow, PWSTR text) {
	Control* control = (Control*)staticWindow;
	if (control == NULL) {
		return;
	}
	SetWindowText(control->hWnd, text);
}