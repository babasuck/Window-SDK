#pragma once

#include <Windows.h>

struct Control;
struct Button;
struct TextEdit;
struct Window;

typedef void (*Button_on_click)(struct Window* window, struct Button*);

void Control_set_isEnabled(struct Control* control, BOOL is);
void Control_set_isVisible(struct Control* control, BOOL is);
HWND Control_get_HWND(struct Control* control);

// Button
struct Button* Button_create_object();
void Button_ctor(struct Button* button, struct Window* pWindow, PWSTR text, UINT controlID, UINT x, UINT y, UINT w, UINT h);
void Button_set_onClick(struct Button* button, Button_on_click onClick);

// Edit
struct TextEdit* TextEdit_create_object();
void TextEdit_ctor(struct TextEdit* textEdit, struct Window* pWindow, PWSTR text, UINT controlID, UINT x, UINT y, UINT w, UINT h);
// Seters
void TextEdit_setReadOnly(struct TextEdit* textEdit, BOOL is);
void TextEdit_setText(struct TextEdit* textEdit, PWSTR text);
// Getes
void TextEdit_getText(struct TextEdit* textEdit, PWSTR buffer);
void TextEdit_set_isTransparent(struct TextEdit* textEdit, BOOL is);

//StaticWindow

struct StaticWindow* StaticWindow_create_object();
void StaticWindow_ctor(struct StaticWindow* staticWindow,
	struct Window* pWindow, PWSTR text, UINT controlID, UINT x, UINT y, UINT w, UINT h);

// Seters
void StaticWindow_set_isTransparent(struct StaticWindow* staticWindow, BOOL is);
void StaticWindow_setText(struct StaticWindow* staticWindow, PWSTR text);
void StaticWindow_set_TextColor(struct StaticWindow* staticWindow, COLORREF color);
void StaticWindow_set_BkColor(struct StaticWindow* staticWindow, COLORREF color);