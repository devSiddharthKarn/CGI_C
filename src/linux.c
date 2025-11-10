// #ifdef __linux__
#define CGI_LINUX_IMPLEMENTATION_ACTIVE
// #endif


#include "cgi.h"
#include "X11/Xlib.h"
#include "X11/Xutil.h"
#include "X11/keysym.h"
#include "stdlib.h"

// CGIKey cgi_convert_x11_key(XKeyEvent *ev) {
//     KeySym sym = XLookupKeysym(ev, 0);

//     switch (sym) {
//         case XK_Escape: return CGI_KEY_ESCAPE;
//         case XK_Return: return CGI_KEY_ENTER;
//         case XK_space:  return CGI_KEY_SPACE;

//         case XK_BackSpace: return CGI_KEY_BACKSPACE;
//         case XK_Tab: return CGI_KEY_TAB;

//         case XK_Left:  return CGI_KEY_LEFT;
//         case XK_Right: return CGI_KEY_RIGHT;
//         case XK_Up: return CGI_KEY_UP;
//         case XK_Down: return CGI_KEY_DOWN;

//         case XK_Shift_L:
//         case XK_Shift_R: return CGI_KEY_SHIFT;

//         case XK_Control_L:
//         case XK_Control_R: return CGI_KEY_CTRL;

//         case XK_Alt_L:
//         case XK_Alt_R: return CGI_KEY_ALT;
//     }

//     // A-Z
//     if (sym >= XK_A && sym <= XK_Z)
//         return (CGI_KEY_A + (sym - XK_A));

//     // 0-9
//     if (sym >= XK_0 && sym <= XK_9)
//         return (CGI_KEY_0 + (sym - XK_0));

//     return CGI_KEY_UNKNOWN;
// }

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
};

struct SYSDISPLAY
{
    unsigned int width;
    unsigned int height;
    CGIPoint cursor;
};

struct CGIWindow
{
    char *name;
    struct WindowState windowState;
    struct SYSDISPLAY display;
    CGIPoint cursor;
    CGIColor_t CGIbase_color;
    CGIPoint position;
    unsigned int width;
    unsigned int height;
    struct CGIFrameBuffer buffer;
    unsigned int buffer_width;
    unsigned int buffer_height;
    CGIBool open;
};

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
        return CGI_false;
    }

    return CGI_true;
}

void DestoryFrameBuffer(CGIWindow *window)
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

CGIBool CGIBufferClear(CGIWindow *window, CGIColor_t color)
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

void CGIPresentBuffer(CGIWindow *window)
{
    if (!window || !window->buffer.pixels || !window->buffer.image)
        return;

    XPutImage(window->windowState.display, window->windowState.window, window->windowState.gc, window->buffer.image, 0, 0, 0, 0, window->buffer.width, window->buffer.height);

    XFlush(window->windowState.display);
}

CGIBool CGIResizeBuffer(CGIWindow *window)
{
    if (!window)
        return CGI_false;

    if (window->width == window->buffer.width && window->height == window->buffer.height)
    {
        return CGI_true;
    }

    DestoryFrameBuffer(window);
    return initialize_cpu_buffer(window);
}

