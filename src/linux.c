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

KeySym  CGIInputKeyToKeySym(CGIInputKey key){
    switch(key){
        case CGI_input_key_a: return XK_a;
        case CGI_input_key_b: return XK_b;
        case CGI_input_key_c: return XK_c;
        case CGI_input_key_d: return XK_d;
        case CGI_input_key_e: return XK_e;
        case CGI_input_key_f: return XK_f;
        case CGI_input_key_g: return XK_g;
        case CGI_input_key_h: return XK_h;
        case CGI_input_key_i: return XK_i;
        case CGI_input_key_j: return XK_j;
        case CGI_input_key_k: return XK_k;
        case CGI_input_key_l: return XK_l;
        case CGI_input_key_m: return XK_m;
        case CGI_input_key_n: return XK_n;
        case CGI_input_key_o: return XK_o;
        case CGI_input_key_p: return XK_p;
        case CGI_input_key_q: return XK_q;
        case CGI_input_key_r: return XK_r;
        case CGI_input_key_s: return XK_s;
        case CGI_input_key_t: return XK_t;
        case CGI_input_key_u: return XK_u;
        case CGI_input_key_v: return XK_v;
        case CGI_input_key_w: return XK_w;
        case CGI_input_key_x: return XK_x;
        case CGI_input_key_y: return XK_y;
        case CGI_input_key_z: return XK_z;
        case CGI_input_key_0: return XK_0;
        case CGI_input_key_1: return XK_1;
        case CGI_input_key_2: return XK_2;
        case CGI_input_key_3: return XK_3;
        case CGI_input_key_4: return XK_4;
        case CGI_input_key_5: return XK_5;
        case CGI_input_key_6: return XK_6;
        case CGI_input_key_7: return XK_7;
        case CGI_input_key_8: return XK_8;
        case CGI_input_key_9: return XK_9;
        case CGI_input_key_f1: return XK_F1;
        case CGI_input_key_f2: return XK_F2;
        case CGI_input_key_f3: return XK_F3;
        case CGI_input_key_f4: return XK_F4;
        case CGI_input_key_f5: return XK_F5;
        case CGI_input_key_f6: return XK_F6;
        case CGI_input_key_f7: return XK_F7;
        case CGI_input_key_f8: return XK_F8;
        case CGI_input_key_f9: return XK_F9;
        case CGI_input_key_f10: return XK_F10;
        case CGI_input_key_f11: return XK_F11;
        case CGI_input_key_f12: return XK_F12;
        case CGI_input_key_up: return XK_Up;
        case CGI_input_key_down: return XK_Down;
        case CGI_input_key_left: return XK_Left;
        case CGI_input_key_right: return XK_Right;
        case CGI_input_key_enter: return XK_Return;
        case CGI_input_key_escape: return XK_Escape;
        case CGI_input_key_space: return XK_space;
        case CGI_input_key_backspace: return XK_BackSpace;
        case CGI_input_key_shift: return XK_Shift_L;
        case CGI_input_key_ctrl: return XK_Control_L;
        case CGI_input_key_alt: return XK_Alt_L;
        
        default: return NoSymbol;
    }

    return NoSymbol;
}


CGIBool CGIIsKeyPressed(CGIWindow* window,CGIInputKey key){
    char keys[32];

    XQueryKeymap(window->windowState.display,keys);

    KeySym keysym = CGIInputKeyToKeySym(key);
    if(keysym==NoSymbol) return CGI_false;

    KeyCode code = XKeysymToKeycode(window->windowState.display,keysym);
    if(code ==0) return CGI_false;

    if(keys[code/8] & (1<<(code%8))){
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

    XSelectInput(window->windowState.display,window->windowState.window,StructureNotifyMask);

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


CGIBool CGIIsWindowFocused(CGIWindow* window){
    int revert;
    Window win;
    XGetInputFocus(window->windowState.display,&win,&revert);

    if(win==window->windowState.window){
        return CGI_true;
    }

    return CGI_false;
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
