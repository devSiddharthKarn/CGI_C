/*
 * Copyright 2025 Siddharth Karn
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/// @file linux.c
#define CGI_LINUX_IMPLEMENTATION_ACTIVE
#include "../include/cgi.h"
#include "X11/Xlib.h"
#include "X11/Xutil.h"
#include "X11/keysym.h"
#include <X11/extensions/Xrandr.h>
#include "stdlib.h"
#include "string.h"

/// @brief Converts a CGI input key to an X11 KeySym
/// @param key CGI input key
/// @return Corresponding X11 KeySym or NoSymbol if not found
KeySym CGIInputKeyToKeySym(CGIKeyCode key)
{
    switch (key)
    {
    case CGI_KEYCODE_A:
        return XK_a;
    case CGI_KEYCODE_B:
        return XK_b;
    case CGI_KEYCODE_C:
        return XK_c;
    case CGI_KEYCODE_D:
        return XK_d;
    case CGI_KEYCODE_E:
        return XK_e;
    case CGI_KEYCODE_F:
        return XK_f;
    case CGI_KEYCODE_G:
        return XK_g;
    case CGI_KEYCODE_H:
        return XK_h;
    case CGI_KEYCODE_I:
        return XK_i;
    case CGI_KEYCODE_J:
        return XK_j;
    case CGI_KEYCODE_K:
        return XK_k;
    case CGI_KEYCODE_L:
        return XK_l;
    case CGI_KEYCODE_M:
        return XK_m;
    case CGI_KEYCODE_N:
        return XK_n;
    case CGI_KEYCODE_O:
        return XK_o;
    case CGI_KEYCODE_P:
        return XK_p;
    case CGI_KEYCODE_Q:
        return XK_q;
    case CGI_KEYCODE_R:
        return XK_r;
    case CGI_KEYCODE_S:
        return XK_s;
    case CGI_KEYCODE_T:
        return XK_t;
    case CGI_KEYCODE_U:
        return XK_u;
    case CGI_KEYCODE_V:
        return XK_v;
    case CGI_KEYCODE_W:
        return XK_w;
    case CGI_KEYCODE_X:
        return XK_x;
    case CGI_KEYCODE_Y:
        return XK_y;
    case CGI_KEYCODE_Z:
        return XK_z;
    case CGI_KEYCODE_0:
        return XK_0;
    case CGI_KEYCODE_1:
        return XK_1;
    case CGI_KEYCODE_2:
        return XK_2;
    case CGI_KEYCODE_3:
        return XK_3;
    case CGI_KEYCODE_4:
        return XK_4;
    case CGI_KEYCODE_5:
        return XK_5;
    case CGI_KEYCODE_6:
        return XK_6;
    case CGI_KEYCODE_7:
        return XK_7;
    case CGI_KEYCODE_8:
        return XK_8;
    case CGI_KEYCODE_9:
        return XK_9;
    case CGI_KEYCODE_F1:
        return XK_F1;
    case CGI_KEYCODE_F2:
        return XK_F2;
    case CGI_KEYCODE_F3:
        return XK_F3;
    case CGI_KEYCODE_F4:
        return XK_F4;
    case CGI_KEYCODE_F5:
        return XK_F5;
    case CGI_KEYCODE_F6:
        return XK_F6;
    case CGI_KEYCODE_F7:
        return XK_F7;
    case CGI_KEYCODE_F8:
        return XK_F8;
    case CGI_KEYCODE_F9:
        return XK_F9;
    case CGI_KEYCODE_F10:
        return XK_F10;
    case CGI_KEYCODE_F11:
        return XK_F11;
    case CGI_KEYCODE_F12:
        return XK_F12;
    case CGI_KEYCODE_UP:
        return XK_Up;
    case CGI_KEYCODE_DOWN:
        return XK_Down;
    case CGI_KEYCODE_LEFT:
        return XK_Left;
    case CGI_KEYCODE_RIGHT:
        return XK_Right;
    case CGI_KEYCODE_ENTER:
        return XK_Return;
    case CGI_KEYCODE_ESCAPE:
        return XK_Escape;
    case CGI_KEYCODE_SPACE:
        return XK_space;
    case CGI_KEYCODE_BACKSPACE:
        return XK_BackSpace;
    case CGI_KEYCODE_SHIFT:
        return XK_Shift_L;
    case CGI_KEYCODE_CTRL:
        return XK_Control_L;
    case CGI_KEYCODE_ALT:
        return XK_Alt_L;

    default:
        return NoSymbol;
    }

    return NoSymbol;
}

CGIKeyCode KeyboardKeyCodes[] = {
    CGI_KEYCODE_A,
    CGI_KEYCODE_B,
    CGI_KEYCODE_C,
    CGI_KEYCODE_D,
    CGI_KEYCODE_E,
    CGI_KEYCODE_F,
    CGI_KEYCODE_G,
    CGI_KEYCODE_H,
    CGI_KEYCODE_I,
    CGI_KEYCODE_J,
    CGI_KEYCODE_K,
    CGI_KEYCODE_L,
    CGI_KEYCODE_M,
    CGI_KEYCODE_N,
    CGI_KEYCODE_O,
    CGI_KEYCODE_P,
    CGI_KEYCODE_Q,
    CGI_KEYCODE_R,
    CGI_KEYCODE_S,
    CGI_KEYCODE_T,
    CGI_KEYCODE_U,
    CGI_KEYCODE_V,
    CGI_KEYCODE_W,
    CGI_KEYCODE_X,
    CGI_KEYCODE_Y,
    CGI_KEYCODE_Z,
    CGI_KEYCODE_0,
    CGI_KEYCODE_1,
    CGI_KEYCODE_2,
    CGI_KEYCODE_3,
    CGI_KEYCODE_4,
    CGI_KEYCODE_5,
    CGI_KEYCODE_6,
    CGI_KEYCODE_7,
    CGI_KEYCODE_8,
    CGI_KEYCODE_9,
    CGI_KEYCODE_ESCAPE,
    CGI_KEYCODE_ENTER,
    CGI_KEYCODE_SPACE,
    CGI_KEYCODE_UP,
    CGI_KEYCODE_DOWN,
    CGI_KEYCODE_LEFT,
    CGI_KEYCODE_RIGHT,
    CGI_KEYCODE_BACKSPACE,
    CGI_KEYCODE_SHIFT,
    CGI_KEYCODE_CTRL,
    CGI_KEYCODE_ALT,
    CGI_KEYCODE_F1,
    CGI_KEYCODE_F2,
    CGI_KEYCODE_F3,
    CGI_KEYCODE_F4,
    CGI_KEYCODE_F5,
    CGI_KEYCODE_F6,
    CGI_KEYCODE_F7,
    CGI_KEYCODE_F8,
    CGI_KEYCODE_F9,
    CGI_KEYCODE_F10,
    CGI_KEYCODE_F11,
    CGI_KEYCODE_F12,

};

CGIKeyCode CursorKeyCodes[] = {
    CGI_KEYCODE_MOUSE_L,
    CGI_KEYCODE_MOUSE_R};

// void ScanCurrentWindowEvent

/// @brief Internal structure representing a framebuffer for a CGI window

struct CGIFrameBuffer
{
    unsigned int *pixels;
    unsigned int width;
    unsigned int height;
    XImage *image;
};

/// @brief Internal structure representing the state of a CGI window on Linux
struct WindowState
{
    char *classname;
    Display *display;
    int screen;
    Window window;
    Colormap colormap;
    XColor base_color;
    XEvent event;
    GC gc;
    // Atom wm_delete_atom;
};

/// @brief Internal structure representing a CGI window on Linux
struct CGIWindow
{
    char *name;
    struct WindowState windowState;
    // CGIPoint cursor;
    CGIColor_t CGIbase_color;
    CGIPoint position;
    unsigned int width;
    unsigned int height;
    struct CGIFrameBuffer buffer;
    unsigned int buffer_width;
    unsigned int buffer_height;
    CGIEventImage event;
    CGIBool open;
};

/// @brief Sets up scroll events in the CGI window
/// @param window Pointer to the CGI window
/// @param e Pointer to the XEvent
void setup_scroll_in_window(CGIWindow *window, XEvent *e)
{
    if (e->type == ButtonPress)
    {
        switch (e->xbutton.button)
        {
        case 4:
        {
            window->event.windowEvent.scroll_delta_y = 1;
            window->event.windowEvent.is_scrolled_y = CGI_true;
            break;
        }
        case 5:
        {
            window->event.windowEvent.scroll_delta_y = -1;
            window->event.windowEvent.is_scrolled_y = CGI_true;
            break;
        }
        case 6:
        {
            window->event.windowEvent.scroll_delta_x = -1;
            window->event.windowEvent.is_scrolled_x = CGI_true;
            break;
        }
        case 7:
        {
            window->event.windowEvent.scroll_delta_x = 1;
            window->event.windowEvent.is_scrolled_x = CGI_true;
            break;
        }
        default:
        {

            break;
        }
        }
    }

    return;
}

/// @brief Creates a CGIColor_t structure with the specified RGB values
/// @param r Red component (0-255)
/// @param g Green component (0-255)
/// @param b Blue component (0-255)
/// @return CGIColor_t structure with the specified color
CGIColor_t CGIMakeColor(unsigned char r, unsigned char g, unsigned char b)
{
    CGIColor_t color;
    color.r = r;
    color.g = g;
    color.b = b;
    return color;
}

/// @brief Initializes the CPU buffer for a CGI window
/// @param window Pointer to the CGI window
/// @return CGI_true on success, CGI_false on failure
CGIBool initialize_cpu_buffer(CGIWindow *window)
{
    if (!window || !window->windowState.display)
        return CGI_false;

    window->buffer.width = window->width;
    window->buffer.height = window->height;
    window->buffer.pixels = (unsigned int *)malloc(window->width * window->height * sizeof(unsigned int));

    if (!window->buffer.pixels)
        return CGI_false;

    window->buffer.image = XCreateImage(window->windowState.display, DefaultVisual(window->windowState.display, window->windowState.screen), DefaultDepth(window->windowState.display, window->windowState.screen), ZPixmap, 0, (char *)window->buffer.pixels, window->buffer.width, window->buffer.height, 32, 0);

    if (!window->buffer.image)
    {
        free(window->buffer.pixels);
        window->buffer.pixels = NULL;
        return CGI_false;
    }

    return CGI_true;
}

/// @brief          Destroys the framebuffer of a CGI window
/// @param window Pointer to the CGI window
/// @return void
void DestroyFrameBuffer(CGIWindow *window)
{
    if (!window)
        return;

    if (window->buffer.image)
    {
        window->buffer.image->data = NULL;
        XDestroyImage(window->buffer.image);
        window->buffer.image = NULL;
    }

    if (window->buffer.pixels)
    {
        free(window->buffer.pixels);
        window->buffer.pixels = NULL;
    }
}

/// @brief Sets a pixel in the CGI window's buffer to the specified color
/// @param window Pointer to the CGI window
/// @param x_pos X position of the pixel
/// @param y_pos Y position of the pixel
/// @param color Color to set the pixel to
void CGISetPixel(CGIWindow *window, int x_pos, int y_pos, CGIColor_t color)
{
    if (!window)
        return;

    if (x_pos < 0 || x_pos >= window->buffer.width || y_pos < 0 || y_pos >= window->buffer.height)
        return;

    unsigned int pixelColor = 0xFF000000 | (color.r << 16) | (color.g << 8) | (color.b);

    window->buffer.pixels[y_pos * window->buffer.width + x_pos] = pixelColor;
}

/// @brief Clears the CGI window's buffer with the specified color
/// @param window Pointer to the CGI window
/// @param color Color to clear the buffer with
/// @return CGI_true on success, CGI_false on failure
CGIBool CGIClearBuffer(CGIWindow *window, CGIColor_t color)
{

    if (!window)
        return CGI_false;
    unsigned int pixel = 0xFF000000 | (color.r << 16) | (color.g << 8) | (color.b);

    unsigned int count = window->buffer.height * window->buffer.width;

    for (int i = 0; i < count; i++)
    {
        window->buffer.pixels[i] = pixel;
    }

    return CGI_true;
}

void CGIClearBufferRegion(CGIWindow *window, int x, int y, int width, int height, CGIColor_t color)
{
    if (!window || !window->buffer.image)
        return;

    int bw = window->buffer.width;
    int bh = window->buffer.height;

    // Completely outside → return
    if (x >= bw || y >= bh || x + width <= 0 || y + height <= 0)
        return;

    // ---- CLIP THE REGION (very important) ----

    int clippedX = x < 0 ? 0 : x;
    int clippedY = y < 0 ? 0 : y;

    int clippedW = (x + width > bw) ? (bw - clippedX) : (width - (clippedX - x));
    int clippedH = (y + height > bh) ? (bh - clippedY) : (height - (clippedY - y));

    unsigned int pixel = 0xFF000000 | (color.r << 16) | (color.g << 8) | (color.b);

    for (int i = clippedY; i < clippedY + clippedH; i++)
    {
        for (int j = clippedX; j < clippedX + clippedW; j++)
        {
            window->buffer.pixels[i * window->buffer.width + j] = pixel;
        }
    }
}

/// @brief Refreshes the CGI window's buffer and updates the display
/// @param window Pointer to the CGI window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGIRefreshBuffer(CGIWindow *window)
{
    if (!window || !window->buffer.pixels || !window->buffer.image)
        return CGI_false;

    XPutImage(window->windowState.display, window->windowState.window, window->windowState.gc, window->buffer.image, 0, 0, 0, 0, window->buffer.width, window->buffer.height);

    XFlush(window->windowState.display);
    return CGI_true;
}

void CGIRefreshBufferRegion(CGIWindow *window, int x, int y, int width, int height)
{
    if (!window || !window->buffer.pixels || !window->buffer.image)
        return;

    int bw = window->buffer.width;
    int bh = window->buffer.height;

    // Completely outside → return
    if (x >= bw || y >= bh || x + width <= 0 || y + height <= 0)
        return;

    // ---- CLIP THE REGION (very important) ----

    int clippedX = x < 0 ? 0 : x;
    int clippedY = y < 0 ? 0 : y;

    int clippedW = (x + width > bw) ? (bw - clippedX) : (width - (clippedX - x));
    int clippedH = (y + height > bh) ? (bh - clippedY) : (height - (clippedY - y));

    XPutImage(window->windowState.display, window->windowState.window, window->windowState.gc, window->buffer.image,
              clippedX, clippedY, // src_x, src_y in ximage
              clippedX, clippedY, // dest_x, dest_y in window
              clippedW, clippedH);
}

/// @brief Checks if a specific key is currently pressed in the CGI window
/// @param window Pointer to the CGI window
/// @param key CGI input key to check
/// @return CGI_true if the key is pressed, CGI_false otherwise
CGIBool CGIIsWindowKeyDown(CGIWindow *window, CGIKeyCode key)
{
    if (window->event.cursorEvent.down_buttons & key)
    {
        return CGI_true;
    }

    if (window->event.keyboardEvent.down_keys & key)
    {
        return CGI_true;
    }

    return CGI_false;
}

CGIBool CGIIsWindowKeyUp(CGIWindow *window, CGIKeyCode key)
{
    if (window->event.cursorEvent.down_buttons & key)
    {
        return CGI_false;
    }

    if (window->event.keyboardEvent.down_keys & key)
    {
        return CGI_false;
    }

    return CGI_true;
}

CGIBool CGIIsWindowCursorKeyDown(CGIWindow *window, CGIKeyCode key)
{
    if (window->event.cursorEvent.down_buttons & key)
    {
        return CGI_true;
    }

    if (window->event.keyboardEvent.down_keys & key)
    {
        return CGI_true;
    }

    return CGI_false;
}

CGIBool CGIIsWindowCursorKeyUp(CGIWindow *window, CGIKeyCode key)
{
    if (window->event.cursorEvent.down_buttons & key)
    {
        return CGI_false;
    }

    if (window->event.keyboardEvent.down_keys & key)
    {
        return CGI_false;
    }

    return CGI_true;
}

/// @brief Resizes the buffer of a CGI window
/// @param window Pointer to the CGI window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGIResizeBuffer(CGIWindow *window)
{
    if (!window)
        return CGI_false;

    if (window->width == window->buffer.width && window->height == window->buffer.height)
    {
        return CGI_true;
    }

    DestroyFrameBuffer(window);
    window->event.windowEvent.is_resized = CGI_true;
    return initialize_cpu_buffer(window);
}

/// @brief Cleans up and frees resources associated with a CGI window
/// @param window Pointer to the CGI window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGIWindowCleanup(CGIWindow *window)
{
    if (!window)
        return CGI_false;

    DestroyFrameBuffer(window);

    if (window->windowState.gc)
    {
        XFreeGC(window->windowState.display, window->windowState.gc);
        window->windowState.gc = NULL;
    }

    if (window->windowState.window)
    {
        XDestroyWindow(window->windowState.display, window->windowState.window);
        window->windowState.window = 0;
    }

    if (window->windowState.display)
    {
        XCloseDisplay(window->windowState.display);
        window->windowState.display = NULL;
    }

    if (window->name)
    {
        free(window->name);
        window->name = NULL;
    }

    free(window);

    return CGI_true;
}

/// @brief Sets the width and height of a CGI window based on its current attributes
/// @param window Pointer to the CGI window
/// @return void
void set_width_and_height(CGIWindow *window)
{
    if (window == NULL)
    {
        return;
    }

    XWindowAttributes attrs;
    XGetWindowAttributes(window->windowState.display, window->windowState.window, &attrs);
    window->height = attrs.height;
    window->width = attrs.width;
    window->buffer_height = attrs.height;
    window->buffer_width = attrs.width;

    return;
}

/// @brief Sets the position of a CGI window on the screen
/// @param window Pointer to the CGI window
/// @return void
void set_window_pos_on_screen(CGIWindow *window)
{
    if (!window || !window->windowState.display)
        return;

    Window child;
    XTranslateCoordinates(window->windowState.display, window->windowState.window, RootWindow(window->windowState.display, window->windowState.screen), 0, 0, &window->position.x, &window->position.y, &child);
    return;
}

/// @brief Sets the display attributes of a CGI window
/// @param window Pointer to the CGI window
/// @return void
// void set_window_display_attrs(CGIWindow *window)
// {
//     if (!window || !window->windowState.display)
//         return;

//     Window root_window, child_window;
//     int root_x;
//     int root_y;
//     int win_x;
//     int win_y;
//     unsigned int mask;

//     XQueryPointer(window->windowState.display, window->windowState.window, &root_window, &child_window, &root_x, &root_y, &win_x, &win_y, &mask);

//     window->cursor.x = win_x;
//     window->cursor.y = win_y;

//     return;
// }

/// @brief Creates an XColor from a CGIColor_t for a given CGI window
/// @param window Pointer to the CGI window
/// @param color CGIColor_t color
/// @return XColor corresponding to the given CGIColor_t
XColor MakeColor(CGIWindow *window, CGIColor_t color)
{

    XColor xcolor;
    xcolor.red = (unsigned short)(color.r / 255.0 * 65535);
    xcolor.green = (unsigned short)(color.g / 255.0 * 65535);
    xcolor.blue = (unsigned short)(color.b / 255.0 * 65535);
    xcolor.flags = DoRed | DoGreen | DoBlue;

    XAllocColor(window->windowState.display, window->windowState.colormap, &xcolor);
    return xcolor;
}

/// @brief              Creates a CGI window with the specified parameters
/// @param classname    add a classname for the window although it is not used in linux
/// @param window_name  The name of the window
/// @param x_pos the x position of the window
/// @param y_pos the y position of the window
/// @param width the width of the window
/// @param height the height of the window
/// @param color the background color of the window
/// @return Pointer to the created CGIWindow or NULL on failure
CGIWindow *CGICreateWindow(char *classname, char *window_name, unsigned int x_pos, unsigned int y_pos, unsigned int width, unsigned int height, CGIColor_t color)
{

    CGIWindow *window = (CGIWindow *)malloc(sizeof(CGIWindow));

    if (!window)
        return NULL;

    window->windowState.display = XOpenDisplay(NULL);
    if (!window->windowState.display)
    {
        free(window);
        return NULL;
    }

    window->windowState.screen = DefaultScreen(window->windowState.display);
    window->windowState.colormap = DefaultColormap(window->windowState.display, window->windowState.screen);

    XColor xcolor;
    xcolor.red = (unsigned short)(color.r / 255.0 * 65535);
    xcolor.green = (unsigned short)(color.g / 255.0 * 65535);
    xcolor.blue = (unsigned short)(color.b / 255.0 * 65535);
    xcolor.flags = DoRed | DoGreen | DoBlue;
    XAllocColor(window->windowState.display, window->windowState.colormap, &xcolor);
    window->windowState.base_color = xcolor;
    window->CGIbase_color = color;

    window->windowState.window = XCreateSimpleWindow(window->windowState.display, RootWindow(window->windowState.display, window->windowState.screen), x_pos, y_pos, width, height, 1, BlackPixel(window->windowState.display, window->windowState.screen), xcolor.pixel);

    if (!window->windowState.window)
    {
        XCloseDisplay(window->windowState.display);
        free(window);
        return NULL;
    }

    // window->windowState.wm_delete_atom = XInternAtom(window->windowState.display, "_WM_DELETE_WINDOW", False);

    // XSetWMProtocols(window->windowState.display, window->windowState.window, &window->windowState.wm_delete_atom, 1);

    const char *title = (window_name != NULL) ? window_name : "";
    XStoreName(window->windowState.display, window->windowState.window, title);
    window->name = strdup(title);

    if (!window->name)
    {
        XDestroyWindow(window->windowState.display, window->windowState.window);
        XCloseDisplay(window->windowState.display);
        free(window);
        return NULL;
    }

    window->windowState.gc = XCreateGC(window->windowState.display, window->windowState.window, 0, NULL);

    XSelectInput(window->windowState.display, window->windowState.window, StructureNotifyMask | MotionNotify | PointerMotionMask | ButtonPressMask | ButtonReleaseMask);

    if (!window->windowState.gc)
    {
        CGIWindowCleanup(window);
        return NULL;
    }

    window->windowState.classname = classname;
    window->position.x = x_pos;
    window->position.y = y_pos;
    set_width_and_height(window);

    if (!initialize_cpu_buffer(window))
    {
        CGIWindowCleanup(window);
        return NULL;
    }

    CGIClearBuffer(window, color);
    window->open = CGI_false;
    window->event.windowEvent.is_focused = CGI_false;
    window->event.windowEvent.is_scrolled_x = CGI_false;
    window->event.windowEvent.is_scrolled_y = CGI_false;
    window->event.windowEvent.scroll_delta_x = 0;
    window->event.windowEvent.scroll_delta_y = 0;
    window->event.windowEvent.is_moved = CGI_false;
    window->event.windowEvent.is_resized = CGI_false;
    return window;
}

void ScanCurrentCursorState(CGIWindow *window, CGICursorEvent *event)
{

    Window root_window;
    Window child_window;
    int root_x, root_y, win_x, win_y;
    unsigned int mask;
    XQueryPointer(window->windowState.display, window->windowState.window, &root_window, &child_window, &root_x, &root_y, &win_x, &win_y, &mask);

    event->system_x = root_x;
    event->system_y = root_y;
    event->win_x = win_x;
    event->win_y = win_y;

    event->up_buttons = CGI_KEYCODE_NONE;
    event->down_buttons = CGI_KEYCODE_NONE;

    if (mask & Button1Mask)
    {
        event->down_buttons |= CGI_KEYCODE_MOUSE_L;
    }
    else
    {
        event->up_buttons |= CGI_KEYCODE_MOUSE_L;
    }

    if (mask & Button3Mask)
    {
        event->down_buttons |= CGI_KEYCODE_MOUSE_R;
    }
    else
    {
        event->up_buttons |= CGI_KEYCODE_MOUSE_R;
    }

    return;
}

void ScanCurrentKeyboardState(CGIWindow *window, CGIKeyboardEvent *event)
{

    event->down_keys = CGI_KEYCODE_NONE;
    event->up_keys = CGI_KEYCODE_NONE;

    char keys[32];
    XQueryKeymap(window->windowState.display, keys);

    // KeySym symbol = CGIInputKeyToKeySym();

    for (int i = 0; i < 59; i++)
    {
        KeySym symbol = CGIInputKeyToKeySym(KeyboardKeyCodes[i]);

        KeyCode keyCode = XKeysymToKeycode(window->windowState.display, symbol);

        if (keys[keyCode / 8] & (1 << (keyCode % 8)))
        {
            event->down_keys |= KeyboardKeyCodes[i];
        }
        else
        {
            event->up_keys |= KeyboardKeyCodes[i];
        }
    }

    return;
}

CGIEventImage CGIGetWindowEventImage(CGIWindow *window)
{
    return window->event;
}

CGIWindowSurface CGIGetWindowSurface(CGIWindow *window)
{
    CGIWindowSurface surface;
    surface.buffer = (void *)malloc(4 * window->buffer_width * window->buffer_height);

    unsigned char *buffer = (unsigned char *)surface.buffer;

    for (int i = 0; i < window->buffer_width * window->buffer_height; i++)
    {
        unsigned int p = window->buffer.pixels[i];
        buffer[i * 4 + 0] = (p >> 16) & 0xFF; // R
        buffer[i * 4 + 1] = (p >> 8) & 0xFF;  // G
        buffer[i * 4 + 2] = p & 0xFF;         // B
        buffer[i * 4 + 3] = (p >> 24) & 0xFF; // A
    }

    surface.height = window->buffer_height;
    surface.width = window->buffer_width;
    surface.channels = 4;

    return surface;
}

CGIWindowSurface CGIGetWindowSurfaceRegion(
    CGIWindow *window,
    int x_pos,
    int y_pos,
    int width,
    int height)
{
    CGIWindowSurface surface = {0};

    int winW = window->buffer_width;
    int winH = window->buffer_height;

    int win_x0 = x_pos < 0 ? 0 : x_pos;
    int win_y0 = y_pos < 0 ? 0 : y_pos;

    int win_x1 = x_pos + width > winW ? winW : x_pos + width;
    int win_y1 = y_pos + height > winH ? winH : y_pos + height;

    if (win_x1 <= win_x0 || win_y1 <= win_y0)
    {
        return surface; // empty
    }

    int copy_width = win_x1 - win_x0;
    int copy_height = win_y1 - win_y0;

    surface.channels = 4;
    surface.width = copy_width;
    surface.height = copy_height;

    surface.buffer = malloc(copy_width * copy_height * 4);
    if (!surface.buffer)
    {
        return surface;
    }

    unsigned char *dst = (unsigned char *)surface.buffer;
    unsigned int *src = window->buffer.pixels;

    /* ---- Copy pixels ---- */
    for (int y = win_y0; y < win_y1; y++)
    {
        int sy = y - win_y0;

        for (int x = win_x0; x < win_x1; x++)
        {
            int sx = x - win_x0;

            unsigned int p =
                src[y * winW + x];

            int dstPos = (sy * copy_width + sx) * 4;

            dst[dstPos + 0] = (p >> 16) & 0xFF; // R
            dst[dstPos + 1] = (p >> 8) & 0xFF;  // G
            dst[dstPos + 2] = p & 0xFF;         // B
            dst[dstPos + 3] = (p >> 24) & 0xFF; // A
        }
    }

    return surface;
}

