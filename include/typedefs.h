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

/// @brief structure to CGI's internal color type contains r ,g,b channels
typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} CGIColor_t;

/// @brief CGI logical bools
typedef enum
{
    CGI_false = 0,
    CGI_true = 1
} CGIBool;

/// @brief structure to represent 2d pixel points (type int)
typedef struct
{
    int x;
    int y;
} CGIPoint;

typedef struct
{
    CGIPoint point;
    CGIColor_t color;
} CGIPixel;

typedef enum
{
    CGI_query_faulty_NULL,
    CGI_query_window_name_charPointer,
    CGI_query_window_position_CGIPoint,
    CGI_query_window_height_unsigned_int,
    CGI_query_window_width_unsigned_int,
    CGI_query_window_buffer_height_unsigned_int,
    CGI_query_window_buffer_width_unsigned_int,
    CGI_query_window_base_color_CGIColor_t,
    CGI_query_window_open_status_CGIBool,
    CGI_query_window_cursor_position_CGIPoint,
    CGI_query_window_focus_status_CGIBool,
    CGI_query_window_scroll_delta_x_float,
    CGI_query_window_scroll_delta_y_float,
    CGI_query_window_is_scrolled_x_CGIBool,
    CGI_query_window_is_scrolled_y_CGIBool,

    CGI_query_system_display_width_unsigned_int,
    CGI_query_system_display_height_unsigned_int,
    CGI_query_system_display_refresh_rate_unsigned_int,
    CGI_query_system_display_physical_width_unsigned_int,
    CGI_query_system_display_physical_height_unsigned_int,
    CGI_query_system_cursor_position_CGIPoint,
    CGI_query_system_l_button_pressed_CGIBool,
    CGI_query_system_r_button_pressed_CGIBool,

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
    CGI_query_window_internal_linux_Xlib_screen_int,
    CGI_query_window_internal_linux_Xlib_window_Window,
    CGI_query_window_internal_linux_Xlib_colormap_Colormap,
    CGI_query_window_internal_linux_Xlib_GC,
    CGI_query_window_internal_linux_Xlib_type_base_color_XColor,
#endif

} CGIQuery;

typedef enum
{
    CGI_command_faulty_NULL,

    // CGI_struct_commands
    CGI_command_CGI_start,
    CGI_command_CGI_update,
    CGI_command_CGI_end,
    CGI_command_CGI_set_cursor_position,

    // window commands
    //  CGI_command_window_create_window,
    CGI_command_window_cleanup,
    CGI_command_window_set_pixel,
    CGI_command_window_refresh_window,
    CGI_command_window_refresh_buffer,
    CGI_command_window_clear_buffer,
    CGI_command_window_set_window_title,
    CGI_command_window_set_window_pos,
    CGI_command_window_set_window_size,
    CGI_command_window_set_window_base_color,
    CGI_command_window_set_window_show_status,
    CGI_command_window_set_focus_status,
    CGI_command_window_close,
    CGI_command_window_resizable_logic,
    CGI_command_window_minimizable_logic,
    CGI_command_window_maximizable_logic,

} CGICommand;

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
