#pragma once
#include <Windows.h>
#include <stdint.h>

// Objects in object list
#define OBJECT_COUNT 255

// Base graphic Object 
struct GraphicObject;

typedef (*graphic_object_draw_routine)(struct GraphicObject* graphicObject, HDC hdc);

graphic_object_draw_routine* graphic_object_get_draw(struct GraphicObject* graphicObject);
void graphic_object_set_draw(struct GraphicObject* graphicObject, graphic_object_draw_routine drawRoutine);
struct GraphicObject* graphic_object_ctor(struct Window* window, uint32_t x, uint32_t y, graphic_object_draw_routine drawRoutine);

// Line 

struct Line;
struct Line* line_ctor(struct Window* window, uint32_t x, uint32_t y);