void CGIFreeWindowSurface(CGIWindowSurface surface)
{
    if (surface.buffer)
    {
        free((void *)surface.buffer);
    }
    surface.buffer = NULL;
    surface.height = 0;
    surface.width = 0;
    surface.channels = 0;
}

/// @brief Checks if the CGI window is currently focused
/// @param window Pointer to the CGI window
/// @return CGI_true if the window is focused, CGI_false otherwise
CGIBool CGIIsWindowFocused(CGIWindow *window)
{
    if (!window || !window->windowState.display)
        return CGI_false;

    int revert;
    Window win;
    XGetInputFocus(window->windowState.display, &win, &revert);

    if (win == window->windowState.window)
    {
        return CGI_true;
    }

    return CGI_false;
}

/// @brief Shows the CGI window
/// @param window Pointer to the CGI window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGIShowWindow(CGIWindow *window)
{
    if (!window || !window->windowState.display)
        return CGI_false;

    XMapWindow(window->windowState.display, window->windowState.window);
    XFlush(window->windowState.display);
    // set_width_and_height(window);
    set_window_pos_on_screen(window);
    // set_window_display_attrs(window);

    window->open = CGI_true;
    window->event.windowEvent.is_focused = CGIIsWindowFocused(window);
    return CGI_true;
}

