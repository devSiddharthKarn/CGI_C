typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
}CGIColor_t;

typedef enum {
    CGI_false=0,
    CGI_true=1
}CGIBool;

typedef struct{
    int x ;
    int y;
}CGIPoint;


typedef enum{
    CGI_query_faulty,
    CGI_query_window_name,
    CGI_query_window_position,
    CGI_query_window_height,
    CGI_query_window_width,
    CGI_query_window_buffer_height,
    CGI_query_window_buffer_width,
    CGI_query_window_base_color,
    CGI_query_window_open_status,
    CGI_query_window_cursor_position,

    #ifdef CGI_WINDOW_IMPLEMENTATION_ACTIVE
    ///@brief window specific
    CGI_query_window_internal_win32_HWND,
    CGI_query_window_internal_win32_HDC,
    CGI_query_window_internal_win32_WNDCLASSA,
    CGI_query_window_internal_win32_PAINTSTRUCT,
    CGI_query_window_internal_win32_BITMAPINFO,
    CGI_query_window_internal_win32_MSG,
    #endif

    #ifdef CGI_LINUX_IMPLEMENTATION_ACTIVE
    ///@brief linux specific implementation
    CGI_query_window_internal_linux_Xlib_DisplayPointer,
    CGI_query_window_internal_linux_Xlib_screen,
    CGI_query_window_internal_linux_Xlib_window,
    CGI_query_window_internal_linux_Xlib_colormap,
    CGI_query_window_internal_linux_Xlib_GC,
    CGI_query_window_internal_linux_Xlib_type_base_color,
    #endif

    ///@brief system-apis
    CGI_query_system_display_width,
    CGI_query_system_display_height,
    CGI_query_system_cursor_position,
    // CGI_query_window_cursor_position,

    ///@brief system queries
    


}CGIQuery;


typedef struct CGIWindow CGIWindow;

CGIWindow *CGICreateWindow(char *classname, char *window_name, unsigned int x_pos, unsigned int y_pos, unsigned int width, unsigned int height, CGIColor_t color);

CGIBool CGIWindowCleanup(CGIWindow *window);

CGIBool CGIShowWindow(CGIWindow *window);

void CGISetPixel(CGIWindow *window, int x_pos, int y_pos, CGIColor_t color);

const void *CGIQueryWindow(CGIQuery query, CGIWindow *window);

CGIBool CGIIsWindowOpen(const CGIWindow *window);

CGIColor_t CGIMakeColor(unsigned char r, unsigned char g, unsigned char b);

CGIBool CGIRefreshWindow(CGIWindow *window);

CGIBool CGIRefreshBuffer(CGIWindow *window);

CGIBool CGIBufferClear(CGIWindow *window, CGIColor_t color);
