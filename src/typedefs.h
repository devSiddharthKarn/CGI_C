#ifndef CGI_TYPEDEFS_H
#define CGI_TYPEDEFS_H
#endif

#pragma once

#ifdef CGI_IMPLEMENTATION_MODE_VERBOSE
    #ifdef _WIN32
        #define CGI_WINDOWS_IMPLEMENTATION_ACTIVE
    #elif __linux__
        #define CGI_LINUX_IMPLEMENTATION_ACTIVE
    #endif
#endif






typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} CGIColor_t;

typedef enum
{
    CGI_false = 0,
    CGI_true = 1
} CGIBool;

typedef struct
{
    int x;
    int y;
} CGIPoint;



struct  CGI
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
        int scroll_delta;
        CGIBool is_scrolling;
    } Cursor;

};




typedef enum
{
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
    CGI_query_window_focus_status,


    CGI_query_system_display_width,
    CGI_query_system_display_height,
    CGI_query_system_display_refresh_rate,
    CGI_query_system_display_physical_width,
    CGI_query_system_display_physical_height,
    CGI_query_system_cursor_position,
    CGI_query_system_l_button_pressed,
    CGI_query_system_r_button_pressed,
    CGI_query_system_scroll_delta,
    CGI_query_system_is_scrolling,

    

#ifdef CGI_WINDOWS_IMPLEMENTATION_ACTIVE
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

    

    
}CGIQuery;

typedef enum
{
    CGI_input_key_up,
    CGI_input_key_down,
    CGI_input_key_left,
    CGI_input_key_right,
    CGI_input_key_enter,
    CGI_input_key_escape,
    CGI_input_key_space,
    CGI_input_key_backspace,
    CGI_input_key_shift,
    CGI_input_key_ctrl,
    CGI_input_key_alt,
    CGI_input_key_a,
    CGI_input_key_b,
    CGI_input_key_c,
    CGI_input_key_d,
    CGI_input_key_e,
    CGI_input_key_f,
    CGI_input_key_g,
    CGI_input_key_h,
    CGI_input_key_i,
    CGI_input_key_j,
    CGI_input_key_k,
    CGI_input_key_l,
    CGI_input_key_m,
    CGI_input_key_n,
    CGI_input_key_o,
    CGI_input_key_p,
    CGI_input_key_q,
    CGI_input_key_r,
    CGI_input_key_s,
    CGI_input_key_t,
    CGI_input_key_u,
    CGI_input_key_v,
    CGI_input_key_w,
    CGI_input_key_x,
    CGI_input_key_y,
    CGI_input_key_z,
    CGI_input_key_0,
    CGI_input_key_1,
    CGI_input_key_2,
    CGI_input_key_3,
    CGI_input_key_4,
    CGI_input_key_5,
    CGI_input_key_6,
    CGI_input_key_7,
    CGI_input_key_8,
    CGI_input_key_9,
    CGI_input_key_f1,
    CGI_input_key_f2,
    CGI_input_key_f3,
    CGI_input_key_f4,
    CGI_input_key_f5,
    CGI_input_key_f6,
    CGI_input_key_f7,
    CGI_input_key_f8,
    CGI_input_key_f9,
    CGI_input_key_f10,
    CGI_input_key_f11,
    CGI_input_key_f12,
} CGIInputKey;

// typedef enum
// {
//     CGI_window_hint_default = 0,
//     CGI_window_hint_centered = 1 << 0,
//     CGI_window_hint_resizable = 1 << 1,
//     CGI_window_hint_minimizable = 1 << 2,
//     CGI_window_hint_maximizable = 1 << 3,
//     CGI_window_hint_decorated = 1 << 4,
//     CGI_window_hint_visible = 1 << 5,
//     CGI_window_hint_on_top = 1 << 6,
//     CGI_window_hint_on_bottom = 1 << 7,
//     CGI_window_hint_fullscreen = 1 << 8,

// } CGIWindowHints; // remaining to implement