#include "window.h"
#include "GraphicsRenderer.h"
#include "GraphicObject.h"
#include <stdint.h>

int WinMain() {
	struct Window* win = window_create_object();
	window_ctor(win, 0, L"Test", 500, 500);
	// beta init
	struct Line* line = line_ctor(win, 100, 100);
	window_start_rendering(win);
	window_start_message_handling(win);
}