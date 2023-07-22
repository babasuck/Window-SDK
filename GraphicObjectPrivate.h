#pragma once
#include "GraphicObject.h"
#include <stdint.h>

typedef struct GraphicObject {
	uint32_t x;
	uint32_t y;
	graphic_object_draw_routine drawRoutine;
} GraphicObject;


typedef struct Line {
	GraphicObject graphicObject;
} Line;