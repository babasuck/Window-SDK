#pragma once
#include "GraphicObject.h"

struct GraphicsRenderer;
struct Window;

struct GraphicsRenderer* GraphicsRenderer_create_object();
// Constructor 
void GraphicsRenderer_ctor(struct GraphicsRenderer* graphicsRenderer, struct Window* window, HBRUSH hbrBg);
// Destructor
void GraphicsRenderer_dtor(struct GraphicsRenderer* graphicsRenderer);
// Start rendering routine
void GraphicsRenderer_startRender(struct GraphicsRenderer* graphicsRenderer);
// Stop rendering routine
void GraphicsRenderer_stopRender(struct GraphicsRenderer* graphicsRenderer);


// Seters 
void GraphicsRenderer_setClientChange(struct GraphicsRenderer* graphicsRenderer);
void GraphicsRenderer_setBgBrush(struct GraphicsRenderer* graphicsRenderer, HBRUSH hbrBg);
void GraphicsRenderer_setClientDC(struct GraphicsRenderer* graphicsRenderer, HDC clientDC);
void GraphicsRenderer_addGraphicObject(struct GraphicsRenderer* graphicsRenderer, struct GraphicObject* graphicObject);

HDC GraphicsRenderer_getBackDC(struct GraphicsRenderer* graphicsRenderer);