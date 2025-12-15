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

typedef enum
{
    CGI_window_refresh_mode_rapid = 1,
    CGI_window_refresh_mode_triggered
} CGIWindowRefreshMode;

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

typedef unsigned long long CGIKeyCode;

#define CGI_KEYCODE_NONE ((CGIKeyCode)0)
#define CGI_KEYCODE_A ((CGIKeyCode)(1ULL << 0))
#define CGI_KEYCODE_B ((CGIKeyCode)(1ULL << 1))
#define CGI_KEYCODE_C ((CGIKeyCode)(1ULL << 2))
#define CGI_KEYCODE_D ((CGIKeyCode)(1ULL << 3))
#define CGI_KEYCODE_E ((CGIKeyCode)(1ULL << 4))
#define CGI_KEYCODE_F ((CGIKeyCode)(1ULL << 5))
#define CGI_KEYCODE_G ((CGIKeyCode)(1ULL << 6))
#define CGI_KEYCODE_H ((CGIKeyCode)(1ULL << 7))
#define CGI_KEYCODE_I ((CGIKeyCode)(1ULL << 8))
#define CGI_KEYCODE_J ((CGIKeyCode)(1ULL << 9))
#define CGI_KEYCODE_K ((CGIKeyCode)(1ULL << 10))
#define CGI_KEYCODE_L ((CGIKeyCode)(1ULL << 11))
#define CGI_KEYCODE_M ((CGIKeyCode)(1ULL << 12))
#define CGI_KEYCODE_N ((CGIKeyCode)(1ULL << 13))
#define CGI_KEYCODE_O ((CGIKeyCode)(1ULL << 14))
#define CGI_KEYCODE_P ((CGIKeyCode)(1ULL << 15))
#define CGI_KEYCODE_Q ((CGIKeyCode)(1ULL << 16))
#define CGI_KEYCODE_R ((CGIKeyCode)(1ULL << 17))
#define CGI_KEYCODE_S ((CGIKeyCode)(1ULL << 18))
#define CGI_KEYCODE_T ((CGIKeyCode)(1ULL << 19))
#define CGI_KEYCODE_U ((CGIKeyCode)(1ULL << 20))
#define CGI_KEYCODE_V ((CGIKeyCode)(1ULL << 21))
#define CGI_KEYCODE_W ((CGIKeyCode)(1ULL << 22))
#define CGI_KEYCODE_X ((CGIKeyCode)(1ULL << 23))
#define CGI_KEYCODE_Y ((CGIKeyCode)(1ULL << 24))
#define CGI_KEYCODE_Z ((CGIKeyCode)(1ULL << 25))
#define CGI_KEYCODE_0 ((CGIKeyCode)(1ULL << 26))
#define CGI_KEYCODE_1 ((CGIKeyCode)(1ULL << 27))
#define CGI_KEYCODE_2 ((CGIKeyCode)(1ULL << 28))
#define CGI_KEYCODE_3 ((CGIKeyCode)(1ULL << 29))
#define CGI_KEYCODE_4 ((CGIKeyCode)(1ULL << 30))
#define CGI_KEYCODE_5 ((CGIKeyCode)(1ULL << 31))
#define CGI_KEYCODE_6 ((CGIKeyCode)(1ULL << 32))
#define CGI_KEYCODE_7 ((CGIKeyCode)(1ULL << 33))
#define CGI_KEYCODE_8 ((CGIKeyCode)(1ULL << 34))
#define CGI_KEYCODE_9 ((CGIKeyCode)(1ULL << 35))
#define CGI_KEYCODE_ESCAPE ((CGIKeyCode)(1ULL << 36))
#define CGI_KEYCODE_ENTER ((CGIKeyCode)(1ULL << 37))
#define CGI_KEYCODE_SPACE ((CGIKeyCode)(1ULL << 38))
#define CGI_KEYCODE_UP ((CGIKeyCode)(1ULL << 39))
#define CGI_KEYCODE_DOWN ((CGIKeyCode)(1ULL << 40))
#define CGI_KEYCODE_LEFT ((CGIKeyCode)(1ULL << 41))
#define CGI_KEYCODE_RIGHT ((CGIKeyCode)(1ULL << 42))
#define CGI_KEYCODE_BACKSPACE ((CGIKeyCode)(1ULL << 43))
#define CGI_KEYCODE_SHIFT ((CGIKeyCode)(1ULL << 44))
#define CGI_KEYCODE_CTRL ((CGIKeyCode)(1ULL << 45))
#define CGI_KEYCODE_ALT ((CGIKeyCode)(1ULL << 46))
#define CGI_KEYCODE_F1 ((CGIKeyCode)(1ULL << 47))
#define CGI_KEYCODE_F2 ((CGIKeyCode)(1ULL << 48))
#define CGI_KEYCODE_F3 ((CGIKeyCode)(1ULL << 49))
#define CGI_KEYCODE_F4 ((CGIKeyCode)(1ULL << 50))
#define CGI_KEYCODE_F5 ((CGIKeyCode)(1ULL << 51))
#define CGI_KEYCODE_F6 ((CGIKeyCode)(1ULL << 52))
#define CGI_KEYCODE_F7 ((CGIKeyCode)(1ULL << 53))
#define CGI_KEYCODE_F8 ((CGIKeyCode)(1ULL << 54))
#define CGI_KEYCODE_F9 ((CGIKeyCode)(1ULL << 55))
#define CGI_KEYCODE_F10 ((CGIKeyCode)(1ULL << 56))
#define CGI_KEYCODE_F11 ((CGIKeyCode)(1ULL << 57))
#define CGI_KEYCODE_F12 ((CGIKeyCode)(1ULL << 58))
#define CGI_KEYCODE_MOUSE_L ((CGIKeyCode)(1ULL << 59))
#define CGI_KEYCODE_MOUSE_R ((CGIKeyCode)(1ULL << 60))

typedef struct KeyboardEvent
{
    CGIKeyCode up_keys;
    CGIKeyCode down_keys;
} CGIKeyboardEvent;

typedef struct CursorEvent
{
    int system_x;
    int system_y;
    int win_x;
    int win_y;
    CGIKeyCode down_buttons;
    CGIKeyCode up_buttons;
} CGICursorEvent;

typedef struct WindowEvent
{
    float scroll_delta_x;
    float scroll_delta_y;
    CGIBool is_scrolled_x;
    CGIBool is_scrolled_y;
    CGIBool is_focused;
    CGIBool is_resized;
    CGIBool is_moved;
} CGIWindowEvent;

typedef struct EventImage
{
    CGIKeyboardEvent keyboardEvent;
    CGICursorEvent cursorEvent;
    CGIWindowEvent windowEvent;
} CGIEventImage;




//CGIWindowSurface represents the surface buffer of a CGI window i.e. the current pixel data buffer of the window
typedef struct CGIWindowSurface{
    //height of the window surface buffer
    unsigned int height;
    //width of the window surface buffer
    unsigned int width;
    
    //channels
    unsigned int channels;

    //pointer to the actual buffer data (const void *)
    void *buffer;
}CGIWindowSurface;
