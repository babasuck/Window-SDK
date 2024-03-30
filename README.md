# Windows SDK - библиотека для создания оконных приложений под Windows

Это библиотека на чистом C для создания оконных приложений под Windows в псевдо объектно-ориентированном стиле. В настоящее время графическим обработчиком служит GDI (Graphics Device Interface).

```c
#include "window.h"
#include "GraphicsRenderer.h"
#include "GraphicObject.h"
#include <stdint.h>

// By default it compiles like executable 
int WinMain() {
	struct Window* win = window_create_object();
	window_ctor(win, 0, L"Test", 500, 500);
	struct Line* line = line_ctor(win, 100, 100);
	window_start_rendering(win);
	window_start_message_handling(win);
}