/// @brief Checks if the CGI window is currently open
/// @param window Pointer to the CGI window
/// @return CGI_true if the window is open, CGI_false otherwise
CGIBool CGIIsWindowOpen(const CGIWindow *window)
{
    if (!window)
        return CGI_false;

    return window->open;
}

/// @brief Processes X events for the CGI window
/// @param window Pointer to the CGI window
/// @param event Pointer to the XEvent to process
void process_events(CGIWindow *window, XEvent *event)
{
    if (!window)
        return;

    switch (event->type)
    {
    case ConfigureNotify:
    { // window->position.x = event->xconfigure.x;
        // window->position.y = event->xconfigure.y;

        int cur_x = window->position.x;
        int cur_y = window->position.y;
        set_window_pos_on_screen(window);

        if (window->position.x != cur_x || window->position.y != cur_y)
        {
            window->event.windowEvent.is_moved = CGI_true;
        }

        int cur_width = window->buffer_width;
        int cur_height = window->buffer_height;

        set_width_and_height(window);

        if (window->buffer_width != cur_width || window->buffer_height != cur_height)
        {
            CGIResizeBuffer(window);
            CGIClearBuffer(window, window->CGIbase_color);
        }

        break;
    }
    case MotionNotify:
    {
        // Window root, child;
        // int root_x, root_y, win_x, win_y;
        // unsigned int mask;

        // XQueryPointer(window->windowState.display,
        //               window->windowState.window,
        //               &root, &child,
        //               &root_x, &root_y,
        //               &win_x, &win_y,
        //               &mask);

        // window->event.windowEvent.cursor.x = win_x;
        // window->cursor.y = win_y;
        // break;
    }

        // case ClientMessage:
        // {
        //     if ((Atom)event->xclient.data.l[0] == window->windowState.wm_delete_atom)
        //     {
        //         window->open = CGI_false;
        //     }

        //     break;
        // }

    default:
        break;
    }

    setup_scroll_in_window(window, event);
}

