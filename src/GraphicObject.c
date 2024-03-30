#include "GraphicObjectPrivate.h"
#include "window.h"
#include "GraphicsRenderer.h"
#include <stdint.h>

void __line_draw(Line* line, HDC hdc);

GraphicObject* graphic_object_ctor(struct Window* window, uint32_t x, uint32_t y, graphic_object_draw_routine drawRoutine) {
	GraphicObject* obj = (GraphicObject*)malloc(sizeof (GraphicObject));
	obj->x = x;
	obj->y = y;
	obj->drawRoutine = drawRoutine;
	GraphicsRenderer_addGraphicObject(window_get_graphicsRenderer(window), obj);
	return obj;
}
graphic_object_draw_routine* graphic_object_get_draw(GraphicObject* graphicObject) {
	return graphicObject->drawRoutine;
}

void graphic_object_set_draw(GraphicObject* graphicObject, graphic_object_draw_routine drawRoutine) {
	graphicObject->drawRoutine = drawRoutine;
}

Line* line_ctor(struct Window* window, uint32_t x, uint32_t y) {
	GraphicObject* obj = graphic_object_ctor(window, x, y, __line_draw);
	return (Line*)obj;
}

void __line_draw(Line* line, HDC hdc) {
	SelectObject(hdc, GetStockObject(WHITE_PEN));
	MoveToEx(hdc, 0, 0, NULL);
	int res = LineTo(hdc, ((GraphicObject*)line)->x, ((GraphicObject*)line)->y);
	int err = GetLastError();
}
