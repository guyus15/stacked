#ifndef UI_INTERNAL_H
#define UI_INTERNAL_H

#include "stacked/input.h"
#include "stacked/ui.h"
#include "stacked/types.h"

#include "font.h"
#include "resources.h"
#include "screen.h"

struct UiWindow;

struct UiContext
{
    bool initialised;
    bool window_resizing;
    UiIO io;
    UiStyle style;
    UiFont font;
    UiId hot_id;
    UiId active_id;
    UiInt current_z_index;
    UiResources resources;
    UiScreen screen;
    UiStack<UiWindow *> window_stack;
    UiWindow *current_window;
    UiWindow *hot_window;
    UiWindow *focused_window;
    std::vector<UiWindow *> windows;
};

extern UiContext *g_context;

static UiContext *GetContext()
{
    assert(g_context != nullptr);
    return g_context;
}

struct UiGlfwPlatformData_OpenGL;

UiGlfwPlatformData_OpenGL *GetGlfwPlatformData_OpenGL();

#endif // UI_INTERNAL_H