/// @brief Closes the CGI window
/// @param window Pointer to the CGI window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGICloseWindow(CGIWindow *window)
{
    if (!window)
        return CGI_false;

    // XEvent event;
    // memset(&event, 0, sizeof(event));
    // event.xclient.type = ClientMessage;
    // event.xclient.window = window->windowState.window;
    // event.xclient.message_type = XInternAtom(window->windowState.display, "WM_PROTOCOLS", True);
    // event.xclient.format = 32;
    // event.xclient.data.l[0] = window->windowState.wm_delete_atom;
    // event.xclient.data.l[1] = CurrentTime;

    // // Send the event once
    // XSendEvent(window->windowState.display, window->windowState.window, False, NoEventMask, &event);
    // XFlush(window->windowState.display);

    // // Process the event immediately to update window->open status
    // while (XPending(window->windowState.display))
    // {
    //     XNextEvent(window->windowState.display, &window->windowState.event);
    //     process_events(window, &window->windowState.event);
    // }

    window->open = CGI_false;

    return CGI_true;
}

/// @brief Performs basic update operations on a CGI window
/// @param window Pointer to the CGI window
/// @return void
// void internal_window_basic_update(CGIWindow *window)
// {
//     set_window_display_attrs(window);
//     window->focused = CGIIsWindowFocused(window);
//     window->scroll_delta_x = 0;
//     window->scroll_delta_y = 0;
//     window->is_scrolled_x = CGI_false;
//     window->is_scrolled_y = CGI_false;
//     window->resized = CGI_false;
// }

