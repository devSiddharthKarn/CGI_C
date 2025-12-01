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
#include "cgi.h"
#include "X11/Xlib.h"
#include "X11/Xutil.h"
#include "X11/keysym.h"
#include <X11/extensions/Xrandr.h>
#include "stdlib.h"
#include "string.h"

// #include "stdio.h"

/// @brief Internal structure representing the CGI instance on Linux

struct CGI
{
    struct
    {
        unsigned int width;
        unsigned int height;
        unsigned int refresh_rate;
        unsigned int physical_width;
        unsigned int physical_height;

    } Display;

    struct
    {
        CGIPoint cursor_position;
        CGIBool l_button_pressed;
        CGIBool r_button_pressed;

    } Cursor;
};

/// @brief Initializes and starts the CGI instance on Linux
/// @return Pointer to the initialized CGI instance or NULL on failure
CGI *CGIStart()
{
    CGI *cgi = (CGI *)malloc(sizeof(CGI));
    if (!cgi)
    {
        return NULL;
    }
    Display *display = XOpenDisplay(NULL);
    if (!display)
    {
        free(cgi);
        return NULL;
    }

    int screen = DefaultScreen(display);

    cgi->Display.width = DisplayWidth(display, screen);
    cgi->Display.height = DisplayHeight(display, screen);
    cgi->Display.physical_height = DisplayHeightMM(display, screen);
    cgi->Display.physical_width = DisplayWidthMM(display, screen);

    // Get refresh rate using XRandR extension
    cgi->Display.refresh_rate = 60; // Default value
    XRRScreenConfiguration *screen_config = XRRGetScreenInfo(display, RootWindow(display, screen));
    if (screen_config)
    {
        cgi->Display.refresh_rate = XRRConfigCurrentRate(screen_config);
        XRRFreeScreenConfigInfo(screen_config);
    }

    Window root_window, child_window;
    unsigned int mask;
    int root_x, root_y, win_x, win_y;
    XQueryPointer(display, DefaultRootWindow(display), &root_window, &child_window,
                  &root_x, &root_y,
                  &win_x, &win_y, &mask);

    cgi->Cursor.cursor_position.x = root_x;
    cgi->Cursor.cursor_position.y = root_y;

    XCloseDisplay(display);

    cgi->Cursor.l_button_pressed = CGI_false;
    cgi->Cursor.r_button_pressed = CGI_false;

    return cgi;
}

/// @brief Updates the CGI instance on Linux
/// @param cgi Pointer to the CGI instance
/// @return CGI_true on success, CGI_false on failure
CGIBool CGIUpdate(CGI *cgi)
{
    if (!cgi)
        return CGI_false;

    Display *display = XOpenDisplay(NULL);
    if (!display)
        return CGI_false;

    int screen = DefaultScreen(display);

    cgi->Display.width = DisplayWidth(display, screen);
    cgi->Display.height = DisplayHeight(display, screen);

    Window root_window, child_window;
    unsigned int mask;
    int root_x, root_y, win_x, win_y;
    XQueryPointer(display, DefaultRootWindow(display), &root_window, &child_window,
                  &root_x, &root_y,
                  &win_x, &win_y, &mask);

    cgi->Cursor.cursor_position.x = root_x;
    cgi->Cursor.cursor_position.y = root_y;

    if (mask & Button1Mask)
    {
        cgi->Cursor.l_button_pressed = CGI_true;
    }
    else
    {
        cgi->Cursor.l_button_pressed = CGI_false;
    }

    if (mask & Button3Mask)
    {
        cgi->Cursor.r_button_pressed = CGI_true;
    }
    else
    {
        cgi->Cursor.r_button_pressed = CGI_false;
    }

    XCloseDisplay(display);

    return CGI_true;
}

/// @brief Ends the CGI instance on Linux and frees resources
/// @param cgi Pointer to the CGI instance
/// @return CGI_true on success, CGI_false on failure
CGIBool CGIEnd(CGI *cgi)
{
    if (!cgi)
        return CGI_false;

    free(cgi);
    return CGI_true;
}

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
    Atom wm_delete_atom;
};

