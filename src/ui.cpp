#include "stacked/ui.h"
#include "stacked/input.h"
#include "stacked/types.h"

#include "font.h"
#include "rect.h"
#include "resource_manager.h"

#include <glm/ext/matrix_clip_space.hpp>

#include <iostream>
#include <unordered_map>
#include <stack>

#define min(a, b) ((a) < (b)) ? (a) : (b)
#define clamp(l, u, c) ((c) > (u)) ? (u) : ((c) < (l)) ? (l) \
                                                       : (c)

// FIXME: Standardise naming of dimensions/size. It should be one or the other, not both.

struct UiWindow
{
    UiId id;
    UiVec2I position;
    UiVec2I size;
    UiVec2I mouse_offset;
};

struct UiContext
{
    bool initialised;
    UiStyle style;
    UiFont font;
    UiStack<UiWindow *> window_stack;
    UiStorage windows;
    UiId hot_id;
    UiId active_id;
    UiWindow *current_window;
};

UiContext *g_context = nullptr;

static UiContext *GetContext()
{
    assert(g_context != nullptr);
    return g_context;
}

static UiWindow *FindWindowById(const UiId id)
{
    UiContext *context = GetContext();
    return static_cast<UiWindow *>(context->windows.GetVoidPtr(id));
}

static UiWindow *FindWindowByName(const std::string &name)
{
    const UiId id = std::hash<std::string>{}(name);
    return FindWindowById(id);
}

static UiWindow *CreateNewWindow(const std::string &name)
{
    UiContext *context = GetContext();

    UiId id = std::hash<std::string>{}(name);
    UiWindow *window = new UiWindow{id};

    context->windows.SetVoidPtr(id, window);

    return window;
}

static void GetWidgetInteraction(const Rect &rect, const std::string &name, bool *hovered, bool *pressed, bool *held)
{
    UiContext *context = GetContext();

    if (rect.IsHovered())
    {
        UiId id = std::hash<std::string>{}(name);
        context->hot_id = id;

        if (hovered)
            *hovered = true;

        if (Input::GetMouse(MouseButton::LeftMouse))
            if (held)
                *held = true;

        if (Input::GetMouseDown(MouseButton::LeftMouse))
        {
            context->active_id = id;
            if (pressed)
                *pressed = true;
        }
    }
}

UiStyle::UiStyle()
{
    window_colour_background = {0.3f, 0.3f, 0.3f, 1.0f};

    button_colour_normal = {0.0f, 0.0f, 1.0f, 1.0f};
    button_colour_highlight = {0.3f, 0.3f, 1.0f, 1.0f};
    button_colour_press = {0.0f, 0.0f, 0.7f, 1.0f};
    button_font_colour = {1.0f, 1.0f, 1.0f, 1.0f};
    button_font_padding = 5.0f;
    button_radius = 5.0f;

    checkbox_colour_normal = {1.0f, 0.0f, 0.0f, 1.0f};
    checkbox_colour_enabled = {0.0f, 1.0f, 0.0f, 1.0f};
    checkbox_radius = 5.0f;

    slider_colour_background = {0.6f, 0.6f, 0.6f, 1.0f};
    slider_colour_handle = {1.0f, 1.0f, 0.0f, 1.0f};
}

void Ui::Initialise()
{
    g_context = new UiContext();

    g_context->initialised = true;
    g_context->window_stack = {};

    ResourceManager::LoadShader("default", "shaders/shader.vs", "shaders/shader.fs");
    Shader &shader = ResourceManager::GetShader("default");
    shader.Use();

    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    shader.SetMat4("projection", projection);
}

void Ui::Dispose()
{
    delete g_context;
}

void Ui::BeginFrame()
{
    UiContext *context = GetContext();
    context->hot_id = 0;
}

void Ui::EndFrame()
{
    UiContext *context = GetContext();

    UiWindow *window = context->current_window;

    if (window && context->hot_id == window->id && context->active_id == 0)
    {
        UiVec2I mouse_pos = Input::GetMousePosition();
        mouse_pos.y = 600 - mouse_pos.y;

        if (Input::GetMouseDown(MouseButton::LeftMouse))
            window->mouse_offset = mouse_pos - window->position;
        else if (Input::GetMouse(MouseButton::LeftMouse))
            window->position = mouse_pos - window->mouse_offset;
    }

    if (Input::GetMouseUp(MouseButton::LeftMouse))
        context->active_id = 0;
}