void getCurrentEventImage(CGIWindow *window)
{
    ScanCurrentKeyboardState(window, &window->event.keyboardEvent);
    ScanCurrentCursorState(window, &window->event.cursorEvent);

    window->event.windowEvent.is_focused = CGIIsWindowFocused(window);
    window->event.windowEvent.is_moved = CGI_false;
    window->event.windowEvent.is_resized = CGI_false;
    window->event.windowEvent.is_scrolled_x = CGI_false;
    window->event.windowEvent.is_scrolled_y = CGI_false;
    window->event.windowEvent.scroll_delta_x = 0;
    window->event.windowEvent.scroll_delta_y = 0;
}

/// @brief Refreshes the CGI window by processing pending X events
/// @param window Pointer to the CGI window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGIRefreshWindow(CGIWindow *window, CGIWindowRefreshMode window_refresh_mode)
{
    if (!window || !window->windowState.display)
        return CGI_false;

    // internal_window_basic_update(window);
    getCurrentEventImage(window);

    if (window_refresh_mode == CGI_window_refresh_mode_rapid)
    {
        while (XPending(window->windowState.display))
        {
            XNextEvent(window->windowState.display, &window->windowState.event);
            process_events(window, &window->windowState.event);
        }
        return CGI_true;
    }

    if (window_refresh_mode == CGI_window_refresh_mode_triggered)
    {

        XNextEvent(window->windowState.display, &window->windowState.event);
        process_events(window, &window->windowState.event);

        return CGI_true;
    }

    return CGI_false;
}

/// @brief Performs a query on the CGI system or window
/// @param query The CGIQuery to perform
/// @param acceptor Pointer to the CGI or CGIWindow instance to query
/// @return Pointer to the queried data or NULL on failure
// const void *CGIPerformQuery(CGIQuery query, const void *acceptor)
// {
//     switch (query)
//     {
// ---------------- Window-related queries ----------------
// case CGI_query_window_name_charPointer:
// {
//     if (!acceptor)
//         return NULL;
//     CGIWindow *window = (CGIWindow *)acceptor;
//     return &window->name;
// }.
// case CGI_query_window_cursor_position_CGIPoint:
// {

//     if (!acceptor)
//         return NULL;
//     CGIWindow *window = (CGIWindow *)acceptor;
//     return &window->cursor;
// }

// case CGI_query_window_width_unsigned_int:
// {

//     if (!acceptor)
//         return NULL;
//     CGIWindow *window = (CGIWindow *)acceptor;
//     return &window->width;
// }

// case CGI_query_window_height_unsigned_int:
// {

//     if (!acceptor)
//         return NULL;
//     CGIWindow *window = (CGIWindow *)acceptor;
//     return &window->height;
// }

// case CGI_query_window_buffer_width_unsigned_int:
// {

//     if (!acceptor)
//         return NULL;
//     CGIWindow *window = (CGIWindow *)acceptor;
//     return &window->buffer_width;
// }

//     case CGI_query_window_buffer_height_unsigned_int:
//     {

//         if (!acceptor)
//             return NULL;
//         CGIWindow *window = (CGIWindow *)acceptor;
//         return &window->buffer_height;
//     }

//     case CGI_query_window_position_CGIPoint:
//     {

//         if (!acceptor)
//             return NULL;
//         CGIWindow *window = (CGIWindow *)acceptor;
//         return &window->position;
//     }

//     case CGI_query_window_open_status_CGIBool:
//     {

//         if (!acceptor)
//             return NULL;
//         CGIWindow *window = (CGIWindow *)acceptor;
//         return &window->open;
//     }

//     case CGI_query_window_base_color_CGIColor_t:
//     {

//         if (!acceptor)
//             return NULL;
//         CGIWindow *window = (CGIWindow *)acceptor;
//         return &window->CGIbase_color;
//     }

//     case CGI_query_window_focus_status_CGIBool:
//     {

//         if (!acceptor)
//             return NULL;
//         CGIWindow *window = (CGIWindow *)acceptor;
//         return &window->event.windowEvent.is_focused;
//     }

//         // // ---------------- System/CGI-related queries ----------------
//         // case CGI_query_system_cursor_position_CGIPoint:
//         // {

//         //     if (!acceptor)
//         //         return NULL;
//         //     CGI *cgi = (CGI *)acceptor;
//         //     return &cgi->Cursor.cursor_position;
//         // }

//         // case CGI_query_system_display_height_unsigned_int:
//         // {

//         //     if (!acceptor)
//         //         return NULL;
//         //     CGI *cgi = (CGI *)acceptor;
//         //     return &cgi->Display.height;
//         // }

//         // case CGI_query_system_display_width_unsigned_int:
//         // {

//         //     if (!acceptor)
//         //         return NULL;
//         //     CGI *cgi = (CGI *)acceptor;
//         //     return &cgi->Display.width;
//         // }

