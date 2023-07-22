#pragma once
#include <Windows.h>
#include "Control.h"


typedef struct Control {
	HWND hWnd;
	struct Window* window;
	UINT controlId;
	UINT x;
	UINT y;
	UINT w;
	UINT h;
	BOOL isEnabled;
	BOOL isVisible;
} Control;

#define MAX_CONTROLS 255

typedef struct Button {
	Control control;
	Button_on_click onClick;
} Button;

typedef struct TextEdit {
	Control control;
	BOOL isTransparent;
	COLORREF textColor;
	BOOL readOnly;
} TextEdit;

typedef struct StaticWindow {
	Control control;
	BOOL isTransparent;
	COLORREF textColor;
	COLORREF bkColor;
} StaticWindow;