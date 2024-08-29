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
    UiVec2I previous_position;
    UiVec2I size;
    UiVec2I mouse_offset;
    bool resizing;
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

static UiId GetId(const std::string &name)
{
    return std::hash<std::string>{}(name);
}

static UiWindow *FindWindowById(const UiId id)
{
    UiContext *context = GetContext();
    return static_cast<UiWindow *>(context->windows.GetVoidPtr(id));
}

static UiWindow *FindWindowByName(const std::string &name)
{
    const UiId id = GetId(name);
    return FindWindowById(id);
}

static UiWindow *CreateNewWindow(const std::string &name)
{
    UiContext *context = GetContext();

    UiId id = GetId(name);
    UiWindow *window = new UiWindow{id};

    context->windows.SetVoidPtr(id, window);

    return window;
}

static void GetWidgetInteraction(const Rect &rect, bool *hovered, bool *pressed,
                                 bool *held, const UiVec2I offset = {},
                                 const UiVec2I padding = {})
{
    if (rect.IsHovered(offset, padding))
    {
        if (hovered)
            *hovered = true;

        if (Input::GetMouse(MouseButton::LeftMouse))
            if (held)
                *held = true;

        if (Input::GetMouseDown(MouseButton::LeftMouse))
        {
            if (pressed)
                *pressed = true;
        }
    }
}

static void GetWidgetInteraction(const Rect &rect, const std::string &name, bool *hovered, bool *pressed, bool *held)
{
    UiContext *context = GetContext();

    UiId id = GetId(name);

    bool local_hovered = false, local_pressed = false;
    GetWidgetInteraction(rect, &local_hovered, &local_pressed, held);

    if (local_hovered)
        context->hot_id = id;

    if (local_pressed)
        context->active_id = id;

    if (hovered)
        *hovered = local_hovered;

    if (pressed)
        *pressed = local_pressed;
}