//         // case CGI_query_system_display_physical_height_unsigned_int:
//         // {
//         //     if (!acceptor)
//         //         return NULL;
//         //     CGI *cgi = (CGI *)acceptor;
//         //     return &cgi->Display.physical_height;
//         // }

//         // case CGI_query_system_display_physical_width_unsigned_int:
//         // {
//         //     if (!acceptor)
//         //         return NULL;
//         //     CGI *cgi = (CGI *)acceptor;
//         //     return &cgi->Display.physical_width;
//         // }

//         // case CGI_query_system_display_refresh_rate_unsigned_int:
//         // {
//         //     if (!acceptor)
//         //         return NULL;
//         //     CGI *cgi = (CGI *)acceptor;
//         //     return &cgi->Display.refresh_rate;
//         // }

//     case CGI_query_window_is_scrolled_x_CGIBool:
//     {
//         if (!acceptor)
//             return NULL;
//         CGIWindow *window = (CGIWindow *)acceptor;
//         return &window->event.windowEvent.is_scrolled_x;
//     }
//     case CGI_query_window_is_scrolled_y_CGIBool:
//     {
//         if (!acceptor)
//             return NULL;
//         CGIWindow *window = (CGIWindow *)acceptor;
//         return &window->event.windowEvent.is_scrolled_y;
//     }
//         // case CGI_query_system_l_button_pressed_CGIBool:
//         // {
//         //     if (!acceptor)
//         //         return NULL;
//         //     CGI *cgi = (CGI *)acceptor;
//         //     return &cgi->Cursor.l_button_pressed;
//         // }

//         // case CGI_query_system_r_button_pressed_CGIBool:
//         // {
//         //     if (!acceptor)
//         //         return NULL;
//         //     CGI *cgi = (CGI *)acceptor;
//         //     return &cgi->Cursor.r_button_pressed;
//         // }

//     case CGI_query_window_scroll_delta_x_float:
//     {
//         if (!acceptor)
//             return NULL;
//         CGIWindow *window = (CGIWindow *)acceptor;
//         return &window->event.windowEvent.scroll_delta_x;
//     }
//     case CGI_query_window_scroll_delta_y_float:
//     {
//         if (!acceptor)
//             return NULL;
//         CGIWindow *window = (CGIWindow *)acceptor;
//         return &window->event.windowEvent.scroll_delta_y;
//     }

//     // ---------------- Linux Xlib-specific queries ----------------
//     case CGI_query_window_internal_linux_Xlib_DisplayPointer:
//     {

//         if (!acceptor)
//             return NULL;
//         CGIWindow *window = (CGIWindow *)acceptor;
//         return &window->windowState.display;
//     }

//     case CGI_query_window_internal_linux_Xlib_screen_int:
//     {

//         if (!acceptor)
//             return NULL;
//         CGIWindow *window = (CGIWindow *)acceptor;
//         return &window->windowState.screen;
//     }

//     case CGI_query_window_internal_linux_Xlib_window_Window:
//     {

//         if (!acceptor)
//             return NULL;
//         CGIWindow *window = (CGIWindow *)acceptor;
//         return &window->windowState.window;
//     }

//     case CGI_query_window_internal_linux_Xlib_colormap_Colormap:
//     {

//         if (!acceptor)
//             return NULL;
//         CGIWindow *window = (CGIWindow *)acceptor;
//         return &window->windowState.colormap;
//     }

//     case CGI_query_window_internal_linux_Xlib_GC:
//     {

//         if (!acceptor)
//             return NULL;
//         CGIWindow *window = (CGIWindow *)acceptor;
//         return &window->windowState.gc;
//     }

//     case CGI_query_window_internal_linux_Xlib_type_base_color_XColor:
//     {

//         if (!acceptor)
//             return NULL;
//         CGIWindow *window = (CGIWindow *)acceptor;
//         return &window->windowState.base_color;
//     }

//     default:
//         break;
//     }
//     return NULL;
// }

/// @brief Performs a command on the CGI system or window
/// @param command The CGICommand to perform
/// @param args Pointer to the arguments for the command
/// @param acceptor Pointer to the CGI or CGIWindow instance to perform the command on
/// @return CGI_true on success, CGI_false on failure
// CGIBool CGIPerformCommand(CGICommand command, const void *args, const void *acceptor)
// {

//     switch (command)
//     {
//     case CGI_command_faulty_NULL:
//         return CGI_false;

//     // case CGI_command_CGI_start:
//     // {
//     //     if (!acceptor)
//     //         return CGI_false;

//     //     CGI *out_cgi = (CGI *)acceptor;
//     //     out_cgi = CGIStart();

//     //     return (out_cgi == NULL) ? CGI_false : CGI_true;
//     // }
//     // case CGI_command_CGI_update:
//     // {
//     //     if (!acceptor)
//     //         return CGI_false;
//     //     CGI *out_cgi = (CGI *)acceptor;
//     //     return CGIUpdate(out_cgi);
//     // }

//     // case CGI_command_CGI_end:
//     // {
//     //     if (!acceptor)
//     //         return CGI_false;
//     //     CGI *out_cgi = (CGI *)acceptor;
//     //     return CGIEnd(out_cgi);
//     // }

//     // case CGI_command_CGI_set_cursor_position:
//     // {
//     //     if (!args || !acceptor)
//     //         return CGI_false;
//     //     CGI *cgi = (CGI *)acceptor;
//     //     CGIPoint *point = (CGIPoint *)args;
//     //     Display *display = XOpenDisplay(NULL);
//     //     int screen = DefaultScreen(display);
//     //     XWarpPointer(display, None, RootWindow(display, screen), 0, 0, 0, 0, point->x, point->y);
//     //     XFlush(display);
//     //     XCloseDisplay(display);
//     //     CGIUpdate(cgi);

//     //     return CGI_true;
//     // }

//     case CGI_command_window_set_pixel:
//     {
//         if (!args || !acceptor)
//             return CGI_false;

//         CGIPixel *pixel = (CGIPixel *)args;
//         CGIWindow *window = (CGIWindow *)acceptor;

//         CGISetPixel(window, pixel->point.x, pixel->point.y, pixel->color);

//         return CGI_true;
//     }

//     case CGI_command_window_refresh_window_rapid:
//     {
//         if (!acceptor)
//             return CGI_false;

//         CGIWindow *window = (CGIWindow *)acceptor;
//         return CGIRefreshWindow(window, CGI_window_refresh_mode_rapid);
//     }

//     case CGI_command_window_refresh_window_triggered:
//     {
//         if (!acceptor)
//             return CGI_false;
//         CGIWindow *window = (CGIWindow *)acceptor;
//         return CGIRefreshWindow(window, CGI_window_refresh_mode_triggered);
//     }

//     case CGI_command_window_refresh_buffer:
//     {
//         if (!acceptor)
//             return CGI_false;
//         CGIWindow *window = (CGIWindow *)acceptor;
//         return CGIRefreshBuffer(window);
//     }

//     case CGI_command_window_clear_buffer:
//     {
//         if (!args || !acceptor)
//             return CGI_false;

//         CGIColor_t *color = (CGIColor_t *)args;
//         CGIWindow *window = (CGIWindow *)acceptor;

//         return CGIClearBuffer(window, *color);
//     }

//     case CGI_command_window_set_window_title:
//     {
//         if (!acceptor)
//             return CGI_false;

//         const char *title = (args == NULL) ? "" : (char *)args;