CGIBool CGIWindowCleanup(CGIWindow *window)
{
    if(!window) return CGI_false;

    DestoryFrameBuffer(window);

    if(window->windowState.gc){
        XFreeGC(window->windowState.display,window->windowState.gc);
    }

    if(window->windowState.window){
        XDestroyWindow(window->windowState.display,window->windowState.window);
    }

    if(window->windowState.display){
        XCloseDisplay(window->windowState.display);
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
    Window child;
    XTranslateCoordinates(window->windowState.display, window->windowState.window, RootWindow(window->windowState.display, window->windowState.screen), 0, 0, &window->position.x, &window->position.y, &child);
    return;
}

void set_window_display_attrs(CGIWindow *window)
{
    window->display.width = DisplayWidth(window->windowState.display, window->windowState.screen);
    window->display.height = DisplayHeight(window->windowState.display, window->windowState.screen);

    Window root_window, child_window;
    int root_x;
    int root_y;
    int win_x;
    int win_y;
    unsigned int mask;

    XQueryPointer(window->windowState.display, window->windowState.window, &root_window, &child_window, &root_x, &root_y, &win_x, &win_y, &mask);

    window->display.cursor.x = root_x;
    window->display.cursor.y = root_y;
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
    XStoreName(window->windowState.display, window->windowState.window, window_name);
    window->windowState.gc = XCreateGC(window->windowState.display, window->windowState.window, 0, NULL);

    if (!window->windowState.window || !window->windowState.gc)
    {
        CGIWindowCleanup(window);
        return NULL;
    }

    window->windowState.classname = classname;
    window->name = window_name;
    window->position.x = x_pos;
    window->position.y = y_pos;
    set_width_and_height(window);
    initialize_cpu_buffer(window);
    CGIBufferClear(window, color);
    window->open = CGI_false;

    return window;
}

CGIBool CGIShowWindow(CGIWindow *window)
{
    XMapWindow(window->windowState.display, window->windowState.window);
    XFlush(window->windowState.display);
    set_width_and_height(window);
    set_window_pos_on_screen(window);
    set_window_display_attrs(window);

    window->open = CGI_true;
    return CGI_true;
}

CGIBool CGIIsWindowOpen(const CGIWindow *window)
{
    return window->open;
}

// CGIBool CGIIsKeyPressed(CGIKey key)
// {
// }

void process_events(CGIWindow* window,XEvent* event)
{   
    if(event->type==ConfigureNotify){
        window->position.x = event->xconfigure.x;
        window->position.y = event->xconfigure.y;
        set_window_pos_on_screen(window);
        set_width_and_height(window);
        set_window_display_attrs(window);
        CGIResizeBuffer(window);
    }


    // set_window_display_attrs(window);
}

CGIBool CGIRefreshWindow(CGIWindow *window)
{
    while (XPending(window->windowState.display))
    {
        XNextEvent(window->windowState.display, &window->windowState.event);
        process_events(window,&window->windowState.event);
    }

    CGIPresentBuffer(window);
    return CGI_true;
}

const void *CGIQueryWindow(CGIQuery query, CGIWindow *window)
{
    if (!window)
        return NULL;

    switch (query)
    {
    case CGI_query_system_cursor_position:
    {
        return &window->display.cursor;
    }
    case CGI_query_window_cursor_position:
    {
        return &window->cursor;
    }
    case CGI_query_window_width:
    {
        return &window->width;
    }
    case CGI_query_window_height:
    {
        return &window->height;
    }
    case CGI_query_window_buffer_width:
    {
        return &window->buffer_width;
    }
    case CGI_query_window_buffer_height:
    {
        return &window->buffer_height;
    }
    case CGI_query_window_position:
    {
        return &window->position;
    }
    case CGI_query_window_open_status:
    {
        return &window->open;
    }
    case CGI_query_system_display_height:
    {
        return &window->display.height;
    }
    case CGI_query_system_display_width:
    {
        return &window->display.width;
    }
    case CGI_query_window_internal_linux_Xlib_DisplayPointer:
    {
        return window->windowState.display;
    }
    case CGI_query_window_internal_linux_Xlib_screen:
    {
        return &window->windowState.screen;
    }
    case CGI_query_window_internal_linux_Xlib_window:
    {
        return &window->windowState.window;
    }
    case CGI_query_window_internal_linux_Xlib_colormap:
    {
        return &window->windowState.colormap;
    }
    case CGI_query_window_internal_linux_Xlib_GC:
    {
        return &window->windowState.gc;
    }
    case CGI_query_window_base_color:
    {
        return &window->CGIbase_color;
    }
    case CGI_query_window_internal_linux_Xlib_type_base_color:
    {
        return &window->windowState.base_color;
    }
    // case CGI_query_system_cursor_position:{
    //     return &window->display.cursor
    // }

    default:
        break;
    }
    return NULL;
}
