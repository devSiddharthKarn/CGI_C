#define CGI_LINUX_IMPLEMENTATION_ACTIVE
#include "cgi.h"
#include "X11/Xlib.h"
#include "X11/Xutil.h"
#include "X11/keysym.h"
#include <X11/extensions/Xrandr.h>
#include "stdlib.h"
#include "string.h"

// #include "stdio.h"

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

CGIBool CGIEnd(CGI *cgi)
{
    if (!cgi)
        return CGI_false;

    free(cgi);
    return CGI_true;
}

struct CGIFrameBuffer
{
    unsigned int *pixels;
    unsigned int width;
    unsigned int height;
    XImage *image;
};

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

CGIColor_t CGIMakeColor(unsigned char r, unsigned char g, unsigned char b)
{
    CGIColor_t color;
    color.r = r;
    color.g = g;
    color.b = b;
    return color;
}

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

void CGISetPixel(CGIWindow *window, int x_pos, int y_pos, CGIColor_t color)
{
    if (!window)
        return;

    if (x_pos < 0 || x_pos >= window->buffer.width || y_pos < 0 || y_pos >= window->buffer.height)
        return;

    unsigned int pixelColor = 0xFF000000 | (color.r << 16) | (color.g << 8) | (color.b);

    window->buffer.pixels[y_pos * window->buffer.width + x_pos] = pixelColor;
}

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

CGIBool CGIRefreshBuffer(CGIWindow *window)
{
    if (!window || !window->buffer.pixels || !window->buffer.image)
        return CGI_false;

    XPutImage(window->windowState.display, window->windowState.window, window->windowState.gc, window->buffer.image, 0, 0, 0, 0, window->buffer.width, window->buffer.height);

    XFlush(window->windowState.display);
    return CGI_true;
}

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

CGIBool CGIResizeBuffer(CGIWindow *window)
{
    if (!window)
        return CGI_false;

    if (window->width == window->buffer.width && window->height == window->buffer.height)
    {
        return CGI_true;
    }

    DestroyFrameBuffer(window);
    window->resized=CGI_true;
    return initialize_cpu_buffer(window);
}

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

void set_window_pos_on_screen(CGIWindow *window)
{
    if (!window || !window->windowState.display)
        return;

    Window child;
    XTranslateCoordinates(window->windowState.display, window->windowState.window, RootWindow(window->windowState.display, window->windowState.screen), 0, 0, &window->position.x, &window->position.y, &child);
    return;
}

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

CGIBool CGIIsWindowOpen(const CGIWindow *window)
{
    if (!window)
        return CGI_false;

    return window->open;
}



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

    window->open=CGI_false;

    return CGI_true;
}


void internal_window_basic_update(CGIWindow *window)
{
    set_window_display_attrs(window);
    window->focused = CGIIsWindowFocused(window);
    window->scroll_delta_x = 0;
    window->scroll_delta_y = 0;
    window->is_scrolled_x = CGI_false;
    window->is_scrolled_y = CGI_false;
    window->resized=CGI_false;
}