// TODO: Pass window flags in to modify the behaviour of the window.
void Ui::BeginWindow(const std::string &name, UiVec2I size, UiVec2I position)
{
    UiWindow *window = FindWindowByName(name);

    bool new_window_created = (window == nullptr);
    if (new_window_created)
    {
        window = CreateNewWindow(name);
        window->position = position;
        window->size = size;
    }

    UiContext *context = GetContext();

    context->window_stack.Push(window);

    const UiStyle &style = context->style;

    Rect window_rect{window->position, window->size};
    window_rect.SetColour(style.window_colour_background);

    if (window_rect.IsHovered())
    {
        context->hot_id = std::hash<std::string>{}(name);
        context->current_window = window;
    }

    Shader &shader = ResourceManager::GetShader("default");
    window_rect.Render(shader);
}

void Ui::EndWindow()
{
    UiContext *context = GetContext();
    context->window_stack.Pop();
}

bool Ui::Button(const std::string &name, UiVec2I size, UiVec2I position)
{
    bool button_press = false;

    Shader &shader = ResourceManager::GetShader("default");

    UiContext *context = GetContext();

    const UiWindow *current_window = context->window_stack.Top();
    position += current_window->position;

    const UiStyle &style = context->style;

    Rect button_rect{position, size};
    button_rect.SetRadius(style.button_radius);
    button_rect.SetColour(style.button_colour_normal);

    bool hovered = false, pressed = false, held = false;
    GetWidgetInteraction(button_rect, name, &hovered, &pressed, &held);

    if (hovered)
        button_rect.SetColour(style.button_colour_highlight);

    if (pressed)
        button_press = true;

    if (held)
        button_rect.SetColour(style.button_colour_press);

    button_rect.Render(shader);

    static Font font{"fonts/Montserrat/Montserrat-VariableFont_wght.ttf"};
    font.SetColour(style.button_font_colour);

    int font_size = min(size.w / name.size() * 2, size.h);
    int padding = style.button_font_padding;
    font_size = ((float)font_size / 100.0f) * (100.0f - padding);
    int font_length = font_size * name.size() / 2;
    float font_divisors = size.h / static_cast<float>(font_size);

    font.SetPosition({position.x + size.w / 2 - font_length / 2,
                      position.y + static_cast<UiInt>(static_cast<float>(font_size) * (font_divisors / 2) - (static_cast<float>(font_size) / 2) + (padding * 2))});
    font.Load(font_size);
    font.Render(name, font_size, shader);

    return button_press;
}

void Ui::Checkbox(const std::string &name, bool &enabled, UiVec2I position)
{
    Shader &shader = ResourceManager::GetShader("default");

    UiContext *context = GetContext();

    const UiWindow *current_window = context->window_stack.Top();
    position += current_window->position;

    const UiStyle &style = context->style;

    Rect checkbox_rect{position, {50, 50}};
    checkbox_rect.SetRadius(style.checkbox_radius);

    bool pressed = false;
    GetWidgetInteraction(checkbox_rect, name, nullptr, &pressed, nullptr);

    if (pressed)
        enabled = !enabled;

    if (enabled)
        checkbox_rect.SetColour(style.checkbox_colour_enabled);
    else
        checkbox_rect.SetColour(style.checkbox_colour_normal);

    checkbox_rect.Render(shader);
}

void Ui::SliderFloat(const std::string &name, float &current_val, float min_val, float max_val, UiVec2I position)
{
    Shader &shader = ResourceManager::GetShader("default");

    UiContext *context = GetContext();

    const UiWindow *current_window = context->window_stack.Top();
    position += current_window->position;

    const UiStyle &style = context->style;

    Rect background_rect{position, {200, 20}};
    background_rect.SetColour(style.slider_colour_background);

    background_rect.Render(shader);

    current_val = clamp(min_val, max_val, current_val);

    const float range = max_val - min_val;

    GetWidgetInteraction(background_rect, name, nullptr, nullptr, nullptr);

    if (context->active_id == std::hash<std::string>{}(name))
    {
        UiVec2I mouse_pos = Input::GetMousePosition();
        mouse_pos.y = 600 - mouse_pos.y;

        int new_pos_x = clamp(position.x, position.x + 200 - 20, mouse_pos.x - 10);

        const float ui_range = 200 - 20;
        const float percent = (new_pos_x - position.x) / ui_range;

        current_val = (percent * range) + min_val;
    }

    const float new_percent = (current_val - min_val) / range;

    Rect handle_rect{{position.x + static_cast<int>(new_percent * (200.0f - 20.0f)), position.y}, {20, 20}};
    handle_rect.SetColour(style.slider_colour_handle);

    handle_rect.Render(shader);
}

UiStyle &Ui::GetStyle()
{
    return g_context->style;
}