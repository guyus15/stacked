#include "stacked/ui.h"

#include "ui_internal.h"
#include "rect.h"
#include "text.h"

#include <glm/ext/matrix_clip_space.hpp>

#include <algorithm>
#include <ctime>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <stack>

#define min(a, b) ((a) < (b)) ? (a) : (b)
#define clamp(l, u, c) ((c) > (u)) ? (u) : ((c) < (l)) ? (l) \
                                                       : (c)

struct UiWindow
{
    UiId id;
    UiVec2I position;
    UiVec2I previous_position;
    UiVec2I size;
    UiVec2I mouse_offset;
    UiInt z_index;
    bool resizing;
    bool is_hovered;
    std::unique_ptr<Rect> window_rect;
    std::vector<std::unique_ptr<Rect>> widget_rects;
    std::vector<std::unique_ptr<UiText>> widget_texts;
};

UiContext *g_context = nullptr;

static UiId GetId(const std::string &name)
{
    return std::hash<std::string>{}(name);
}

static UiWindow *FindWindowById(const UiId id)
{
    UiContext *context = GetContext();

    for (UiWindow *window : context->windows)
    {
        if (window->id == id)
            return window;
    }

    return nullptr;
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
    window->z_index = context->current_z_index + 1;
    context->current_z_index = window->z_index;

    context->windows.push_back(window);

    return window;
}