//         CGIWindow *window = (CGIWindow *)acceptor;

//         XStoreName((window)->windowState.display, (window)->windowState.window, title);

//         // Free old name before allocating new one
//         if (window->name)
//         {
//             free(window->name);
//         }
//         window->name = strdup(title);

//         if (!window->name)
//         {
//             return CGI_false;
//         }

//         return CGI_true;
//     }

//     case CGI_command_window_set_window_pos:
//     {
//         if (!args || !acceptor)
//             return CGI_false;

//         CGIWindow *window = (CGIWindow *)acceptor;
//         CGIPoint *point = (CGIPoint *)args;

//         XMoveWindow(window->windowState.display, window->windowState.window, point->x, point->y);

//         window->position.x = point->x;
//         window->position.y = point->y;

//         XFlush(window->windowState.display);

//         return CGI_true;
//     }

//     case CGI_command_window_set_window_size:
//     {
//         if (!args || !acceptor)
//             return CGI_false;

//         CGIPoint *point = (CGIPoint *)args;
//         CGIWindow *window = (CGIWindow *)acceptor;

//         XResizeWindow(window->windowState.display, window->windowState.window, point->x, point->y);

//         window->width = point->x;
//         window->height = point->y;

//         XFlush(window->windowState.display);

//         return CGI_true;
//     }

//     case CGI_command_window_set_window_base_color:
//     {
//         if (!args || !acceptor)
//             return CGI_false;

//         CGIColor_t *color = (CGIColor_t *)args;
//         CGIWindow *window = (CGIWindow *)acceptor;

//         window->windowState.base_color = MakeColor(window, *color);
//         window->CGIbase_color = *color;

//         return CGI_true;
//     }

//     case CGI_command_window_set_window_show_status:
//     {
//         if (!args || !acceptor)
//             return CGI_false;

//         CGIBool *logic = (CGIBool *)args;
//         CGIWindow *window = (CGIWindow *)acceptor;

//         if (*logic)
//         {
//             XMapWindow(window->windowState.display, window->windowState.window);
//             XFlush(window->windowState.display);

//             // Update window state similar to CGIShowWindow
//             set_width_and_height(window);
//             set_window_pos_on_screen(window);
//             // set_window_display_attrs(window);

//             window->open = CGI_true;
//             window->event.windowEvent.is_focused = CGIIsWindowFocused(window);
//         }
//         else
//         {
//             XUnmapWindow(window->windowState.display, window->windowState.window);
//             XFlush(window->windowState.display);

//             window->open = CGI_false;
//         }

//         return CGI_true;
//     }

//     case CGI_command_window_set_focus_status:
//     {
//         if (!args || !acceptor)
//             return CGI_false;

//         CGIBool *logic = (CGIBool *)args;
//         CGIWindow *window = (CGIWindow *)acceptor;

//         if (*logic)
//         {
//             XSetInputFocus(window->windowState.display, window->windowState.window, RevertToParent, CurrentTime);
//             window->event.windowEvent.is_focused = CGI_true;
//         }
//         else
//         {
//             XSetInputFocus(window->windowState.display, PointerRoot, RevertToParent, CurrentTime);
//             window->event.windowEvent.is_focused = CGI_false;
//         }

//         XFlush(window->windowState.display);

//         return CGI_true;
//     }

//     case CGI_command_window_close:
//     {
//         if (!acceptor)
//             return CGI_false;

//         CGIWindow *window = (CGIWindow *)acceptor;

//         return CGICloseWindow(window);
//     }

//     case CGI_command_window_resizable_logic:
//     {
//         // printf("here");
//         if (!args || !acceptor)
//             return CGI_false;

//         CGIBool *logic = (CGIBool *)args;
//         CGIWindow *window = (CGIWindow *)acceptor;

//         XSizeHints hints;
//         hints.flags = PMinSize | PMaxSize;
//         if (*logic)
//         {
//             hints.min_width = 0;
//             hints.max_width = DisplayWidth(window->windowState.display, window->windowState.screen);
//             hints.min_height = 0;
//             hints.max_height = DisplayHeight(window->windowState.display, window->windowState.screen);
//         }
//         else
//         {
//             hints.min_width = window->width;
//             hints.max_width = window->width;
//             hints.min_height = window->height;
//             hints.max_height = window->height;
//         }

//         XSetWMNormalHints(window->windowState.display, window->windowState.window, &hints);

//         XFlush(window->windowState.display);

//         return CGI_true;
//     }

//     case CGI_command_window_minimizable_logic:
//     {
//         return CGI_true;
//     }

//     case CGI_command_window_maximizable_logic:
//     {
//         return CGI_true;
//     }

//     default:
//         break;
//     }
//     return CGI_false;
// }

// query functions

/// @brief /// Checks if the CGI window has been resized
/// @param window Pointer to the CGI window
/// @return CGI_true if the window has been resized, CGI_false otherwise
CGIBool CGIIsWindowResized(CGIWindow *window)
{
    return window->event.windowEvent.is_resized;
}

/// @brief Gets the name of the CGI window
/// @param window Pointer to the CGI window
/// @return Pointer to the window name string
char *CGIGetWindowName(CGIWindow *window)
{
    return window->name;
}

/// @brief Gets the position of the CGI window
/// @param window Pointer to the CGI window
/// @return CGIPoint representing the window position
CGIPoint CGIGetWindowPosition(CGIWindow *window)
{
    return window->position;
}

/// @brief Gets the height of the CGI window
/// @param window Pointer to the CGI window
/// @return Height of the window
unsigned int CGIGetWindowHeight(CGIWindow *window)
{
    return window->height;
}

/// @brief Gets the width of the CGI window
/// @param window Pointer to the CGI window
/// @return Width of the window
unsigned int CGIGetWindowWidth(CGIWindow *window)
{
    return window->width;
}

/// @brief Gets the buffer height of the CGI window
/// @param window Pointer to the CGI window
/// @return Buffer height of the window

unsigned int CGIGetWindowBufferHeight(CGIWindow *window)
{
    return window->buffer_height;
}

/// @brief Gets the buffer width of the CGI window
/// @param window Pointer to the CGI window
/// @return Buffer width of the window
unsigned int CGIGetWindowBufferWidth(CGIWindow *window)
{
    return window->buffer_width;
}

/// @brief Gets the base color of the CGI window
/// @param window Pointer to the CGI window
/// @return Base color of the window
CGIColor_t CGIGetWindowBaseColor(CGIWindow *window)
{
    return window->CGIbase_color;
}

/// @brief Gets the cursor position of the CGI window
/// @param window Pointer to the CGI window
/// @return CGIPoint representing the cursor position
CGIPoint CGIGetWindowCursorPosition(CGIWindow *window)
{
    CGIPoint p;
    p.x = window->event.cursorEvent.win_x;
    p.y = window->event.cursorEvent.win_y;
    return p;
}

CGIPoint CGIGetSystemCursorPosition(CGIWindow *window)
{
    CGIPoint p;
    p.x = window->event.cursorEvent.system_x;
    p.y = window->event.cursorEvent.system_y;
    return p;
}

/// @brief Gets the scroll delta in the X direction of the CGI window
/// @param window Pointer to the CGI window
/// @return Scroll delta in the X direction
float CGIGetWindowScrollDeltaX(CGIWindow *window)
{
    return window->event.windowEvent.scroll_delta_x;
}

