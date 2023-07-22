/*
	GraphicsRenderer Internal
	By Mantissa 15.04.2023
*/
#include "window.h"
#include "GraphicObjectPrivate.h"

typedef struct _GraphicsRenderer{
	HANDLE drawThread;
	HDC backDC;
	HDC clientDC;
	HBRUSH hbrBg;
	BOOL isDrawing;
	BOOL clientChange;
	UINT objectCount;
	struct Window* window;
	GraphicObject* objList[OBJECT_COUNT];
} GraphicsRenderer;

// Private methods 
void __GraphicsRenderer_drawFrame(GraphicsRenderer* graphicsRenderer);

GraphicsRenderer *GraphicsRenderer_create_object() {
	return((GraphicsRenderer*)malloc(sizeof(GraphicsRenderer)));
}

// Constructor 
void GraphicsRenderer_ctor(GraphicsRenderer* graphicsRenderer, struct Window* window, HBRUSH hbrBg) {
	graphicsRenderer->window = window;
	graphicsRenderer->isDrawing = FALSE;
	graphicsRenderer->clientChange = FALSE;
	graphicsRenderer->hbrBg = hbrBg;
	graphicsRenderer->objectCount = 0;
	HANDLE thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)__GraphicsRenderer_drawFrame, graphicsRenderer, CREATE_SUSPENDED, 0);
	if (thread == NULL)
		ExitProcess(1);
	graphicsRenderer->drawThread = thread;
}

// Destructor
void GraphicsRenderer_dtor(GraphicsRenderer* graphicsRenderer) {
	graphicsRenderer->isDrawing = FALSE;
	free(graphicsRenderer);
}
/*
	Main graphics render rouitne 60 fps
*/
void __GraphicsRenderer_drawFrame(GraphicsRenderer *graphicsRenderer) {
	HDC hdc = GetDC(window_getHWND(graphicsRenderer->window));
	graphicsRenderer->backDC = CreateCompatibleDC(hdc);
	RECT* clientRect = window_getClientRect(graphicsRenderer->window);
	HBITMAP backBM = CreateCompatibleBitmap(hdc, clientRect->right, clientRect->bottom);
	DeleteObject(SelectObject(graphicsRenderer->backDC, backBM));
	while (graphicsRenderer->isDrawing) {
		// Check if client Rect is change
		if (graphicsRenderer->clientChange) {
			RECT* clientRect = window_getClientRect(graphicsRenderer->window);
			graphicsRenderer->clientChange = FALSE;
		}
		HBITMAP backBM = CreateCompatibleBitmap(hdc, clientRect->right, clientRect->bottom);
		DeleteObject(SelectObject(graphicsRenderer->backDC, backBM));
		// Fill background
		DeleteObject(SelectObject(graphicsRenderer->backDC, graphicsRenderer->hbrBg));
		FillRect(graphicsRenderer->backDC, clientRect, graphicsRenderer->hbrBg);

		// Render all the graphics objects
		for (int i = 0; i < graphicsRenderer->objectCount; i++) {
			if (graphicsRenderer->objList[i] != NULL) {
				GraphicObject* obj = graphicsRenderer->objList[i];
				graphic_object_draw_routine drawRoutine = graphic_object_get_draw(obj);
				if (drawRoutine != NULL) {
					drawRoutine(obj, graphicsRenderer->backDC);
				}
			}
		}
		// Clean up
		//RedrawWindow(graphicsRenderer->window, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN | RDW_ERASE);
		InvalidateRect(window_getHWND(graphicsRenderer->window), 0, 1);
		window_update_controls(graphicsRenderer->window);
		DeleteObject(backBM);
		Sleep(16);
	}
	DeleteDC(graphicsRenderer->backDC);
	ReleaseDC(window_getHWND(graphicsRenderer->window), hdc);
}

// Start render routine 
void GraphicsRenderer_startRender(GraphicsRenderer *graphicsRenderer) {
	graphicsRenderer->isDrawing = TRUE;
	ResumeThread(graphicsRenderer->drawThread);
}


// Stop render routine
void GraphicsRenderer_stopRender(GraphicsRenderer *graphicsRenderer) {
	graphicsRenderer->isDrawing = FALSE;
}


// Seters 
void GraphicsRenderer_setBgBrush(GraphicsRenderer *graphicsRenderer, HBRUSH hbrBg) {
	graphicsRenderer->hbrBg = hbrBg;
}

void GraphicsRenderer_setClientChange(GraphicsRenderer* graphicsRenderer) {
	graphicsRenderer->clientChange = TRUE;
}

void GraphicsRenderer_setClientDC(GraphicsRenderer* graphicsRenderer, HDC clientDC) {
	graphicsRenderer->clientDC = clientDC;
}


void GraphicsRenderer_addGraphicObject(GraphicsRenderer* graphicsRenderer, GraphicObject* graphicObject) {
	graphicsRenderer->objList[graphicsRenderer->objectCount++] = graphicObject;
}

// Geters
HDC GraphicsRenderer_getBackDC(GraphicsRenderer* graphicsRenderer) {
	return graphicsRenderer->backDC;
}