static void GetWidgetInteraction(const Rect &rect, bool *hovered, bool *pressed,
                                 bool *held, const UiVec2I offset = {},
                                 const UiVec2I padding = {})
{
    if (rect.IsHovered(offset, padding))
    {
        UiIO &io = Ui::GetIO();

        if (hovered)
            *hovered = true;

        if (io.input.GetMouse(UiMouseButton::LeftMouse))
            if (held)
                *held = true;

        if (io.input.GetMouseDown(UiMouseButton::LeftMouse))
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

// TODO: Constrain window position to screen dimensions.
static void MoveWindow(UiWindow *window, UiVec2I target)
{
    UiContext *context = GetContext();

    window->resizing = true; // Maybe change to window->moving?
    window->position = target;
}

// TODO: Constrain window size to screen dimensions and contents of the window.
static void ResizeWindow(UiWindow *window, UiVec2I target)
{
    UiContext *context = GetContext();

    window->resizing = true;
    window->size = target;
}

static void HandleWindowResizing(UiWindow *window, const std::string &name, const Shader &shader)
{
    UiContext *context = GetContext();

    UiStyle style = context->style;

    int border_width = style.window_border_width;

    std::unique_ptr<Rect> border_left = std::make_unique<Rect>(
        UiVec2I{window->position.x, window->position.y + border_width},
        UiVec2I{border_width, window->size.h - border_width * 2});
    std::unique_ptr<Rect> border_right = std::make_unique<Rect>(
        UiVec2I{window->position.x + window->size.w - border_width, window->position.y + border_width},
        UiVec2I{border_width, window->size.h - border_width * 2});
    std::unique_ptr<Rect> border_top = std::make_unique<Rect>(
        UiVec2I{window->position.x + border_width, window->position.y + window->size.h - border_width},
        UiVec2I{window->size.w - border_width * 2, border_width});
    std::unique_ptr<Rect> border_bottom = std::make_unique<Rect>(
        UiVec2I{window->position.x + border_width, window->position.y},
        UiVec2I{window->size.w - border_width * 2, border_width});
    std::unique_ptr<Rect> border_top_left = std::make_unique<Rect>(
        UiVec2I{window->position.x, window->position.y + window->size.h - border_width},
        UiVec2I{border_width, border_width});
    std::unique_ptr<Rect> border_top_right = std::make_unique<Rect>(
        UiVec2I{window->position.x + window->size.w - border_width, window->position.y + window->size.h - border_width},
        UiVec2I{border_width, border_width});
    std::unique_ptr<Rect> border_bottom_left = std::make_unique<Rect>(
        window->position,
        UiVec2I{border_width, border_width});
    std::unique_ptr<Rect> border_bottom_right = std::make_unique<Rect>(
        UiVec2I{window->position.x + window->size.w - border_width, window->position.y},
        UiVec2I{border_width, border_width});

    border_left->SetColour(style.window_border_colour);
    border_right->SetColour(style.window_border_colour);
    border_top->SetColour(style.window_border_colour);
    border_bottom->SetColour(style.window_border_colour);
    border_top_left->SetColour(style.window_border_colour);
    border_top_right->SetColour(style.window_border_colour);
    border_bottom_left->SetColour(style.window_border_colour);
    border_bottom_right->SetColour(style.window_border_colour);

    bool bl_hover = false, bl_press = false;
    bool br_hover = false, br_press = false;
    bool bt_hover = false, bt_press = false;
    bool bb_hover = false, bb_press = false;
    bool btl_hover = false, btl_press = false;
    bool btr_hover = false, btr_press = false;
    bool bbl_hover = false, bbl_press = false;
    bool bbr_hover = false, bbr_press = false;

    if (context->hot_window == window)
    {
        GetWidgetInteraction(*border_left, name + "###border_left", &bl_hover, &bl_press, nullptr);
        GetWidgetInteraction(*border_right, name + "###border_right", &br_hover, &br_press, nullptr);
        GetWidgetInteraction(*border_top, name + "###border_top", &bt_hover, &bt_press, nullptr);
        GetWidgetInteraction(*border_bottom, name + "###border_bottom", &bb_hover, &bb_press, nullptr);
        GetWidgetInteraction(*border_top_left, name + "###border_top_left", &btl_hover, &btl_press, nullptr);
        GetWidgetInteraction(*border_top_right, name + "###border_top_right", &btr_hover, &btr_press, nullptr);
        GetWidgetInteraction(*border_bottom_left, name + "###border_bottom_left", &bbl_hover, &bbl_press, nullptr);
        GetWidgetInteraction(*border_bottom_right, name + "###border_bottom_right", &bbr_hover, &bbr_press, nullptr);
    }

    bool border_pressed = bl_press | br_press | bt_press | bb_press |
                          btl_press | btr_press | bbl_press | bbr_press;

    UiIO &io = Ui::GetIO();

    UiVec2I mouse_pos = io.input.GetMousePosition();
    mouse_pos.y = io.display_size.y - mouse_pos.y;

    if (border_pressed)
    {
        context->window_resizing = true;
        context->current_z_index += 1;
        window->z_index = context->current_z_index;
        window->mouse_offset = mouse_pos - window->position;
    }

    // FIXME: Reduce duplicate code below.

    if (context->active_id == GetId(name + "###border_left"))
    {
        int pos_x = mouse_pos.x - window->mouse_offset.x;
        MoveWindow(window, {pos_x, window->position.y});

        int size_w = window->size.w - (window->position - window->previous_position).x;
        ResizeWindow(window, {size_w, window->size.h});
    }

    if (context->active_id == GetId(name + "###border_right"))
    {
        int size_w = window->size.w + (mouse_pos.x - window->position.x) - window->mouse_offset.x;
        ResizeWindow(window, {size_w, window->size.h});

        window->mouse_offset = mouse_pos - window->position;
    }
    if (context->active_id == GetId(name + "###border_top"))
    {
        int size_h = window->size.h + (mouse_pos.y - window->position.y) - window->mouse_offset.y;
        ResizeWindow(window, {window->size.w, size_h});

        window->mouse_offset = mouse_pos - window->position;
    }

    if (context->active_id == GetId(name + "###border_bottom"))
    {
        int pos_y = mouse_pos.y - window->mouse_offset.y;
        MoveWindow(window, {window->position.x, pos_y});

        int size_h = window->size.h - (window->position - window->previous_position).y;
        ResizeWindow(window, {window->size.w, size_h});
    }

    if (context->active_id == GetId(name + "###border_top_left"))
    {
        int pos_x = mouse_pos.x - window->mouse_offset.x;
        MoveWindow(window, {pos_x, window->position.y});

        int size_w = window->size.w - (window->position - window->previous_position).x;
        int size_h = window->size.h + (mouse_pos.y - window->position.y) - window->mouse_offset.y;
        ResizeWindow(window, {size_w, size_h});

        window->mouse_offset = mouse_pos - window->position;
    }

    if (context->active_id == GetId(name + "###border_top_right"))
    {
        int size_w = window->size.w + (mouse_pos.x - window->position.x) - window->mouse_offset.x;
        int size_h = window->size.h + (mouse_pos.y - window->position.y) - window->mouse_offset.y;

        ResizeWindow(window, {size_w, size_h});

        window->mouse_offset = mouse_pos - window->position;
    }

    if (context->active_id == GetId(name + "###border_bottom_left"))
    {
        int pos_x = mouse_pos.x - window->mouse_offset.x;
        MoveWindow(window, {pos_x, window->position.y});

        int size_w = window->size.w - (window->position - window->previous_position).x;
        ResizeWindow(window, {size_w, window->size.h});

        int pos_y = mouse_pos.y - window->mouse_offset.y;
        MoveWindow(window, {window->position.x, pos_y});

        int size_h = window->size.h - (window->position - window->previous_position).y;
        ResizeWindow(window, {window->size.w, size_h});
    }

    if (context->active_id == GetId(name + "###border_bottom_right"))
    {
        int size_w = window->size.w + (mouse_pos.x - window->position.x) - window->mouse_offset.x;
        ResizeWindow(window, {size_w, window->size.y});

        int pos_y = mouse_pos.y - window->mouse_offset.y;
        MoveWindow(window, {window->position.x, pos_y});

        int size_h = window->size.h - (window->position - window->previous_position).y;
        ResizeWindow(window, {window->size.w, size_h});

        window->mouse_offset = mouse_pos - window->position;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (context->active_id == GetId(name + "###border_left") || (bl_hover && !context->window_resizing))
        window->widget_rects.push_back(std::move(border_left));
    if (context->active_id == GetId(name + "###border_right") || (br_hover && !context->window_resizing))
        window->widget_rects.push_back(std::move(border_right));
    if (context->active_id == GetId(name + "###border_top") || (bt_hover && !context->window_resizing))
        window->widget_rects.push_back(std::move(border_top));
    if (context->active_id == GetId(name + "###border_bottom") || (bb_hover && !context->window_resizing))
        window->widget_rects.push_back(std::move(border_bottom));
    if (context->active_id == GetId(name + "###border_top_left") || (btl_hover && !context->window_resizing))
        window->widget_rects.push_back(std::move(border_top_left));
    if (context->active_id == GetId(name + "###border_top_right") || (btr_hover && !context->window_resizing))
        window->widget_rects.push_back(std::move(border_top_right));
    if (context->active_id == GetId(name + "###border_bottom_left") || (bbl_hover && !context->window_resizing))
        window->widget_rects.push_back(std::move(border_bottom_left));
    if (context->active_id == GetId(name + "###border_bottom_right") || (bbr_hover && !context->window_resizing))
        window->widget_rects.push_back(std::move(border_bottom_right));

    glDisable(GL_BLEND);
}

static void AddWindowContent(UiWindow *window, UiVec2I content_position, UiVec2I content_size)
{
}

UiStyle::UiStyle()
{
    window_colour_background = {0.2f, 0.2f, 0.2f, 1.0f};

    window_border_colour = {1.0f, 1.0f, 1.0f, 0.2f};
    window_border_width = 10;
    window_border_radius = 5.0f;

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
    g_context->current_z_index = 0;
    g_context->window_resizing = false;

    g_context->resources.LoadShader("font", "shaders/default.vs", "shaders/font.fs");
    g_context->resources.LoadShader("rect", "shaders/default.vs", "shaders/rect.fs");

    Shader &font_shader = g_context->resources.GetShader("font");
    font_shader.Use();

    Shader &rect_shader = g_context->resources.GetShader("rect");
    rect_shader.Use();

    srand(time(nullptr));
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
    context->hot_window = nullptr;

    UiWindow *hot_window = context->hot_window;

    for (UiWindow *window : context->windows)
    {
        if (window->is_hovered)
        {
            if (!hot_window || hot_window->z_index < window->z_index)
                context->hot_window = window;
        }
    }

    UiIO &io = GetIO();

    UiVec2I mouse_pos = io.input.GetMousePosition();
    mouse_pos.y = io.display_size.y - mouse_pos.y;

    if (context->hot_window && context->active_id == 0)
    {
        UiWindow *window = context->hot_window;

        if (io.input.GetMouseDown(UiMouseButton::LeftMouse))
        {
            context->current_window = window;
            context->current_z_index += 1;
            window->z_index = context->current_z_index;
            window->mouse_offset = mouse_pos - window->position;
        }
    }

    if (context->current_window)
    {
        UiWindow *window = context->current_window;

        if (io.input.GetMouse(UiMouseButton::LeftMouse))
            window->position = mouse_pos - window->mouse_offset;

        context->focused_window = window;
    }

    if (io.input.GetMouseUp(UiMouseButton::LeftMouse))
    {
        context->active_id = 0;
        context->current_window = nullptr;
        context->window_resizing = false;
    }

    std::sort(context->windows.begin(), context->windows.end(),
              [](UiWindow *win1, UiWindow *win2)
              {
                  return win1->z_index < win2->z_index;
              });

    Shader &rect_shader = context->resources.GetShader("rect");
    Shader &font_shader = context->resources.GetShader("font");

    for (UiWindow *window : context->windows)
    {
        window->window_rect->Render(rect_shader);

        for (const auto &widget_rect : window->widget_rects)
            widget_rect->Render(rect_shader);

        window->widget_rects.clear();

        for (const auto &widget_text : window->widget_texts)
            widget_text->Render(context->font, font_shader);

        window->widget_texts.clear();
    }

    io.input.Update();
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

    const UiStyle &style = context->style;

    context->window_stack.Push(window);

    window->window_rect = std::make_unique<Rect>(window->position, window->size);
    window->window_rect->SetColour(style.window_colour_background);

    window->is_hovered = window->window_rect->IsHovered();
    window->resizing = false;

    Shader &shader = context->resources.GetShader("rect");
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

    UiWindow *current_window = context->window_stack.Top();
    position += current_window->position;

    const UiStyle &style = context->style;

    std::unique_ptr<Rect> button_rect = std::make_unique<Rect>(position, size);
    button_rect->SetRadius(style.button_radius);
    button_rect->SetColour(style.button_colour_normal);

    bool hovered = false, pressed = false, held = false;

    if (context->hot_window == current_window)
        GetWidgetInteraction(*button_rect, name, &hovered, &pressed, &held);

    if (hovered)
        button_rect->SetColour(style.button_colour_highlight);

    if (held)
        button_rect->SetColour(style.button_colour_press);

    UiFont &font = context->font;
    int font_size = min(size.w / name.size() * 2, size.h);
    int padding = style.button_font_padding;
    font_size = ((float)font_size / 100.0f) * (100.0f - padding);
    int font_length = font_size * name.size() / 2;
    float font_divisors = size.h / static_cast<float>(font_size);

    std::unique_ptr<UiText> button_text = std::make_unique<UiText>(
        name,
        font_size,
        UiVec2I{position.x + size.w / 2 - font_length / 2,
                position.y + static_cast<UiInt>(static_cast<float>(font_size) * (font_divisors / 2) - (static_cast<float>(font_size) / 2) + (padding * 2))},
        style.button_font_colour);

    current_window->widget_rects.push_back(std::move(button_rect));
    current_window->widget_texts.push_back(std::move(button_text));

    return pressed;
}

void Ui::Checkbox(const std::string &name, bool &enabled, UiVec2I position)
{
    UiContext *context = GetContext();

    UiWindow *current_window = context->window_stack.Top();
    position += current_window->position;

    const UiStyle &style = context->style;

    std::unique_ptr<Rect> checkbox_rect = std::make_unique<Rect>(position, UiVec2I{50, 50});
    checkbox_rect->SetRadius(style.checkbox_radius);

    bool pressed = false;

    if (context->hot_window == current_window)
        GetWidgetInteraction(*checkbox_rect, name, nullptr, &pressed, nullptr);

    if (pressed)
        enabled = !enabled;

    if (enabled)
        checkbox_rect->SetColour(style.checkbox_colour_enabled);
    else
        checkbox_rect->SetColour(style.checkbox_colour_normal);

    std::unique_ptr<UiText> checkbox_text = std::make_unique<UiText>(
        name,
        28,
        UiVec2I{position.x + 50, position.y},
        UiVec4F{1.0f, 1.0f, 1.0f, 1.0f});

    current_window->widget_rects.push_back(std::move(checkbox_rect));
    current_window->widget_texts.push_back(std::move(checkbox_text));
}

void Ui::SliderFloat(const std::string &name, float &current_val, float min_val, float max_val, UiVec2I position)
{
    UiContext *context = GetContext();

    UiWindow *current_window = context->window_stack.Top();
    position += current_window->position;

    const UiStyle &style = context->style;

    std::unique_ptr<Rect> background_rect = std::make_unique<Rect>(position, UiVec2I{200, 20});
    background_rect->SetColour(style.slider_colour_background);

    current_val = clamp(min_val, max_val, current_val);

    const float range = max_val - min_val;

    if (context->hot_window == current_window)
        GetWidgetInteraction(*background_rect, name, nullptr, nullptr, nullptr);

    if (context->active_id == GetId(name))
    {
        UiIO &io = Ui::GetIO();

        UiVec2I mouse_pos = io.input.GetMousePosition();
        mouse_pos.y = 600 - mouse_pos.y;

        int new_pos_x = clamp(position.x, position.x + 200 - 20, mouse_pos.x - 10);

        const float ui_range = 200 - 20;
        const float percent = (new_pos_x - position.x) / ui_range;

        current_val = (percent * range) + min_val;
    }

    const float new_percent = (current_val - min_val) / range;

    std::unique_ptr<Rect> handle_rect = std::make_unique<Rect>(UiVec2I{position.x + static_cast<int>(new_percent * (200.0f - 20.0f)), position.y}, UiVec2I{20, 20});
    handle_rect->SetColour(style.slider_colour_handle);

    std::unique_ptr<UiText> text = std::make_unique<UiText>(
        name,
        28,
        UiVec2I{position.x + 50, position.y},
        UiVec4F{1.0f, 1.0f, 1.0f, 1.0f});

    current_window->widget_rects.push_back(std::move(background_rect));
    current_window->widget_rects.push_back(std::move(handle_rect));
    current_window->widget_texts.push_back(std::move(text));
}

void Ui::TextBox(const std::string &name, std::string &text, UiVec2I size, UiVec2I position)
{
    UiContext *context = GetContext();

    UiWindow *current_window = context->window_stack.Top();
    position += current_window->position;

    const UiStyle &style = context->style;

    std::unique_ptr<Rect> background_rect = std::make_unique<Rect>(position, size);
    background_rect->SetColour({0.15f, 0.15f, 0.15f, 1.0f});

    Shader &rect_shader = context->resources.GetShader("rect");

    current_window->widget_rects.push_back(std::move(background_rect));
}

UiStyle &Ui::GetStyle()
{
    return g_context->style;
}

UiIO &Ui::GetIO()
{
    return g_context->io;
}

UiGlfwPlatformData_OpenGL *GetGlfwPlatformData_OpenGL()
{
    return (UiGlfwPlatformData_OpenGL *)Ui::GetIO().platform_data;
}