/// @brief Internal structure representing a CGI window on Linux
struct CGIWindow
{
    char *name;
    struct WindowState windowState;
    CGIPoint cursor;
    CGIColor_t CGIbase_color;
    CGIPoint position;
    unsigned int width;
    unsigned int height;
    struct CGIFrameBuffer buffer;
    unsigned int buffer_width;
    unsigned int buffer_height;
    float scroll_delta_x;
    float scroll_delta_y;
    CGIBool is_scrolled_x;
    CGIBool is_scrolled_y;
    CGIBool open;
    CGIBool focused;
    CGIBool resized;
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
            window->scroll_delta_y = 1;
            window->is_scrolled_y = CGI_true;
            break;
        }
        case 5:
        {
            window->scroll_delta_y = -1;
            window->is_scrolled_y = CGI_true;
            break;
        }
        case 6:
        {
            window->scroll_delta_x = -1;
            window->is_scrolled_x = CGI_true;
            break;
        }
        case 7:
        {
            window->scroll_delta_x = 1;
            window->is_scrolled_x = CGI_true;
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

/// @brief Converts a CGI input key to an X11 KeySym
/// @param key CGI input key
/// @return Corresponding X11 KeySym or NoSymbol if not found
KeySym CGIInputKeyToKeySym(CGIInputKey key)
{
    switch (key)
    {
    case CGI_input_key_a:
        return XK_a;
    case CGI_input_key_b:
        return XK_b;
    case CGI_input_key_c:
        return XK_c;
    case CGI_input_key_d:
        return XK_d;
    case CGI_input_key_e:
        return XK_e;
    case CGI_input_key_f:
        return XK_f;
    case CGI_input_key_g:
        return XK_g;
    case CGI_input_key_h:
        return XK_h;
    case CGI_input_key_i:
        return XK_i;
    case CGI_input_key_j:
        return XK_j;
    case CGI_input_key_k:
        return XK_k;
    case CGI_input_key_l:
        return XK_l;
    case CGI_input_key_m:
        return XK_m;
    case CGI_input_key_n:
        return XK_n;
    case CGI_input_key_o:
        return XK_o;
    case CGI_input_key_p:
        return XK_p;
    case CGI_input_key_q:
        return XK_q;
    case CGI_input_key_r:
        return XK_r;
    case CGI_input_key_s:
        return XK_s;
    case CGI_input_key_t:
        return XK_t;
    case CGI_input_key_u:
        return XK_u;
    case CGI_input_key_v:
        return XK_v;
    case CGI_input_key_w:
        return XK_w;
    case CGI_input_key_x:
        return XK_x;
    case CGI_input_key_y:
        return XK_y;
    case CGI_input_key_z:
        return XK_z;
    case CGI_input_key_0:
        return XK_0;
    case CGI_input_key_1:
        return XK_1;
    case CGI_input_key_2:
        return XK_2;
    case CGI_input_key_3:
        return XK_3;
    case CGI_input_key_4:
        return XK_4;
    case CGI_input_key_5:
        return XK_5;
    case CGI_input_key_6:
        return XK_6;
    case CGI_input_key_7:
        return XK_7;
    case CGI_input_key_8:
        return XK_8;
    case CGI_input_key_9:
        return XK_9;
    case CGI_input_key_f1:
        return XK_F1;
    case CGI_input_key_f2:
        return XK_F2;
    case CGI_input_key_f3:
        return XK_F3;
    case CGI_input_key_f4:
        return XK_F4;
    case CGI_input_key_f5:
        return XK_F5;
    case CGI_input_key_f6:
        return XK_F6;
    case CGI_input_key_f7:
        return XK_F7;
    case CGI_input_key_f8:
        return XK_F8;
    case CGI_input_key_f9:
        return XK_F9;
    case CGI_input_key_f10:
        return XK_F10;
    case CGI_input_key_f11:
        return XK_F11;
    case CGI_input_key_f12:
        return XK_F12;
    case CGI_input_key_up:
        return XK_Up;
    case CGI_input_key_down:
        return XK_Down;
    case CGI_input_key_left:
        return XK_Left;
    case CGI_input_key_right:
        return XK_Right;
    case CGI_input_key_enter:
        return XK_Return;
    case CGI_input_key_escape:
        return XK_Escape;
    case CGI_input_key_space:
        return XK_space;
    case CGI_input_key_backspace:
        return XK_BackSpace;
    case CGI_input_key_shift:
        return XK_Shift_L;
    case CGI_input_key_ctrl:
        return XK_Control_L;
    case CGI_input_key_alt:
        return XK_Alt_L;

    default:
        return NoSymbol;
    }

    return NoSymbol;
}

/// @brief Checks if a specific key is currently pressed in the CGI window
/// @param window Pointer to the CGI window
/// @param key CGI input key to check
/// @return CGI_true if the key is pressed, CGI_false otherwise
CGIBool CGIIsKeyPressed(CGIWindow *window, CGIInputKey key)
{
    if (!window || !window->windowState.display)
        return CGI_false;

    char keys[32];

    XQueryKeymap(window->windowState.display, keys);

    KeySym keysym = CGIInputKeyToKeySym(key);
    if (keysym == NoSymbol)
        return CGI_false;

    KeyCode code = XKeysymToKeycode(window->windowState.display, keysym);
    if (code == 0)
        return CGI_false;

    if (keys[code / 8] & (1 << (code % 8)))
    {
        return CGI_true;
    }

    return CGI_false;
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
    window->resized = CGI_true;
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
void set_window_display_attrs(CGIWindow *window)
{
    if (!window || !window->windowState.display)
        return;

    Window root_window, child_window;
    int root_x;
    int root_y;
    int win_x;
    int win_y;
    unsigned int mask;

    XQueryPointer(window->windowState.display, window->windowState.window, &root_window, &child_window, &root_x, &root_y, &win_x, &win_y, &mask);

    window->cursor.x = win_x;
    window->cursor.y = win_y;

    return;
}

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

    window->windowState.wm_delete_atom = XInternAtom(window->windowState.display, "_WM_DELETE_WINDOW", False);

    XSetWMProtocols(window->windowState.display, window->windowState.window, &window->windowState.wm_delete_atom, 1);

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
    window->focused = CGI_false;
    window->is_scrolled_x = CGI_false;
    window->is_scrolled_y = CGI_false;
    window->scroll_delta_x = 0;
    window->scroll_delta_y = 0;

    return window;
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
    set_width_and_height(window);
    set_window_pos_on_screen(window);
    set_window_display_attrs(window);

    window->open = CGI_true;
    window->focused = CGIIsWindowFocused(window);
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
        window->position.x = event->xconfigure.x;
        window->position.y = event->xconfigure.y;
        set_window_pos_on_screen(window);
        set_width_and_height(window);
        CGIResizeBuffer(window);
        CGIClearBuffer(window, window->CGIbase_color);
        break;

    case MotionNotify:
    {
        Window root, child;
        int root_x, root_y, win_x, win_y;
        unsigned int mask;

        XQueryPointer(window->windowState.display,
                      window->windowState.window,
                      &root, &child,
                      &root_x, &root_y,
                      &win_x, &win_y,
                      &mask);

        window->cursor.x = win_x;
        window->cursor.y = win_y;
        break;
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
void internal_window_basic_update(CGIWindow *window)
{
    set_window_display_attrs(window);
    window->focused = CGIIsWindowFocused(window);
    window->scroll_delta_x = 0;
    window->scroll_delta_y = 0;
    window->is_scrolled_x = CGI_false;
    window->is_scrolled_y = CGI_false;
    window->resized = CGI_false;
}

/// @brief Refreshes the CGI window by processing pending X events
/// @param window Pointer to the CGI window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGIRefreshWindow(CGIWindow *window, CGIWindowRefreshMode window_refresh_mode)
{
    if (!window || !window->windowState.display)
        return CGI_false;

    internal_window_basic_update(window);

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
const void *CGIPerformQuery(CGIQuery query, const void *acceptor)
{
    switch (query)
    {
    // ---------------- Window-related queries ----------------
    case CGI_query_window_name_charPointer:
    {
        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->name;
    }
    case CGI_query_window_cursor_position_CGIPoint:
    {

        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->cursor;
    }

    case CGI_query_window_width_unsigned_int:
    {

        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->width;
    }

    case CGI_query_window_height_unsigned_int:
    {

        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->height;
    }

    case CGI_query_window_buffer_width_unsigned_int:
    {

        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->buffer_width;
    }

    case CGI_query_window_buffer_height_unsigned_int:
    {

        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->buffer_height;
    }

    case CGI_query_window_position_CGIPoint:
    {

        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->position;
    }

    case CGI_query_window_open_status_CGIBool:
    {

        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->open;
    }

    case CGI_query_window_base_color_CGIColor_t:
    {

        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->CGIbase_color;
    }

    case CGI_query_window_focus_status_CGIBool:
    {

        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->focused;
    }

    // ---------------- System/CGI-related queries ----------------
    case CGI_query_system_cursor_position_CGIPoint:
    {

        if (!acceptor)
            return NULL;
        CGI *cgi = (CGI *)acceptor;
        return &cgi->Cursor.cursor_position;
    }

    case CGI_query_system_display_height_unsigned_int:
    {

        if (!acceptor)
            return NULL;
        CGI *cgi = (CGI *)acceptor;
        return &cgi->Display.height;
    }

    case CGI_query_system_display_width_unsigned_int:
    {

        if (!acceptor)
            return NULL;
        CGI *cgi = (CGI *)acceptor;
        return &cgi->Display.width;
    }

    case CGI_query_system_display_physical_height_unsigned_int:
    {
        if (!acceptor)
            return NULL;
        CGI *cgi = (CGI *)acceptor;
        return &cgi->Display.physical_height;
    }

    case CGI_query_system_display_physical_width_unsigned_int:
    {
        if (!acceptor)
            return NULL;
        CGI *cgi = (CGI *)acceptor;
        return &cgi->Display.physical_width;
    }

    case CGI_query_system_display_refresh_rate_unsigned_int:
    {
        if (!acceptor)
            return NULL;
        CGI *cgi = (CGI *)acceptor;
        return &cgi->Display.refresh_rate;
    }

    case CGI_query_window_is_scrolled_x_CGIBool:
    {
        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->is_scrolled_x;
    }
    case CGI_query_window_is_scrolled_y_CGIBool:
    {
        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->is_scrolled_y;
    }
    case CGI_query_system_l_button_pressed_CGIBool:
    {
        if (!acceptor)
            return NULL;
        CGI *cgi = (CGI *)acceptor;
        return &cgi->Cursor.l_button_pressed;
    }

    case CGI_query_system_r_button_pressed_CGIBool:
    {
        if (!acceptor)
            return NULL;
        CGI *cgi = (CGI *)acceptor;
        return &cgi->Cursor.r_button_pressed;
    }

    case CGI_query_window_scroll_delta_x_float:
    {
        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->scroll_delta_x;
    }
    case CGI_query_window_scroll_delta_y_float:
    {
        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->scroll_delta_y;
    }

    // ---------------- Linux Xlib-specific queries ----------------
    case CGI_query_window_internal_linux_Xlib_DisplayPointer:
    {

        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->windowState.display;
    }

    case CGI_query_window_internal_linux_Xlib_screen_int:
    {

        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->windowState.screen;
    }

    case CGI_query_window_internal_linux_Xlib_window_Window:
    {

        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->windowState.window;
    }

    case CGI_query_window_internal_linux_Xlib_colormap_Colormap:
    {

        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->windowState.colormap;
    }

    case CGI_query_window_internal_linux_Xlib_GC:
    {

        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->windowState.gc;
    }

    case CGI_query_window_internal_linux_Xlib_type_base_color_XColor:
    {

        if (!acceptor)
            return NULL;
        CGIWindow *window = (CGIWindow *)acceptor;
        return &window->windowState.base_color;
    }

    default:
        break;
    }
    return NULL;
}

/// @brief Performs a command on the CGI system or window
/// @param command The CGICommand to perform
/// @param args Pointer to the arguments for the command
/// @param acceptor Pointer to the CGI or CGIWindow instance to perform the command on
/// @return CGI_true on success, CGI_false on failure
CGIBool CGIPerformCommand(CGICommand command, const void *args, const void *acceptor)
{

    switch (command)
    {
    case CGI_command_faulty_NULL:
        return CGI_false;

    case CGI_command_CGI_start:
    {
        if (!acceptor)
            return CGI_false;

        CGI *out_cgi = (CGI *)acceptor;
        out_cgi = CGIStart();

        return (out_cgi == NULL) ? CGI_false : CGI_true;
    }
    case CGI_command_CGI_update:
    {
        if (!acceptor)
            return CGI_false;
        CGI *out_cgi = (CGI *)acceptor;
        return CGIUpdate(out_cgi);
    }

    case CGI_command_CGI_end:
    {
        if (!acceptor)
            return CGI_false;
        CGI *out_cgi = (CGI *)acceptor;
        return CGIEnd(out_cgi);
    }

    case CGI_command_CGI_set_cursor_position:
    {
        if (!args || !acceptor)
            return CGI_false;
        CGI *cgi = (CGI *)acceptor;
        CGIPoint *point = (CGIPoint *)args;
        Display *display = XOpenDisplay(NULL);
        int screen = DefaultScreen(display);
        XWarpPointer(display, None, RootWindow(display, screen), 0, 0, 0, 0, point->x, point->y);
        XFlush(display);
        XCloseDisplay(display);
        CGIUpdate(cgi);

        return CGI_true;
    }

    case CGI_command_window_set_pixel:
    {
        if (!args || !acceptor)
            return CGI_false;

        CGIPixel *pixel = (CGIPixel *)args;
        CGIWindow *window = (CGIWindow *)acceptor;

        CGISetPixel(window, pixel->point.x, pixel->point.y, pixel->color);

        return CGI_true;
    }

    case CGI_command_window_refresh_window_rapid:
    {
        if (!acceptor)
            return CGI_false;

        CGIWindow *window = (CGIWindow *)acceptor;
        return CGIRefreshWindow(window, CGI_window_refresh_mode_rapid);
    }

    case CGI_command_window_refresh_window_triggered:
    {
        if (!acceptor)
            return CGI_false;
        CGIWindow *window = (CGIWindow *)acceptor;
        return CGIRefreshWindow(window, CGI_window_refresh_mode_triggered);
    }

    case CGI_command_window_refresh_buffer:
    {
        if (!acceptor)
            return CGI_false;
        CGIWindow *window = (CGIWindow *)acceptor;
        return CGIRefreshBuffer(window);
    }

    case CGI_command_window_clear_buffer:
    {
        if (!args || !acceptor)
            return CGI_false;

        CGIColor_t *color = (CGIColor_t *)args;
        CGIWindow *window = (CGIWindow *)acceptor;

        return CGIClearBuffer(window, *color);
    }

    case CGI_command_window_set_window_title:
    {
        if (!acceptor)
            return CGI_false;

        const char *title = (args == NULL) ? "" : (char *)args;

        CGIWindow *window = (CGIWindow *)acceptor;

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

    case CGI_command_window_set_window_pos:
    {
        if (!args || !acceptor)
            return CGI_false;

        CGIWindow *window = (CGIWindow *)acceptor;
        CGIPoint *point = (CGIPoint *)args;

        XMoveWindow(window->windowState.display, window->windowState.window, point->x, point->y);

        window->position.x = point->x;
        window->position.y = point->y;

        XFlush(window->windowState.display);

        return CGI_true;
    }

    case CGI_command_window_set_window_size:
    {
        if (!args || !acceptor)
            return CGI_false;

        CGIPoint *point = (CGIPoint *)args;
        CGIWindow *window = (CGIWindow *)acceptor;

        XResizeWindow(window->windowState.display, window->windowState.window, point->x, point->y);

        window->width = point->x;
        window->height = point->y;

        XFlush(window->windowState.display);

        return CGI_true;
    }

    case CGI_command_window_set_window_base_color:
    {
        if (!args || !acceptor)
            return CGI_false;

        CGIColor_t *color = (CGIColor_t *)args;
        CGIWindow *window = (CGIWindow *)acceptor;

        window->windowState.base_color = MakeColor(window, *color);
        window->CGIbase_color = *color;

        return CGI_true;
    }

    case CGI_command_window_set_window_show_status:
    {
        if (!args || !acceptor)
            return CGI_false;

        CGIBool *logic = (CGIBool *)args;
        CGIWindow *window = (CGIWindow *)acceptor;

        if (*logic)
        {
            XMapWindow(window->windowState.display, window->windowState.window);
            XFlush(window->windowState.display);

            // Update window state similar to CGIShowWindow
            set_width_and_height(window);
            set_window_pos_on_screen(window);
            set_window_display_attrs(window);

            window->open = CGI_true;
            window->focused = CGIIsWindowFocused(window);
        }
        else
        {
            XUnmapWindow(window->windowState.display, window->windowState.window);
            XFlush(window->windowState.display);

            window->open = CGI_false;
        }

        return CGI_true;
    }

    case CGI_command_window_set_focus_status:
    {
        if (!args || !acceptor)
            return CGI_false;

        CGIBool *logic = (CGIBool *)args;
        CGIWindow *window = (CGIWindow *)acceptor;

        if (*logic)
        {
            XSetInputFocus(window->windowState.display, window->windowState.window, RevertToParent, CurrentTime);
            window->focused = CGI_true;
        }
        else
        {
            XSetInputFocus(window->windowState.display, PointerRoot, RevertToParent, CurrentTime);
            window->focused = CGI_false;
        }

        XFlush(window->windowState.display);

        return CGI_true;
    }

    case CGI_command_window_close:
    {
        if (!acceptor)
            return CGI_false;

        CGIWindow *window = (CGIWindow *)acceptor;

        return CGICloseWindow(window);
    }

    case CGI_command_window_resizable_logic:
    {
        // printf("here");
        if (!args || !acceptor)
            return CGI_false;

        CGIBool *logic = (CGIBool *)args;
        CGIWindow *window = (CGIWindow *)acceptor;

        XSizeHints hints;
        hints.flags = PMinSize | PMaxSize;
        if (*logic)
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

    case CGI_command_window_minimizable_logic:
    {
        return CGI_true;
    }

    case CGI_command_window_maximizable_logic:
    {
        return CGI_true;
    }

    default:
        break;
    }
    return CGI_false;
}

// query functions

/// @brief /// Checks if the CGI window has been resized
/// @param window Pointer to the CGI window
/// @return CGI_true if the window has been resized, CGI_false otherwise
CGIBool CGIIsWindowResized(CGIWindow *window)
{
    return window->resized;
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
    return window->cursor;
}

/// @brief Gets the scroll delta in the X direction of the CGI window
/// @param window Pointer to the CGI window
/// @return Scroll delta in the X direction
float CGIGetWindowScrollDeltaX(CGIWindow *window)
{
    return window->scroll_delta_x;
}

/// @brief Gets the scroll delta in the Y direction of the CGI window
/// @param window Pointer to the CGI window
/// @return Scroll delta in the Y direction
float CGIGetWindowScrollDeltaY(CGIWindow *window)
{
    return window->scroll_delta_y;
}

/// @brief Checks if the CGI window has been scrolled in the X direction
/// @param window Pointer to the CGI window
/// @return CGI_true if the window has been scrolled in the X direction, CGI_false otherwise
CGIBool CGIIsWindowScrolledX(CGIWindow *window)
{
    return window->is_scrolled_x;
}

/// @brief Checks if the CGI window has been scrolled in the Y direction
/// @param window Pointer to the CGI window
/// @return CGI_true if the window has been scrolled in the Y direction, CGI_false otherwise
CGIBool CGIIsWindowScrolledY(CGIWindow *window)
{
    return window->is_scrolled_y;
}

/// @brief Checks if a specific mouse button is currently pressed
/// @param cgi Pointer to the CGI instance
/// @param key CGI input key representing the mouse button to check
/// @return CGI_true if the mouse button is pressed, CGI_false otherwise
CGIBool CGIIsMouseButtonPressed(CGI *cgi, CGIInputKey key)
{
    switch (key)
    {
    case CGI_input_key_mouse_l:
    {
        return cgi->Cursor.l_button_pressed;
    }
    case CGI_input_key_mouse_r:
    {
        return cgi->Cursor.r_button_pressed;
    }
    default:
    {
        return CGI_false;
    }
    }

    return CGI_false;
}

// commands function

/// @brief Sets the title of the CGI window
/// @param window Pointer to the CGI window
/// @param title New title for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowTitle(CGIWindow *window, char *title)
{

    return CGIPerformCommand(CGI_command_window_set_window_title, title, window);
}

/// @brief Sets the position of the CGI window
/// @param window Pointer to the CGI window
/// @param position New position for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowPosition(CGIWindow *window, CGIPoint position)
{
    return CGIPerformCommand(CGI_command_window_set_window_pos, &position, window);
}

/// @brief Sets the size of the CGI window
/// @param window Pointer to the CGI window
/// @param size New size for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowSize(CGIWindow *window, CGIPoint size)
{
    return CGIPerformCommand(CGI_command_window_set_window_size, &size, window);
}

/// @brief Sets the base color of the CGI window
/// @param window Pointer to the CGI window
/// @param base_color New base color for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowBaseColor(CGIWindow *window, CGIColor_t base_color)
{
    return CGIPerformCommand(CGI_command_window_set_window_base_color, &base_color, window);
}

/// @brief Sets the show logic of the CGI window
/// @param window Pointer to the CGI window
/// @param logic New show logic for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowShowLogic(CGIWindow *window, CGIBool logic)
{
    return CGIPerformCommand(CGI_command_window_set_window_show_status, &logic, window);
}

/// @brief Sets the focus logic of the CGI window
/// @param window Pointer to the CGI window
/// @param logic New focus logic for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowFocusLogic(CGIWindow *window, CGIBool logic)
{
    return CGIPerformCommand(CGI_command_window_set_focus_status, &logic, window);
}

/// @brief Sets the resizable logic of the CGI window
/// @param window Pointer to the CGI window
/// @param logic New resizable logic for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowResizableLogic(CGIWindow *window, CGIBool logic)
{
    return CGIPerformCommand(CGI_command_window_resizable_logic, &logic, window);
}

/// @brief Sets the minimizable logic of the CGI window
/// @param window Pointer to the CGI window
/// @param logic New minimizable logic for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowMinimizableLogic(CGIWindow *window, CGIBool logic)
{
    return CGIPerformCommand(CGI_command_window_minimizable_logic, &logic, window);
}

/// @brief Sets the maximizable logic of the CGI window
/// @param window Pointer to the CGI window
/// @param logic New maximizable logic for the window
/// @return CGI_true on success, CGI_false on failure
CGIBool CGISetWindowMaximizableLogic(CGIWindow *window, CGIBool logic)
{
    return CGIPerformCommand(CGI_command_window_maximizable_logic, &logic, window);
}