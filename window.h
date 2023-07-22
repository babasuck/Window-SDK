/*
	Window External
	By Mantissa 15.04.2023
*/
#pragma once
#include <Windows.h>
struct Window;

struct Window* window_create_object();
void window_ctor(struct Window* window, HINSTANCE hInstance, PWSTR windowName, int w, int h);
void window_dtor(struct Window* window);
// Start WndProc message proccesing routine
void window_start_message_handling(struct Window* window);
// Start rendering graphics routine
void window_start_rendering(struct Window* window);
void window_update_controls(struct Window* window);

// Handlers
typedef void (*Window_keyboard_handler)(struct Window* window, UINT_PTR data);
// Geters
HWND window_getHWND(struct Window* window);
RECT* window_getClientRect(struct Window* window);
HINSTANCE window_getHInstance(struct Window* window);
struct Control* window_getControl_byID(struct Window* window, UINT id);
struct GraphicsRenderer* window_get_graphicsRenderer(struct Window* window);
// Seters
void window_setBgColor(struct Window* window, COLORREF color);
void window_set_KeyboardHandler(struct Window* window, Window_keyboard_handler handler);