static void HandleWindowResizing(UiWindow *window, const std::string &name, const Shader &shader)
{
    UiContext *context = GetContext();

    Rect border_left{{window->position.x, window->position.y + 5}, {5, window->size.h - 10}};
    Rect border_right{{window->position.x + window->size.w - 5, window->position.y + 5}, {5, window->size.h - 10}};
    Rect border_top{{window->position.x + 5, window->position.y + window->size.h - 5}, {window->size.w - 10, 5}};
    Rect border_bottom{{window->position.x + 5, window->position.y}, {window->size.w - 10, 5}};

    Rect border_top_left{{window->position.x, window->position.y + window->size.h - 5}, {5, 5}};
    border_top_left.SetColour({0.0f, 1.0f, 0.0f, 1.0f});
    Rect border_top_right{{window->position.x + window->size.w - 5, window->position.y + window->size.h - 5}, {5, 5}};
    border_top_right.SetColour({0.0f, 1.0f, 0.0f, 1.0f});
    Rect border_bottom_left{window->position, {5, 5}};
    border_bottom_left.SetColour({0.0f, 1.0f, 0.0f, 1.0f});
    Rect border_bottom_right{{window->position.x + window->size.w - 5, window->position.y}, {5, 5}};
    border_bottom_right.SetColour({0.0f, 1.0f, 0.0f, 1.0f});

    bool bl_hover = false, bl_press = false;
    bool br_hover = false, br_press = false;
    bool bt_hover = false, bt_press = false;
    bool bb_hover = false, bb_press = false;
    bool btl_hover = false, btl_press = false;
    bool btr_hover = false, btr_press = false;
    bool bbl_hover = false, bbl_press = false;
    bool bbr_hover = false, bbr_press = false;

    GetWidgetInteraction(border_left, name + "###border_left", &bl_hover, &bl_press, nullptr);
    GetWidgetInteraction(border_right, name + "###border_right", &br_hover, &br_press, nullptr);
    GetWidgetInteraction(border_top, name + "###border_top", &bt_hover, &bt_press, nullptr);
    GetWidgetInteraction(border_bottom, name + "###border_bottom", &bb_hover, &bb_press, nullptr);
    GetWidgetInteraction(border_top_left, name + "###border_top_left", &btl_hover, &btl_press, nullptr);
    GetWidgetInteraction(border_top_right, name + "###border_top_right", &btr_hover, &btr_press, nullptr);
    GetWidgetInteraction(border_bottom_left, name + "###border_bottom_left", &bbl_hover, &bbl_press, nullptr);
    GetWidgetInteraction(border_bottom_right, name + "###border_bottom_right", &bbr_hover, &bbr_press, nullptr);

    bool border_pressed = bl_press | br_press | bt_press | bb_press |
                          btl_press | btr_press | bbl_press | bbr_press;

    UiVec2I mouse_pos = Input::GetMousePosition();
    mouse_pos.y = 600 - mouse_pos.y;

    if (border_pressed)
        window->mouse_offset = mouse_pos - window->position;

    // FIXME: Reorganise this to remove duplicate code.

    if (context->active_id == GetId(name + "###border_left"))
    {
        window->resizing = true;
        window->position.x = mouse_pos.x - window->mouse_offset.x;
        window->size.w += -(window->position - window->previous_position).x;
    }

    if (context->active_id == GetId(name + "###border_right"))
    {
        window->resizing = true;
        window->size.w += (mouse_pos.x - window->position.x) - window->mouse_offset.x;
        window->mouse_offset = mouse_pos - window->position;
    }

    if (context->active_id == GetId(name + "###border_top"))
    {
        window->resizing = true;
        window->size.h += (mouse_pos.y - window->position.y) - window->mouse_offset.y;
        window->mouse_offset = mouse_pos - window->position;
    }

    if (context->active_id == GetId(name + "###border_bottom"))
    {
        window->resizing = true;
        window->position.y = mouse_pos.y - window->mouse_offset.y;
        window->size.h += -(window->position - window->previous_position).y;
    }

    if (context->active_id == GetId(name + "###border_top_left"))
    {
        window->resizing = true;
        window->position.x = mouse_pos.x - window->mouse_offset.x;
        window->size.w += -(window->position - window->previous_position).x;
        window->size.h += (mouse_pos.y - window->position.y) - window->mouse_offset.y;
        window->mouse_offset = mouse_pos - window->position;
    }

    if (context->active_id == GetId(name + "###border_top_right"))
    {
        window->resizing = true;
        window->size.h += (mouse_pos.y - window->position.y) - window->mouse_offset.y;
        window->size.w += (mouse_pos.x - window->position.x) - window->mouse_offset.x;
        window->mouse_offset = mouse_pos - window->position;
    }

    if (context->active_id == GetId(name + "###border_bottom_left"))
    {
        window->resizing = true;
        window->position.x = mouse_pos.x - window->mouse_offset.x;
        window->size.w += -(window->position - window->previous_position).x;
        window->position.y = mouse_pos.y - window->mouse_offset.y;
        window->size.h += -(window->position - window->previous_position).y;
    }

    if (context->active_id == GetId(name + "###border_bottom_right"))
    {
        window->resizing = true;
        window->size.w += (mouse_pos.x - window->position.x) - window->mouse_offset.x;
        window->position.y = mouse_pos.y - window->mouse_offset.y;
        window->size.h += -(window->position - window->previous_position).y;
        window->mouse_offset = mouse_pos - window->position;
    }

    if (context->active_id == GetId(name + "###border_left") || (bl_hover && !window->resizing))
        border_left.Render(shader);
    if (context->active_id == GetId(name + "###border_right") || (br_hover && !window->resizing))
        border_right.Render(shader);
    if (context->active_id == GetId(name + "###border_top") || (bt_hover && !window->resizing))
        border_top.Render(shader);
    if (context->active_id == GetId(name + "###border_bottom") || (bb_hover && !window->resizing))
        border_bottom.Render(shader);
    if (context->active_id == GetId(name + "###border_top_left") || (btl_hover && !window->resizing))
        border_top_left.Render(shader);
    if (context->active_id == GetId(name + "###border_top_right") || (btr_hover && !window->resizing))
        border_top_right.Render(shader);
    if (context->active_id == GetId(name + "###border_bottom_left") || (bbl_hover && !window->resizing))
        border_bottom_left.Render(shader);
    if (context->active_id == GetId(name + "###border_bottom_right") || (bbr_hover && !window->resizing))
        border_bottom_right.Render(shader);
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
    g_context->font = UiFont{"fonts/Montserrat/Montserrat-VariableFont_wght.ttf"};
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

    UiWindow *window = context->current_window;
    if (window)
        window->resizing = false;
}