CGIBool CGIRefreshWindow(CGIWindow *window)
{
    if (!window || !window->windowState.display)
        return CGI_false;

    internal_window_basic_update(window);
    while (XPending(window->windowState.display))
    {
        XNextEvent(window->windowState.display, &window->windowState.event);
        process_events(window, &window->windowState.event);
    }

    return CGI_true;
}
const void *CGIPerformQuery(CGIQuery query, CGI *cgi, CGIWindow *window)
{
    switch (query)
    {
    // ---------------- Window-related queries ----------------
    case CGI_query_window_name_charPointer:
    {
        if (!window)
            return NULL;
        return &window->name;
    }
    case CGI_query_window_cursor_position_CGIPoint:
        if (!window)
            return NULL;
        return &window->cursor;

    case CGI_query_window_width_unsigned_int:
        if (!window)
            return NULL;
        return &window->width;

    case CGI_query_window_height_unsigned_int:
        if (!window)
            return NULL;
        return &window->height;

    case CGI_query_window_buffer_width_unsigned_int:
        if (!window)
            return NULL;
        return &window->buffer_width;

    case CGI_query_window_buffer_height_unsigned_int:
        if (!window)
            return NULL;
        return &window->buffer_height;

    case CGI_query_window_position_CGIPoint:
        if (!window)
            return NULL;
        return &window->position;

    case CGI_query_window_open_status_CGIBool:
        if (!window)
            return NULL;
        return &window->open;

    case CGI_query_window_base_color_CGIColor_t:
        if (!window)
            return NULL;
        return &window->CGIbase_color;

    case CGI_query_window_focus_status_CGIBool:
        if (!window)
            return NULL;
        return &window->focused;

    // ---------------- System/CGI-related queries ----------------
    case CGI_query_system_cursor_position_CGIPoint:
        if (!cgi)
            return NULL;
        return &cgi->Cursor.cursor_position;

    case CGI_query_system_display_height_unsigned_int:
        if (!cgi)
            return NULL;
        return &cgi->Display.height;

    case CGI_query_system_display_width_unsigned_int:
        if (!cgi)
            return NULL;
        return &cgi->Display.width;

    case CGI_query_system_display_physical_height_unsigned_int:
    {
        if (!cgi)
            return NULL;
        return &cgi->Display.physical_height;
    }

    case CGI_query_system_display_physical_width_unsigned_int:
    {
        if (!cgi)
            return NULL;
        return &cgi->Display.physical_width;
    }

    case CGI_query_system_display_refresh_rate_unsigned_int:
    {
        if (!cgi)
            return NULL;
        return &cgi->Display.refresh_rate;
    }

    case CGI_query_window_is_scrolled_x_CGIBool:
    {
        if (!window)
            return NULL;
        return &window->is_scrolled_x;
    }
    case CGI_query_window_is_scrolled_y_CGIBool:
    {
        if (!window)
            return NULL;
        return &window->is_scrolled_y;
    }
    case CGI_query_system_l_button_pressed_CGIBool:
    {
        if (!cgi)
            return NULL;
        return &cgi->Cursor.l_button_pressed;
    }

    case CGI_query_system_r_button_pressed_CGIBool:
    {
        if (!cgi)
            return NULL;
        return &cgi->Cursor.r_button_pressed;
    }

    case CGI_query_window_scroll_delta_x_float:
    {
        if (!window)
            return NULL;
        return &window->scroll_delta_x;
    }
    case CGI_query_window_scroll_delta_y_float:
    {
        if (!window)
            return NULL;
        return &window->scroll_delta_y;
    }

    // ---------------- Linux Xlib-specific queries ----------------
    case CGI_query_window_internal_linux_Xlib_DisplayPointer:
        if (!window)
            return NULL;
        return &window->windowState.display;

    case CGI_query_window_internal_linux_Xlib_screen_int:
        if (!window)
            return NULL;
        return &window->windowState.screen;

    case CGI_query_window_internal_linux_Xlib_window_Window:
        if (!window)
            return NULL;
        return &window->windowState.window;

    case CGI_query_window_internal_linux_Xlib_colormap_Colormap:
        if (!window)
            return NULL;
        return &window->windowState.colormap;

    case CGI_query_window_internal_linux_Xlib_GC:
        if (!window)
            return NULL;
        return &window->windowState.gc;

    case CGI_query_window_internal_linux_Xlib_type_base_color_XColor:
        if (!window)
            return NULL;
        return &window->windowState.base_color;

    default:
        break;
    }
    return NULL;
}

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

        CGI **out_cgi = (CGI **)acceptor;
        *out_cgi = CGIStart();

        return (*out_cgi == NULL) ? CGI_false : CGI_true;
    }
    case CGI_command_CGI_update:
    {
        if (!acceptor)
            return CGI_false;
        CGI **out_cgi = (CGI **)acceptor;
        return CGIUpdate(*out_cgi);
    }

    case CGI_command_CGI_end:
    {
        if (!acceptor)
            return CGI_false;
        CGI **out_cgi = (CGI **)acceptor;
        return CGIEnd(*out_cgi);
    }

    case CGI_command_CGI_set_cursor_position:
    {
        if (!args || !acceptor)
            return CGI_false;
        CGI **cgi = (CGI **)acceptor;
        CGIPoint *point = (CGIPoint *)args;
        Display *display = XOpenDisplay(NULL);
        int screen = DefaultScreen(display);
        XWarpPointer(display, None, RootWindow(display, screen), 0, 0, 0, 0, point->x, point->y);
        XFlush(display);
        XCloseDisplay(display);
        CGIUpdate(*cgi);

        return CGI_true;
    }

    case CGI_command_window_set_pixel:
    {
        if (!args || !acceptor)
            return CGI_false;

        CGIPixel *pixel = (CGIPixel *)args;
        CGIWindow **window = (CGIWindow **)acceptor;

        CGISetPixel(*window, pixel->point.x, pixel->point.y, pixel->color);

        return CGI_true;
    }

    case CGI_command_window_refresh_window:
    {
        if (!acceptor)
            return CGI_false;

        CGIWindow **window = (CGIWindow **)acceptor;
        return CGIRefreshWindow(*window);
    }

    case CGI_command_window_refresh_buffer:
    {
        if (!acceptor)
            return CGI_false;
        CGIWindow **window = (CGIWindow **)acceptor;
        return CGIRefreshBuffer(*window);
    }

    case CGI_command_window_clear_buffer:
    {
        if (!args || !acceptor)
            return CGI_false;

        CGIColor_t *color = (CGIColor_t *)args;
        CGIWindow **window = (CGIWindow **)acceptor;

        return CGIClearBuffer(*window, *color);
    }

    case CGI_command_window_set_window_title:
    {
        if (!acceptor)
            return CGI_false;

        const char *title = (args == NULL) ? "" : (*(char **)args);

        CGIWindow **window = (CGIWindow **)acceptor;

        XStoreName((*window)->windowState.display, (*window)->windowState.window, title);

        // Free old name before allocating new one
        if ((*window)->name)
        {
            free((*window)->name);
        }
        (*window)->name = strdup(title);

        if (!(*window)->name)
        {
            return CGI_false;
        }

        return CGI_true;
    }

    case CGI_command_window_set_window_pos:
    {
        if (!args || !acceptor)
            return CGI_false;

        CGIWindow **window = (CGIWindow **)acceptor;
        CGIPoint *point = (CGIPoint *)args;

        XMoveWindow((*window)->windowState.display, (*window)->windowState.window, point->x, point->y);

        (*window)->position.x = point->x;
        (*window)->position.y = point->y;

        XFlush((*window)->windowState.display);

        return CGI_true;
    }

    case CGI_command_window_set_window_size:
    {
        if (!args || !acceptor)
            return CGI_false;

        CGIPoint *point = (CGIPoint *)args;
        CGIWindow **window = (CGIWindow **)acceptor;

        XResizeWindow((*window)->windowState.display, (*window)->windowState.window, point->x, point->y);

        (*window)->width = point->x;
        (*window)->height = point->y;

        XFlush((*window)->windowState.display);

        return CGI_true;
    }

    case CGI_command_window_set_window_base_color:
    {
        if (!args || !acceptor)
            return CGI_false;

        CGIColor_t *color = (CGIColor_t *)args;
        CGIWindow **window = (CGIWindow **)acceptor;

        (*window)->windowState.base_color = MakeColor((*window), *color);
        (*window)->CGIbase_color = *color;

        return CGI_true;
    }

    case CGI_command_window_set_window_show_status:
    {
        if (!args || !acceptor)
            return CGI_false;

        CGIBool *logic = (CGIBool *)args;
        CGIWindow **window = (CGIWindow **)acceptor;

        if (*logic)
        {
            XMapWindow((*window)->windowState.display, (*window)->windowState.window);
            XFlush((*window)->windowState.display);

            // Update window state similar to CGIShowWindow
            set_width_and_height(*window);
            set_window_pos_on_screen(*window);
            set_window_display_attrs(*window);

            (*window)->open = CGI_true;
            (*window)->focused = CGIIsWindowFocused(*window);
        }
        else
        {
            XUnmapWindow((*window)->windowState.display, (*window)->windowState.window);
            XFlush((*window)->windowState.display);

            (*window)->open = CGI_false;
        }

        return CGI_true;
    }

    case CGI_command_window_set_focus_status:
    {
        if (!args || !acceptor)
            return CGI_false;

        CGIBool *logic = (CGIBool *)args;
        CGIWindow **window = (CGIWindow **)acceptor;

        if (*logic)
        {
            XSetInputFocus((*window)->windowState.display, (*window)->windowState.window, RevertToParent, CurrentTime);
            (*window)->focused = CGI_true;
        }
        else
        {
            XSetInputFocus((*window)->windowState.display, PointerRoot, RevertToParent, CurrentTime);
            (*window)->focused = CGI_false;
        }

        XFlush((*window)->windowState.display);

        return CGI_true;
    }

    case CGI_command_window_close:
    {
        if (!acceptor)
            return CGI_false;

        CGIWindow **window = (CGIWindow **)acceptor;

        return CGICloseWindow(*window);
    }

    case CGI_command_window_resizable_logic:
    {
        // printf("here");
        if (!args || !acceptor)
            return CGI_false;

        CGIBool *logic = (CGIBool *)args;
        CGIWindow **window = (CGIWindow **)acceptor;

        XSizeHints hints;
        hints.flags = PMinSize | PMaxSize;
        if (*logic)
        {
            hints.min_width = 0;
            hints.max_width = DisplayWidth((*window)->windowState.display, (*window)->windowState.screen);
            hints.min_height = 0;
            hints.max_height = DisplayHeight((*window)->windowState.display, (*window)->windowState.screen);
        }
        else
        {
            hints.min_width = (*window)->width;
            hints.max_width = (*window)->width;
            hints.min_height = (*window)->height;
            hints.max_height = (*window)->height;
        }

        XSetWMNormalHints((*window)->windowState.display, (*window)->windowState.window, &hints);

        XFlush((*window)->windowState.display);

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


CGIBool CGIIsWindowResized(CGIWindow* window){
    return window->resized;
}


CGIPoint CGIGetWindowPosition(CGIWindow* window){
    return window->position;
}

unsigned int CGIGetWindowHeight(CGIWindow* window){
    return window->height;
}

unsigned int CGIGetWindowwidth(CGIWindow* window){
    return window->width;
}

unsigned int CGIGetWindowBufferHeight(CGIWindow* window){
    return window->buffer_height;
}

unsigned int CGIGetWindowBufferWidth(CGIWindow* window){
    return window->buffer_width;
}


CGIColor_t CGIGetWindowBaseColor(CGIWindow* window){
    return window->CGIbase_color;
}

CGIPoint CGIGetWindowCursorPosition(CGIWindow* window){
    return window->cursor;
}

float CGIGetWindowScrollDeltaX(CGIWindow* window){
    return window->scroll_delta_x;
}

float CGIGetWindowScrollDeltaY(CGIWindow* window){
    return window->scroll_delta_y;
}

CGIBool CGIIsWindowScrolledX(CGIWindow* window){
    return window->is_scrolled_x;
}

CGIBool CGIIsWindowScrolledY(CGIWindow* window){
    return window->is_scrolled_y;
}