/// @brief Gets the scroll delta in the Y direction of the CGI window
/// @param window Pointer to the CGI window
/// @return Scroll delta in the Y direction
float CGIGetWindowScrollDeltaY(CGIWindow *window)
{
    return window->event.windowEvent.scroll_delta_y;
}

/// @brief Checks if the CGI window has been scrolled in the X direction
/// @param window Pointer to the CGI window
/// @return CGI_true if the window has been scrolled in the X direction, CGI_false otherwise
CGIBool CGIIsWindowScrolledX(CGIWindow *window)
{
    return window->event.windowEvent.is_scrolled_x;
}

/// @brief Checks if the CGI window has been scrolled in the Y direction
/// @param window Pointer to the CGI window
/// @return CGI_true if the window has been scrolled in the Y direction, CGI_false otherwise
CGIBool CGIIsWindowScrolledY(CGIWindow *window)
{
    return window->event.windowEvent.is_scrolled_y;
}

/// @brief Checks if a specific mouse button is currently pressed
/// @param cgi Pointer to the CGI instance
/// @param key CGI input key representing the mouse button to check
/// @return CGI_true if the mouse button is pressed, CGI_false otherwise
// CGIBool CGIIsMouseButtonPressed(CGI *cgi, CGIInputKey key)
// {
//     switch (key)
//     {
//     case CGI_input_key_mouse_l:
//     {
//         return cgi->Cursor.l_button_pressed;
//     }
//     case CGI_input_key_mouse_r:
//     {
//         return cgi->Cursor.r_button_pressed;
//     }
//     default:
//     {
//         return CGI_false;
//     }
//     }

//     return CGI_false;
// }

// commands function

/// @brief Sets the title of the CGI window
/// @param window Pointer to the CGI window
/// @param title New title for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowTitle(CGIWindow *window, char *title)
{

    XStoreName((window)->windowState.display, (window)->windowState.window, title);

    // Free old name before allocating new one
    if (window->name)
    {
        free(window->name);
    }
    window->name = strdup(title);

    if (!window->name)
    {
        return CGI_false;
    }

    return CGI_true;
}

/// @brief Sets the position of the CGI window
/// @param window Pointer to the CGI window
/// @param position New position for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowPosition(CGIWindow *window, CGIPoint position)
{

    XMoveWindow(window->windowState.display, window->windowState.window, position.x, position.y);

    window->position.x = position.x;
    window->position.y = position.y;

    XFlush(window->windowState.display);

    return CGI_true;
}

CGIBool CGISetCursorPosition(CGIWindow *window, CGIPoint point)
{
    XWarpPointer(window->windowState.display, None, DefaultRootWindow(window->windowState.display), 0, 0, 0, 0, point.x, point.y);
}

/// @brief Sets the size of the CGI window
/// @param window Pointer to the CGI window
/// @param size New size for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowSize(CGIWindow *window, CGIPoint size)
{

    XResizeWindow(window->windowState.display, window->windowState.window, size.x, size.y);

    window->width = size.x;
    window->height = size.y;

    XFlush(window->windowState.display);

    return CGI_true;
}

/// @brief Sets the base color of the CGI window
/// @param window Pointer to the CGI window
/// @param base_color New base color for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowBaseColor(CGIWindow *window, CGIColor_t base_color)
{

    window->windowState.base_color = MakeColor(window, base_color);
    window->CGIbase_color = base_color;
    return CGI_true;
}

/// @brief Sets the show logic of the CGI window
/// @param window Pointer to the CGI window
/// @param logic New show logic for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowShowLogic(CGIWindow *window, CGIBool logic)
{

    if (logic)
    {
        XMapWindow(window->windowState.display, window->windowState.window);
        XFlush(window->windowState.display);

        // Update window state similar to CGIShowWindow
        set_width_and_height(window);
        set_window_pos_on_screen(window);
        // set_window_display_attrs(window);

        window->open = CGI_true;
        window->event.windowEvent.is_focused = CGIIsWindowFocused(window);
    }
    else
    {
        XUnmapWindow(window->windowState.display, window->windowState.window);
        XFlush(window->windowState.display);

        window->open = CGI_false;
    }

    return CGI_true;
}

/// @brief Sets the focus logic of the CGI window
/// @param window Pointer to the CGI window
/// @param logic New focus logic for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowFocusLogic(CGIWindow *window, CGIBool logic)
{

    if (logic)
    {
        XSetInputFocus(window->windowState.display, window->windowState.window, RevertToParent, CurrentTime);
        window->event.windowEvent.is_focused = CGI_true;
    }
    else
    {
        XSetInputFocus(window->windowState.display, PointerRoot, RevertToParent, CurrentTime);
        window->event.windowEvent.is_focused = CGI_false;
    }

    XFlush(window->windowState.display);

    return CGI_true;
}

/// @brief Sets the resizable logic of the CGI window
/// @param window Pointer to the CGI window
/// @param logic New resizable logic for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowResizableLogic(CGIWindow *window, CGIBool logic)
{

    XSizeHints hints;
    hints.flags = PMinSize | PMaxSize;
    if (logic)
    {
        hints.min_width = 0;
        hints.max_width = DisplayWidth(window->windowState.display, window->windowState.screen);
        hints.min_height = 0;
        hints.max_height = DisplayHeight(window->windowState.display, window->windowState.screen);
    }
    else
    {
        hints.min_width = window->width;
        hints.max_width = window->width;
        hints.min_height = window->height;
        hints.max_height = window->height;
    }

    XSetWMNormalHints(window->windowState.display, window->windowState.window, &hints);

    XFlush(window->windowState.display);

    return CGI_true;
}

/// @brief Sets the minimizable logic of the CGI window
/// @param window Pointer to the CGI window
/// @param logic New minimizable logic for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowMinimizableLogic(CGIWindow *window, CGIBool logic)
{
    return CGI_true;
}

/// @brief Sets the maximizable logic of the CGI window
/// @param window Pointer to the CGI window
/// @param logic New maximizable logic for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowMaximizableLogic(CGIWindow *window, CGIBool logic)
{
    return CGI_true;
}

/// @brief Gets the Xlib Display pointer of the CGI window
/// @param window Pointer to the CGI window
/// @return Display pointer
Display *CGIGetWindow_Linux_Xlib_Display_pointer(CGIWindow *window)
{
    return window->windowState.display;
}

/// @brief Gets the Xlib screen number of the CGI window
/// @param window Pointer to the CGI window
/// @return Screen number
int CGIGetWindow_Linux_Xlib_screen(CGIWindow *window)
{
    return window->windowState.screen;
}

/// @brief Gets the Xlib Window of the CGI window
/// @param window Pointer to the CGI window
/// @return Window
Window CGIGetWindow_Linux_Xlib_window(CGIWindow *window)
{
    return window->windowState.window;
}

/// @brief Gets the Xlib Colormap of the CGI window
/// @param window Pointer to the CGI window
/// @return Colormap
Colormap CGIGetWindow_Linux_Xlib_colormap(CGIWindow *window)
{
    return window->windowState.colormap;
}

/// @brief Gets the Xlib GC (Graphics Context) of the CGI window
/// @param window Pointer to the CGI window
/// @return GC (Graphics Context)
GC CGIGetWindow_Linux_Xlib_GC(CGIWindow *window)
{
    return window->windowState.gc;
}

/// @brief Gets the Xlib base color of the CGI window
/// @param window Pointer to the CGI window
/// @return XColor representing the base color
XColor CGIGetWindow_Linux_Xlib_base_color(CGIWindow *window)
{
    return window->windowState.base_color;
}