void Ui::EndFrame()
{
    UiContext *context = GetContext();

    UiWindow *window = context->current_window;

    if (window && context->hot_id == window->id && context->active_id == 0 && !window->resizing)
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
        context->hot_id = GetId(name);
        context->current_window = window;
    }

    Shader &shader = ResourceManager::GetShader("default");
    window_rect.Render(shader);

    HandleWindowResizing(window, name, shader);
}

void Ui::EndWindow()
{
    UiContext *context = GetContext();

    UiWindow *window = context->window_stack.Top();
    window->previous_position = window->position;

    context->window_stack.Pop();
}

bool Ui::Button(const std::string &name, UiVec2I size, UiVec2I position)
{
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

    if (held)
        button_rect.SetColour(style.button_colour_press);

    UiFont &font = context->font;
    int font_size = min(size.w / name.size() * 2, size.h);
    int padding = style.button_font_padding;
    font_size = ((float)font_size / 100.0f) * (100.0f - padding);
    int font_length = font_size * name.size() / 2;
    float font_divisors = size.h / static_cast<float>(font_size);

    font.Load(font_size);
    font.SetColour(style.button_font_colour);
    font.SetPosition({position.x + size.w / 2 - font_length / 2,
                      position.y + static_cast<UiInt>(static_cast<float>(font_size) * (font_divisors / 2) - (static_cast<float>(font_size) / 2) + (padding * 2))});

    Shader &shader = ResourceManager::GetShader("default");
    button_rect.Render(shader);
    font.Render(name, font_size, shader);

    return pressed;
}

void Ui::Checkbox(const std::string &name, bool &enabled, UiVec2I position)
{
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

    UiFont &font = context->font;
    font.Load(28);
    font.SetColour({1.0f, 1.0f, 1.0f, 1.0f});
    font.SetPosition({position.x + 50, position.y});

    Shader &shader = ResourceManager::GetShader("default");
    checkbox_rect.Render(shader);
    font.Render(name, 28, shader);
}

void Ui::SliderFloat(const std::string &name, float &current_val, float min_val, float max_val, UiVec2I position)
{
    UiContext *context = GetContext();

    const UiWindow *current_window = context->window_stack.Top();
    position += current_window->position;

    const UiStyle &style = context->style;

    Rect background_rect{position, {200, 20}};
    background_rect.SetColour(style.slider_colour_background);

    current_val = clamp(min_val, max_val, current_val);

    const float range = max_val - min_val;

    GetWidgetInteraction(background_rect, name, nullptr, nullptr, nullptr);

    if (context->active_id == GetId(name))
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

    UiFont &font = context->font;
    font.Load(28);
    font.SetColour({1.0f, 1.0f, 1.0f, 1.0f});
    font.SetPosition({position.x + 50, position.y});

    Shader &shader = ResourceManager::GetShader("default");
    background_rect.Render(shader);
    handle_rect.Render(shader);
    font.Render(name, 28, shader);
}

void Ui::TextBox(const std::string &name, std::string &text, UiVec2I size, UiVec2I position)
{
    UiContext *context = GetContext();

    const UiWindow *current_window = context->window_stack.Top();
    position += current_window->position;

    const UiStyle &style = context->style;

    Rect background_rect{position, size};
    background_rect.SetColour({0.15f, 0.15f, 0.15f, 1.0f});

    const Shader &shader = ResourceManager::GetShader("default");
    background_rect.Render(shader);
}

UiStyle &Ui::GetStyle()
{
    return g_context->style;
}