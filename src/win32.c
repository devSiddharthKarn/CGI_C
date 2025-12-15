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

#define CGI_WINDOWS_IMPLEMENTATION_ACTIVE

/*
include headers for windows implementation
source file: win32.c
*/

#include "Windows.h"
#include "../include/cgi.h"
#include "stdlib.h"
#include "string.h"

CGIColor_t CGIMakeColor(unsigned char r, unsigned char g, unsigned char b)
{
    CGIColor_t color;
    color.r = r;
    color.g = g;
    color.b = b;
    return color;
}

/// @brief Convert CGIInputKey to Windows Virtual-Key code
/// @param key The CGIInputKey to convert
/// @return Corresponding Windows Virtual-Key code, or 0 if not found

int CGIKeyToWin32VKKey(CGIKeyCode key)
{
    switch (key)
    {
    case CGI_KEYCODE_A:
        return 'A';
    case CGI_KEYCODE_B:
        return 'B';
    case CGI_KEYCODE_C:
        return 'C';
    case CGI_KEYCODE_D:
        return 'D';
    case CGI_KEYCODE_E:
        return 'E';
    case CGI_KEYCODE_F:
        return 'F';
    case CGI_KEYCODE_G:
        return 'G';
    case CGI_KEYCODE_H:
        return 'H';
    case CGI_KEYCODE_I:
        return 'I';
    case CGI_KEYCODE_J:
        return 'J';
    case CGI_KEYCODE_K:
        return 'K';
    case CGI_KEYCODE_L:
        return 'L';
    case CGI_KEYCODE_M:
        return 'M';
    case CGI_KEYCODE_N:
        return 'N';
    case CGI_KEYCODE_O:
        return 'O';
    case CGI_KEYCODE_P:
        return 'P';
    case CGI_KEYCODE_Q:
        return 'Q';
    case CGI_KEYCODE_R:
        return 'R';
    case CGI_KEYCODE_S:
        return 'S';
    case CGI_KEYCODE_T:
        return 'T';
    case CGI_KEYCODE_U:
        return 'U';
    case CGI_KEYCODE_V:
        return 'V';
    case CGI_KEYCODE_W:
        return 'W';
    case CGI_KEYCODE_X:
        return 'X';
    case CGI_KEYCODE_Y:
        return 'Y';
    case CGI_KEYCODE_Z:
        return 'Z';
    case CGI_KEYCODE_0:
        return '0';
    case CGI_KEYCODE_1:
        return '1';
    case CGI_KEYCODE_2:
        return '2';
    case CGI_KEYCODE_3:
        return '3';
    case CGI_KEYCODE_4:
        return '4';
    case CGI_KEYCODE_5:
        return '5';
    case CGI_KEYCODE_6:
        return '6';
    case CGI_KEYCODE_7:
        return '7';
    case CGI_KEYCODE_8:
        return '8';
    case CGI_KEYCODE_9:
        return '9';
    case CGI_KEYCODE_F1:
        return VK_F1;
    case CGI_KEYCODE_F2:
        return VK_F2;
    case CGI_KEYCODE_F3:
        return VK_F3;
    case CGI_KEYCODE_F4:
        return VK_F4;
    case CGI_KEYCODE_F5:
        return VK_F5;
    case CGI_KEYCODE_F6:
        return VK_F6;
    case CGI_KEYCODE_F7:
        return VK_F7;
    case CGI_KEYCODE_F8:
        return VK_F8;
    case CGI_KEYCODE_F9:
        return VK_F9;
    case CGI_KEYCODE_F10:
        return VK_F10;
    case CGI_KEYCODE_F11:
        return VK_F11;
    case CGI_KEYCODE_F12:
        return VK_F12;
    case CGI_KEYCODE_ESCAPE:
        return VK_ESCAPE;
    case CGI_KEYCODE_ENTER:
        return VK_RETURN;
    case CGI_KEYCODE_SPACE:
        return VK_SPACE;
    case CGI_KEYCODE_UP:
        return VK_UP;
    case CGI_KEYCODE_DOWN:
        return VK_DOWN;
    case CGI_KEYCODE_LEFT:
        return VK_LEFT;
    case CGI_KEYCODE_RIGHT:
        return VK_RIGHT;
    case CGI_KEYCODE_BACKSPACE:
        return VK_BACK;
    case CGI_KEYCODE_SHIFT:
        return VK_SHIFT;
    case CGI_KEYCODE_CTRL:
        return VK_CONTROL;
    case CGI_KEYCODE_ALT:
        return VK_MENU;

    default:
        break;
    }

    return 0;
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

/// @brief Definition of the WindowState structure for Windows implementation
/// Contains internal Windows-specific window data
struct WindowState
{
    HWND hwnd;
    HDC hdc;
    WNDCLASSA wc;
    COLORREF *buffer;
    unsigned int buffer_width;
    unsigned int buffer_height;
    PAINTSTRUCT ps;
    HDC offscreenBuffer;
    BITMAPINFO bmi;
    HBITMAP hBitMap;
    void *pixelbuffer;
    MSG msg;
};

/// @brief Definition of the CGIWindow structure for Windows implementation
/// Contains window properties and internal state
struct CGIWindow
{
    struct WindowState windowState;
    char *window_name;
    CGIPoint position;
    CGIPoint cursor;
    unsigned int width;
    unsigned int height;
    COLORREF win_base_color;
    CGIColor_t base_color;
    CGIEventImage event;
    
    CGIBool open;
    
};

/// @brief Cleanup and free resources associated with a CGIWindow
/// @param window Pointer to the CGIWindow to be cleaned up
/// @return CGI_true if successful, otherwise CGI_false
CGIBool CGIWindowCleanup(CGIWindow *window)
{
    if (!window)
        return CGI_false;

    if (window->windowState.hBitMap)
    {
        DeleteObject(window->windowState.hBitMap);
        window->windowState.hBitMap = NULL;
    }

    if (window->windowState.offscreenBuffer)
    {
        DeleteDC(window->windowState.offscreenBuffer);
        window->windowState.offscreenBuffer = NULL;
    }

    if (window->windowState.hwnd)
    {
        if (window->windowState.hdc)
        {
            ReleaseDC(window->windowState.hwnd, window->windowState.hdc);
            window->windowState.hdc = NULL;
        }

        DestroyWindow(window->windowState.hwnd);
        UnregisterClassA(window->windowState.wc.lpszClassName, window->windowState.wc.hInstance);
        window->windowState.hwnd = NULL;
    }

    

    if (window->window_name)
    {
        free(window->window_name);
        window->window_name = NULL;
    }

    if (window->windowState.buffer)
    {
        free(window->windowState.buffer);
        window->windowState.buffer = NULL;
    }

    free(window);

    return CGI_true;
}

/// @brief Initialize a BITMAPINFO structure for the given window and dimensions
/// @param window Pointer to the CGIWindow
/// @param bmi Pointer to the BITMAPINFO structure to initialize
/// @param height Height of the bitmap
/// @param width Width of the bitmap
/// @return CGI_true if successful, otherwise CGI_false
CGIBool MakeBMI(CGIWindow *window, BITMAPINFO *bmi, unsigned int height, unsigned int width)
{
    if (bmi == NULL || window == NULL)
        return CGI_false;

    ZeroMemory(bmi, sizeof(BITMAPINFO));
    bmi->bmiHeader.biBitCount = 24;
    bmi->bmiHeader.biCompression = BI_RGB;
    bmi->bmiHeader.biHeight = -(int)height; // top down view
    bmi->bmiHeader.biPlanes = 1;
    bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi->bmiHeader.biWidth = width;

    if (window->windowState.hBitMap)
    {
        SelectObject(window->windowState.offscreenBuffer, GetStockObject(NULL_BRUSH));
        DeleteObject(window->windowState.hBitMap);
        window->windowState.hBitMap = NULL;
    }

    if (window->windowState.offscreenBuffer)
    {
        DeleteDC(window->windowState.offscreenBuffer);
        window->windowState.offscreenBuffer = NULL;
    }

    window->windowState.offscreenBuffer = CreateCompatibleDC(window->windowState.hdc);
    if (!window->windowState.offscreenBuffer)
        return CGI_false;

    window->windowState.hBitMap = CreateDIBSection(window->windowState.offscreenBuffer,
                                                   &window->windowState.bmi,
                                                   DIB_RGB_COLORS,
                                                   &window->windowState.pixelbuffer,
                                                   NULL, 0);
    if (!window->windowState.hBitMap)
    {
        DeleteDC(window->windowState.offscreenBuffer);
        window->windowState.offscreenBuffer = NULL;
        return CGI_false;
    }

    SelectObject(window->windowState.offscreenBuffer, window->windowState.hBitMap);

    return CGI_true;
}

/// @brief Convert the pixel buffer to a COLORREF buffer for the window
/// @param pixelBuffer Pointer to the pixel buffer
/// @param window_main_buffer Pointer to the window's main buffer
/// @param width Width of the buffer
/// @param height Height of the buffer
/// @return CGI_true if successful, otherwise CGI_false
CGIBool LoadBufferView(CGIWindow *window, int x_pos, int y_pos, int width, int height)
{

    void *pixelBuffer = window->windowState.pixelbuffer;
    COLORREF *window_main_buffer = window->windowState.buffer;

    if (!pixelBuffer || !window_main_buffer)
        return CGI_false;

    unsigned char *buf = (unsigned char *)pixelBuffer;

    int rowSize = (window->windowState.buffer_width * 3 + 3) & ~3; // align scanline to 4 bytes

    for (int y = y_pos; y < height; y++)
    {
        unsigned char *row = buf + y * rowSize;

        for (int x = x_pos; x < width; x++)
        {

            int srcPos = y * window->windowState.buffer_width + x;

            unsigned char r = GetRValue(window_main_buffer[srcPos]);
            unsigned char g = GetGValue(window_main_buffer[srcPos]);
            unsigned char b = GetBValue(window_main_buffer[srcPos]);

            int offset = x * 3;
            row[offset + 0] = b;
            row[offset + 1] = g;
            row[offset + 2] = r;
        }
    }

    return CGI_true;
}

void CGIRefreshBufferRegion(CGIWindow *window, int x, int y, int width, int height)
{
    int bw = window->windowState.buffer_width;
    int bh = window->windowState.buffer_height;

    // Completely outside → return
    if (x >= bw || y >= bh || x + width <= 0 || y + height <= 0)
        return;

    // ---- CLIP THE REGION (very important) ----

    int clippedX = x < 0 ? 0 : x;
    int clippedY = y < 0 ? 0 : y;

    int clippedW = (x + width > bw) ? (bw - clippedX) : (width - (clippedX - x));
    int clippedH = (y + height > bh) ? (bh - clippedY) : (height - (clippedY - y));

    // Convert pixels from logical window buffer → pixelBuffer
    LoadBufferView(window, clippedX, clippedY, clippedW, clippedH);

    // Copy the correct region from offscreenBuffer → window DC
    BitBlt(window->windowState.hdc,
           clippedX, clippedY, clippedW, clippedH,
           window->windowState.offscreenBuffer,
           clippedX, clippedY,
           SRCCOPY);

    return;
}

/// @brief Windows procedure callback function to handle window messages
/// @param hwnd Handle to the window
/// @param msg Message identifier
/// @param wp Additional message information (WPARAM)
/// @param lp Additional message information (LPARAM)
/// @return Result of message processing (LRESULT)
LRESULT CALLBACK windows_procedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    CGIWindow *window = (CGIWindow *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (msg)
    {
    case WM_NCCREATE:
    {
        CREATESTRUCT *cs = (CREATESTRUCT *)lp;
        window = (CGIWindow *)cs->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
        return DefWindowProcA(hwnd, msg, wp, lp);
    }

    case WM_PAINT:
    {
        if (!window)
            return DefWindowProcA(hwnd, msg, wp, lp);

        BeginPaint(hwnd, &window->windowState.ps);

        LoadBufferView(window, 0, 0,
                       window->windowState.buffer_width, window->windowState.buffer_height);

        BitBlt(window->windowState.hdc, 0, 0, window->windowState.buffer_width, window->windowState.buffer_height, window->windowState.offscreenBuffer, 0, 0, SRCCOPY);

        EndPaint(hwnd, &window->windowState.ps);

        // // HRGN rgn= CreateRectRgn(0,0,)
        // CombineRgn(,)

        return TRUE;
    }

    case WM_SIZE:
    {
        if (!window)
            return DefWindowProcA(hwnd, msg, wp, lp);

        RECT rect;
        GetWindowRect(window->windowState.hwnd, &rect);
        window->width = rect.right - rect.left;
        window->height = rect.bottom - rect.top;

        GetClientRect(window->windowState.hwnd, &rect);
        unsigned int new_width = rect.right - rect.left;
        unsigned int new_height = rect.bottom - rect.top;

        if (new_width != window->windowState.buffer_width ||
            new_height != window->windowState.buffer_height)
        {
            COLORREF *new_buffer = (COLORREF *)realloc(
                window->windowState.buffer,
                new_width * new_height * sizeof(COLORREF));

            if (new_buffer)
            {
                window->windowState.buffer = new_buffer;

                // Update to new dimensions
                window->windowState.buffer_width = new_width;
                window->windowState.buffer_height = new_height;

                // Clear new buffer with base color
                for (unsigned int i = 0; i < new_width * new_height; i++)
                {
                    window->windowState.buffer[i] = window->win_base_color;
                }

                MakeBMI(window, &window->windowState.bmi, new_height, new_width);

                // window->resized = CGI_true;
                window->event.windowEvent.is_resized = CGI_true;
            }
        }
        break;
    }

    case WM_CLOSE:
    {
        if (!window)
            return DefWindowProcA(hwnd, msg, wp, lp);

        window->open = CGI_false;
        DestroyWindow(hwnd);
        return TRUE;
    }

    case WM_MOVE:
    {
        if (!window)
            return DefWindowProcA(hwnd, msg, wp, lp);

        RECT rect;
        GetWindowRect(window->windowState.hwnd, &rect);
        window->position.x = rect.left;
        window->position.y = rect.top;

        window->event.windowEvent.is_moved = CGI_true;

        break;
    }

    case WM_MOUSEWHEEL:
    {
        int delta = GET_WHEEL_DELTA_WPARAM(wp);
        window->event.windowEvent.scroll_delta_y = (float)delta / 120;
        window->event.windowEvent.is_scrolled_y = CGI_true;
        break;
    }
    case WM_MOUSEHWHEEL:
    {
        int delta = GET_WHEEL_DELTA_WPARAM(wp);
        window->event.windowEvent.scroll_delta_x = (float)delta / 120;
        window->event.windowEvent.is_scrolled_x = CGI_true;
        break;
    }

    case WM_DESTROY:
    {
        if (!window)
            return DefWindowProcA(hwnd, msg, wp, lp);

        window->open = CGI_false;
        PostQuitMessage(0);
        return TRUE;
    }

    default:
    {
        return DefWindowProcA(hwnd, msg, wp, lp);
    }
    }
    return TRUE;
}

/// @brief Create a new window with the specified parameters
/// @param classname classname of the window although some OS may ignore it
/// @param window_name Name of the window
/// @param x_pos X position of the window
/// @param y_pos Y position of the window
/// @param width Width of the window
/// @param height Height of the window
/// @param color Base color of the window
/// @return Pointer to the created CGIWindow, or NULL on failure
CGIWindow *CGICreateWindow(char *classname, char *window_name, unsigned int x_pos, unsigned int y_pos, unsigned int width, unsigned int height, CGIColor_t color)
{
    CGIWindow *window = (CGIWindow *)malloc(sizeof(CGIWindow));
    if (!window)
        return NULL;

    memset(window, 0, sizeof(CGIWindow));

    window->window_name = _strdup(window_name);
    if (!window->window_name)
    {
        free(window);
        return NULL;
    }

    window->position.x = x_pos;
    window->position.y = y_pos;
    window->height = height;
    window->width = width;
    window->win_base_color = RGB(color.r, color.g, color.b);
    window->base_color = color;
    window->open = CGI_false;

    window->event.windowEvent.is_focused = CGI_false;
    window->event.windowEvent.is_moved = CGI_false;
    window->event.windowEvent.is_resized = CGI_false;
    window->event.windowEvent.is_scrolled_x = CGI_false;
    window->event.windowEvent.scroll_delta_x = 0;
    window->event.windowEvent.is_scrolled_y = CGI_false;
    window->event.windowEvent.scroll_delta_y = 0;

    // Handle OS level setup
    ZeroMemory(&window->windowState.wc, sizeof(WNDCLASSA));
    window->windowState.wc.style = CS_HREDRAW | CS_VREDRAW;
    window->windowState.wc.lpfnWndProc = windows_procedure;
    window->windowState.wc.hInstance = GetModuleHandleA(NULL);
    window->windowState.wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    window->windowState.wc.lpszClassName = classname;

    if (!RegisterClassA(&window->windowState.wc))
    {
        CGIWindowCleanup(window);
        return NULL;
    }

    window->windowState.hwnd = CreateWindowA(window->windowState.wc.lpszClassName,
                                             window->window_name,
                                             WS_OVERLAPPEDWINDOW,
                                             window->position.x,
                                             window->position.y,
                                             window->width,
                                             window->height,
                                             NULL, NULL,
                                             window->windowState.wc.hInstance,
                                             window);

    if (!window->windowState.hwnd)
    {
        CGIWindowCleanup(window);
        return NULL;
    }

    window->windowState.hdc = GetDC(window->windowState.hwnd);
    if (!window->windowState.hdc)
    {
        CGIWindowCleanup(window);
        return NULL;
    }

    // Get actual client area size
    RECT rect;
    GetClientRect(window->windowState.hwnd, &rect);
    window->windowState.buffer_width = rect.right - rect.left;
    window->windowState.buffer_height = rect.bottom - rect.top;

    // Allocate buffer with correct client area size
    window->windowState.buffer = (COLORREF *)malloc(window->windowState.buffer_width *
                                                    window->windowState.buffer_height *
                                                    sizeof(COLORREF));

    if (!window->windowState.buffer)
    {
        CGIWindowCleanup(window);
        return NULL;
    }

    // Fill buffer with base color
    unsigned int buffer_size = window->windowState.buffer_width * window->windowState.buffer_height;
    for (unsigned int i = 0; i < buffer_size; i++)
    {
        window->windowState.buffer[i] = window->win_base_color;
    }

    // Create bitmap with CLIENT AREA dimensions
    if (!MakeBMI(window, &window->windowState.bmi, window->windowState.buffer_height, window->windowState.buffer_width))
    {
        CGIWindowCleanup(window);
        return NULL;
    }

    return window;
}


CGIEventImage CGIGetWindowEventImage(CGIWindow* window){
    return window->event;
}


CGIWindowSurface CGIGetWindowSurface(CGIWindow* window){
    CGIWindowSurface surface;

    surface.buffer=(void *)malloc(sizeof(unsigned char)*window->windowState.buffer_width*window->windowState.buffer_height*3);

    
    COLORREF *window_main_buffer = window->windowState.buffer;

    unsigned char *buf = (unsigned char *)surface.buffer;

    int rowSize = (window->windowState.buffer_width * 3 + 3) & ~3; // align scanline to 4 bytes

    for (int y = 0; y < window->windowState.buffer_height; y++)
    {
        unsigned char *row = buf + y * rowSize;

        for (int x = 0; x < window->windowState.buffer_width; x++)
        {

            int srcPos = y * window->windowState.buffer_width + x;

            unsigned char r = GetRValue(window_main_buffer[srcPos]);
            unsigned char g = GetGValue(window_main_buffer[srcPos]);
            unsigned char b = GetBValue(window_main_buffer[srcPos]);

            int offset = x * 3;
            row[offset + 0] = r;
            row[offset + 1] = g;
            row[offset + 2] = b;
        }
    }

    
    surface.height=window->windowState.buffer_height;
    surface.width = window->windowState.buffer_width;
    surface.channels=3;

    return surface;
}


void CGIFreeWindowSurface(CGIWindowSurface surface){
    if(surface.buffer){
        free((void *)surface.buffer);
    }
    surface.buffer=NULL;
    surface.height=0;
    surface.width=0;
    surface.channels=0;
}



/// @brief Check if the window is currently focused
/// @param window Pointer to the CGIWindow structure
/// @return CGI_true if the window is focused, otherwise CGI_false
CGIBool CGIIsWindowFocused(CGIWindow *window)
{
    if (!window || !window->windowState.hwnd)
        return CGI_false;

    if (GetForegroundWindow() == window->windowState.hwnd)
    {
        return CGI_true;
    }

    return CGI_false;
}

CGIBool CGIIsKeyDown(CGIKeyCode key)
{
    int vk_key = CGIKeyToWin32VKKey(key);
    if (vk_key == 0)
    {
        return CGI_false;
    }

    SHORT key_state = GetAsyncKeyState(vk_key);

    if (key_state & 0x8000)
    {
        return CGI_true;
    }
    else
    {
        return CGI_false;
    }
}

CGIBool CGIIsKeyUp(CGIKeyCode key)
{
    int vk_key = CGIKeyToWin32VKKey(key);
    if (vk_key == 0)
    {
        return CGI_false;
    }

    SHORT key_state = GetAsyncKeyState(vk_key);

    if (!(key_state & 0x8000))
    {
        return CGI_true;
    }
    else
    {
        return CGI_false;
    }
}

CGIBool CGIIsWindowKeyUp(CGIWindow *window, CGIKeyCode key)
{
    if (window->event.cursorEvent.up_buttons & key)
    {
        return CGI_true;
    }

    if (window->event.keyboardEvent.up_keys & key)
    {
        return CGI_true;
    }

    return CGI_false;
}

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

/// @brief Update basic states of the window
/// @param window Pointer to the CGIWindow structure
/// @return void
// Will update the non-relying states of window
// void internal_window_basic_update(CGIWindow *window)
// {
//     if (!window || !window->windowState.hwnd)
//         return;

//     POINT point;
//     RECT rect;
//     GetClientRect(window->windowState.hwnd, &rect);
//     GetCursorPos(&point);
//     ScreenToClient(window->windowState.hwnd, &point);
//     window->cursor.x = point.x;
//     window->cursor.y = point.y;

//     window->focused = CGIIsWindowFocused(window);
//     window->resized = CGI_false;

//     window->is_scrolled_x = CGI_false;
//     window->scroll_delta_x = 0;
//     window->is_scrolled_y = CGI_false;
//     window->scroll_delta_y = 0;
// }

void ScanCursor(CGIWindow* window,CGICursorEvent *cursorEvent)
{
    POINT point;
    GetCursorPos(&point);
    cursorEvent->system_x = point.x;
    cursorEvent->system_y = point.y;

    ScreenToClient(window->windowState.hwnd,&point);
    
    cursorEvent->win_x=point.x;
    cursorEvent->win_y=point.y;

    SHORT left_button_state = GetAsyncKeyState(VK_LBUTTON);
    SHORT right_button_state = GetAsyncKeyState(VK_RBUTTON);

    cursorEvent->down_buttons = CGI_KEYCODE_NONE;
    cursorEvent->up_buttons = CGI_KEYCODE_NONE;
    if (left_button_state & 0x8000)
    {
        cursorEvent->down_buttons |= CGI_KEYCODE_MOUSE_L;
    }
    else
    {
        cursorEvent->up_buttons |= CGI_KEYCODE_MOUSE_L;
    }

    if (right_button_state & 0x8000)
    {
        cursorEvent->down_buttons |= CGI_KEYCODE_MOUSE_R;
    }
    else
    {
        cursorEvent->up_buttons |= CGI_KEYCODE_MOUSE_R;
    }

    return;
}

void ScanAllKeyBoardKeys(CGIKeyboardEvent *state)
{
    state->down_keys = CGI_KEYCODE_NONE;
    state->up_keys = CGI_KEYCODE_NONE;

    for (int i = 0; i < 59; i++)
    {
        int vk_key = CGIKeyToWin32VKKey(KeyboardKeyCodes[i]);
        SHORT key_state = GetAsyncKeyState(vk_key);
        if (key_state & 0x8000)
        {
            state->down_keys |= KeyboardKeyCodes[i];
        }
        else
        {
            state->up_keys |= KeyboardKeyCodes[i];
        }
    }
}

void getCurrentEventImage(CGIWindow *window, CGIEventImage *eventImage)
{
    ScanAllKeyBoardKeys(&eventImage->keyboardEvent);
    ScanCursor(window,&eventImage->cursorEvent);

    eventImage->windowEvent.is_focused = CGI_false;
    eventImage->windowEvent.is_resized = CGI_false;
    eventImage->windowEvent.is_scrolled_x = CGI_false;
    eventImage->windowEvent.is_scrolled_y = CGI_false;
    eventImage->windowEvent.scroll_delta_x = 0.0f;
    eventImage->windowEvent.scroll_delta_y = 0.0f;

    eventImage->windowEvent.is_focused = CGIIsWindowFocused(window);

    POINT point;
    // RECT rect;
    // GetClientRect(window->windowState.hwnd, &rect);
    GetCursorPos(&point);
    ScreenToClient(window->windowState.hwnd, &point);
    window->cursor.x = point.x;
    window->cursor.y = point.y;

    /// rest of it will be updated in refresh window usage
}

/// @brief  Show the specified window
/// @param window Pointer to the CGIWindow structure
/// @return CGI_true if the window was shown successfully, otherwise CGI_false
CGIBool CGIShowWindow(CGIWindow *window)
{
    if (!window || !window->windowState.hwnd)
    {
        return CGI_false;
    }

    ShowWindow(window->windowState.hwnd, SW_SHOW);

    window->open = CGI_true;
    UpdateWindow(window->windowState.hwnd);
    window->event.windowEvent.is_focused = CGIIsWindowFocused(window);
    return CGI_true;
}

/// @brief Check if the window is currently open
/// @param window Pointer to the CGIWindow structure
/// @return CGI_true if the window is open, otherwise CGI_false
CGIBool CGIIsWindowOpen(const CGIWindow *window)
{
    if (!window)
        return CGI_false;

    return window->open;
}

/// @brief Close the specified window (always the safest way)
/// @param window Pointer to the CGIWindow structure
/// @return CGI_true if the window was closed successfully, otherwise CGI_false
CGIBool CGICloseWindow(CGIWindow *window)
{
    if (!window)
        return CGI_false;
    window->open = CGI_false;
    // SendMessageA(window->windowState.hwnd,WM_CLOSE,0,0);
    PostMessageA(window->windowState.hwnd, WM_CLOSE, 0, 0);
    return CGI_true;
}

/// @brief Refresh the buffer of the specified window
/// @param window Pointer to the CGIWindow structure
/// @return CGI_true if the buffer was refreshed successfully, otherwise CGI_false
CGIBool CGIRefreshBuffer(CGIWindow *window)
{
    if (!window || !window->windowState.hwnd)
        return CGI_false;

    InvalidateRect(window->windowState.hwnd, NULL, TRUE);
    UpdateWindow(window->windowState.hwnd);
    return CGI_true;
}

/// @brief Clear the buffer of the specified window with a given color
/// @param window Pointer to the CGIWindow structure
/// @param color Color to clear the buffer with
/// @return CGI_true if the buffer was cleared successfully, otherwise CGI_false
CGIBool CGIClearBuffer(CGIWindow *window, CGIColor_t color)
{
    if (!window || !window->windowState.buffer)
        return CGI_false;

    unsigned int size = window->windowState.buffer_height * window->windowState.buffer_width;
    COLORREF refcolor = RGB(color.r, color.g, color.b);

    for (unsigned int i = 0; i < size; i++)
    {
        window->windowState.buffer[i] = refcolor;
    }
    return CGI_true;
}

void CGIClearBufferRegion(CGIWindow *window, int x, int y, int width, int height, CGIColor_t color)
{

    if (!window || !window->windowState.buffer)
        return;

    int bw = window->windowState.buffer_width;
    int bh = window->windowState.buffer_height;

    // Completely outside → return
    if (x >= bw || y >= bh || x + width <= 0 || y + height <= 0)
        return;

    // ---- CLIP THE REGION (very important) ----

    int clippedX = x < 0 ? 0 : x;
    int clippedY = y < 0 ? 0 : y;

    int clippedW = (x + width > bw) ? (bw - clippedX) : (width - (clippedX - x));
    int clippedH = (y + height > bh) ? (bh - clippedY) : (height - (clippedY - y));

    for (int i = clippedY; i < clippedY + clippedH; i++)
    {
        for (int j = clippedX; j < clippedX + clippedW; j++)
        {
            window->windowState.buffer[i * window->windowState.buffer_width + j] = RGB(color.r, color.g, color.b);
        }
    }
}

/// @brief Refresh the specified window
/// @param window Pointer to the CGIWindow structure
/// @return CGI_true if the window was refreshed successfully, otherwise CGI_false
CGIBool CGIRefreshWindow(CGIWindow *window, CGIWindowRefreshMode window_refresh_mode)
{
    if (!window || !window->windowState.hwnd)
        return CGI_false;

    if (window->open == CGI_false)
    {
        return CGI_false;
    }

    // internal_window_basic_update(window);

    getCurrentEventImage(window, &window->event);

    if (window_refresh_mode == CGI_window_refresh_mode_rapid)
    {

        while (PeekMessageA(&window->windowState.msg, NULL, 0, 0, PM_REMOVE))
        {
            if (window->windowState.msg.message == WM_QUIT)
            {
                window->open = CGI_false;
                return CGI_true;
            }
            TranslateMessage(&window->windowState.msg);
            DispatchMessageA(&window->windowState.msg);
        }
        return CGI_true;
    }

    if (window_refresh_mode == CGI_window_refresh_mode_triggered)
    {
        while (GetMessageA(&window->windowState.msg, NULL, 0, 0))
        {
            if (window->windowState.msg.message == WM_QUIT)
            {
                window->open = CGI_false;
                return CGI_true;
            }
            TranslateMessage(&window->windowState.msg);
            DispatchMessageA(&window->windowState.msg);
        }
        return CGI_true;
    }

    return CGI_false;
    // Update window states
}

/// @brief Set a pixel at the specified position with the given color
/// @param window Pointer to the CGIWindow structure
/// @param x_pos X-coordinate of the pixel
/// @param y_pos Y-coordinate of the pixel
/// @param color Color to set the pixel to
/// @return void
void CGISetPixel(CGIWindow *window, int x_pos, int y_pos, CGIColor_t color)
{
    if (!window || !window->windowState.buffer)
        return;

    if (x_pos < 0 || y_pos < 0 ||
        x_pos >= (int)window->windowState.buffer_width ||
        y_pos >= (int)window->windowState.buffer_height)
        return;

    window->windowState.buffer[y_pos * window->windowState.buffer_width + x_pos] = RGB(color.r, color.g, color.b);
}

/// @brief Check if the specified window has been resized
/// @param window Pointer to the CGIWindow structure
/// @return CGI_true if the window has been resized, otherwise CGI_false
CGIBool CGIIsWindowResized(CGIWindow *window)
{
    return window->event.windowEvent.is_resized;
}

/// @brief Get the name of the specified window
/// @param window Pointer to the CGIWindow structure
/// @return Pointer to the window name string
char *CGIGetWindowName(CGIWindow *window)
{
    return window->window_name;
}

/// @brief Get the position of the specified window
/// @param window Pointer to the CGIWindow structure
/// @return Position of the window as a CGIPoint structure
CGIPoint CGIGetWindowPosition(CGIWindow *window)
{
    return window->position;
}

/// @brief Get the height of the specified window
/// @param window Pointer to the CGIWindow structure
/// @return Height of the window
unsigned int CGIGetWindowHeight(CGIWindow *window)
{
    return window->height;
}

/// @brief Get the width of the specified window
/// @param window Pointer to the CGIWindow structure
/// @return Width of the window
unsigned int CGIGetWindowWidth(CGIWindow *window)
{
    return window->width;
}

/// @brief Get the buffer height of the specified window
/// @param window Pointer to the CGIWindow structure
/// @return Buffer height of the window
unsigned int CGIGetWindowBufferHeight(CGIWindow *window)
{
    return window->windowState.buffer_height;
}

/// @brief Get the buffer width of the specified window
/// @param window Pointer to the CGIWindow structure
/// @return Buffer width of the window
unsigned int CGIGetWindowBufferWidth(CGIWindow *window)
{
    return window->windowState.buffer_width;
}

/// @brief Get the base color of the specified window
/// @param window Pointer to the CGIWindow structure
/// @return Base color of the window
CGIColor_t CGIGetWindowBaseColor(CGIWindow *window)
{
    return window->base_color;
}

/// @brief Get the cursor position of the specified window
/// @param window Pointer to the CGIWindow structure
/// @return Cursor position of the window as a CGIPoint structure
CGIPoint CGIGetWindowCursorPosition(CGIWindow *window)
{
    return window->cursor;
}

/// @brief Get the horizontal scroll delta of the specified window
/// @param window Pointer to the CGIWindow structure
/// @return Horizontal scroll delta of the window
float CGIGetWindowScrollDeltaX(CGIWindow *window)
{
    return window->event.windowEvent.scroll_delta_x;
}

/// @brief Get the vertical scroll delta of the specified window
/// @param window Pointer to the CGIWindow structure
/// @return Vertical scroll delta of the window
float CGIGetWindowScrollDeltaY(CGIWindow *window)
{
    return window->event.windowEvent.scroll_delta_y;
}

/// @brief Check if the specified window has been scrolled horizontally
/// @param window Pointer to the CGIWindow structure
/// @return CGI_true if the window has been scrolled horizontally, otherwise CGI_false
CGIBool CGIIsWindowScrolledX(CGIWindow *window)
{
    return window->event.windowEvent.is_scrolled_x;
}

/// @brief Check if the specified window has been scrolled vertically
/// @param window Pointer to the CGIWindow structure
/// @return CGI_true if the window has been scrolled vertically, otherwise CGI_false
CGIBool CGIIsWindowScrolledY(CGIWindow *window)
{
    return window->event.windowEvent.is_scrolled_y;
}

CGIBool CGIIsCursorKeyDown(CGIKeyCode key)
{
    if (key == CGI_KEYCODE_MOUSE_L)
    {
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
        {
            return CGI_true;
        }
    }
    else if (key == CGI_KEYCODE_MOUSE_R)
    {
        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
        {
            return CGI_true;
        }
    }

    return CGI_false;
}

CGIBool CGIIsCursorKeyUp(CGIKeyCode key)
{
    if (key == CGI_KEYCODE_MOUSE_L)
    {
        if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
        {
            return CGI_true;
        }
    }
    else if (key == CGI_KEYCODE_MOUSE_R)
    {
        if (!(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
        {
            return CGI_true;
        }
    }

    return CGI_false;
}

CGIBool CGIIsWindowCursorKeyDown(CGIWindow *window, CGIKeyCode key)
{
    if (window->event.cursorEvent.down_buttons & key)
    {
        return CGI_true;
    }

    return CGI_false;
}

CGIBool CGIIsWindowCursorKeyUp(CGIWindow *window, CGIKeyCode key)
{
    if (window->event.cursorEvent.up_buttons & key)
    {
        return CGI_true;
    }

    return CGI_false;
}

/// @brief Set the title of the specified window
/// @param window Pointer to the CGIWindow structure
/// @param title New title for the window
/// @return CGI_true if the operation was successful, otherwise CGI_false
CGIBool CGISetWindowTitle(CGIWindow *window, char *title)
{
    window->window_name = title;
    return (SetWindowTextA(window->windowState.hwnd, title) ? CGI_true : CGI_false);
}

/// @brief Set the position of the specified window
/// @param window Pointer to the CGIWindow structure
/// @param position New position for the window
/// @return CGI_true if the operation was successful, otherwise CGI_false
CGIBool CGISetWindowPosition(CGIWindow *window, CGIPoint position)
{
    return (SetWindowPos(window->windowState.hwnd, 0, position.x, position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER) ? CGI_true : CGI_false);
}

/// @brief Set the size of the specified window
/// @param window Pointer to the CGIWindow structure
/// @param size New size for the window
/// @return CGI_true if the operation was successful, otherwise CGI_false
CGIBool CGISetWindowSize(CGIWindow *window, CGIPoint size)
{

    return (SetWindowPos(window->windowState.hwnd, 0, 0, 0, size.x, size.y, SWP_NOMOVE | SWP_NOZORDER) ? CGI_true : CGI_false);
}

/// @brief Set the base color of the specified window
/// @param window Pointer to the CGIWindow structure
/// @param base_color New base color for the window
/// @return CGI_true if the operation was successful, otherwise CGI_false
CGIBool CGISetWindowBaseColor(CGIWindow *window, CGIColor_t base_color)
{
    window->base_color = base_color;

    return CGI_true;
}

/// @brief Set the show logic of the specified window
/// @param window Pointer to the CGIWindow structure
/// @param logic New show logic for the window
/// @return CGI_true if the operation was successful, otherwise CGI_false
CGIBool CGISetWindowShowLogic(CGIWindow *window, CGIBool logic)
{
    ShowWindow(window->windowState.hwnd, (logic ? SW_SHOW : SW_HIDE));
    if (logic)
    {
        UpdateWindow(window->windowState.hwnd);
        window->open = CGI_true;
    }
    else
    {
        window->open = CGI_false;
    }

    window->event.windowEvent.is_focused = CGIIsWindowFocused(window);

    return CGI_true;
}

/// @brief Set the focus logic of the specified window
/// @param window Pointer to the CGIWindow structure
/// @param logic New focus logic for the window
/// @return CGI_true if the operation was successful, otherwise CGI_false
CGIBool CGISetWindowFocusLogic(CGIWindow *window, CGIBool logic)
{
    SetFocus((logic ? window->windowState.hwnd : NULL));
    return CGI_true;
}

/// @brief Set the resizable logic of the specified window
/// @param window Pointer to the CGIWindow structure
/// @param logic New resizable logic for the window
/// @return CGI_true if the operation was successful, otherwise CGI_false
CGIBool CGISetWindowResizableLogic(CGIWindow *window, CGIBool logic)
{

    LONG style = GetWindowLong(window->windowState.hwnd, GWL_STYLE);

    if (logic)
    {
        style |= WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
    }
    else
    {
        style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
    }

    SetWindowLong(window->windowState.hwnd, GWL_STYLE, style);

    SetWindowPos(window->windowState.hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

    return CGI_true;
}

/// @brief Set the minimizable logic of the specified window
/// @param window Pointer to the CGIWindow structure
/// @param logic New minimizable logic for the window
/// @return CGI_true if the operation was successful, otherwise CGI_false
CGIBool CGISetWindowMinimizableLogic(CGIWindow *window, CGIBool logic)
{

    LONG style = GetWindowLong(window->windowState.hwnd, GWL_STYLE);

    if (logic)
    {
        style |= WS_MINIMIZEBOX;
    }
    else
    {
        style &= ~WS_MINIMIZEBOX;
    }

    SetWindowLong(window->windowState.hwnd, GWL_STYLE, style);

    SetWindowPos(window->windowState.hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

    return CGI_true;
}

/// @brief Set the maximizable logic of the specified window
/// @param window Pointer to the CGIWindow structure
/// @param logic New maximizable logic for the window
/// @return CGI_true if the operation was successful, otherwise CGI_false
CGIBool CGISetWindowMaximizableLogic(CGIWindow *window, CGIBool logic)
{

    LONG style = GetWindowLong(window->windowState.hwnd, GWL_STYLE);

    if (logic)
    {
        style |= WS_MAXIMIZEBOX;
    }
    else
    {
        style &= ~WS_MAXIMIZEBOX;
    }

    SetWindowLong(window->windowState.hwnd, GWL_STYLE, style);

    SetWindowPos(window->windowState.hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

    return CGI_true;
}

/// @brief Get the Win32 HWND handle of the specified window
/// @param window CGIWindow instance
/// @return HWND handle of the window
HWND CGIGetWindow_Win32_HWND(CGIWindow *window)
{
    return window->windowState.hwnd;
}

/// @brief Get the Win32 HDC handle of the specified window
/// @param window CGIWindow instance
/// @return HDC handle of the window
HDC CGIGetWindow_Win32_HDC(CGIWindow *window)
{
    return window->windowState.hdc;
}

/// @brief Get the Win32 WNDCLASSA structure of the specified window
/// @param window CGIWindow instance
/// @return WNDCLASSA structure of the window
WNDCLASSA CGIGetWindow_Win32_WNDCLASSA(CGIWindow *window)
{
    return window->windowState.wc;
}

/// @brief Get the Win32 PAINTSTRUCT structure of the specified window
/// @param window CGIWindow instance
/// @return PAINTSTRUCT structure of the window
PAINTSTRUCT CGIGetWindow_Win32_PaintStruct(CGIWindow *window)
{
    return window->windowState.ps;
}

/// @brief Get the Win32 BITMAPINFO structure of the specified window
/// @param window CGIWindow instance
/// @return BITMAPINFO structure of the window
BITMAPINFO CGIGetWindow_Win32_BITMAPINFO(CGIWindow *window)
{
    return window->windowState.bmi;
}

/// @brief Get the Win32 MSG structure of the specified window
/// @param window CGIWindow instance
/// @return MSG structure of the window
MSG CGIGetWindow_Win32_MSG(CGIWindow *window)
{
    return window->windowState.msg;
}
