#define CGI_WINDOWS_IMPLEMENTATION_ACTIVE

#include "Windows.h"
#include "cgi.h"
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

int CGIKeyToWin32VKKey(CGIInputKey key)
{
    switch (key)
    {
    case CGI_input_key_a:
        return 'A';
    case CGI_input_key_b:
        return 'B';
    case CGI_input_key_c:
        return 'C';
    case CGI_input_key_d:
        return 'D';
    case CGI_input_key_e:
        return 'E';
    case CGI_input_key_f:
        return 'F';
    case CGI_input_key_g:
        return 'G';
    case CGI_input_key_h:
        return 'H';
    case CGI_input_key_i:
        return 'I';
    case CGI_input_key_j:
        return 'J';
    case CGI_input_key_k:
        return 'K';
    case CGI_input_key_l:
        return 'L';
    case CGI_input_key_m:
        return 'M';
    case CGI_input_key_n:
        return 'N';
    case CGI_input_key_o:
        return 'O';
    case CGI_input_key_p:
        return 'P';
    case CGI_input_key_q:
        return 'Q';
    case CGI_input_key_r:
        return 'R';
    case CGI_input_key_s:
        return 'S';
    case CGI_input_key_t:
        return 'T';
    case CGI_input_key_u:
        return 'U';
    case CGI_input_key_v:
        return 'V';
    case CGI_input_key_w:
        return 'W';
    case CGI_input_key_x:
        return 'X';
    case CGI_input_key_y:
        return 'Y';
    case CGI_input_key_z:
        return 'Z';
    case CGI_input_key_0:
        return '0';
    case CGI_input_key_1:
        return '1';
    case CGI_input_key_2:
        return '2';
    case CGI_input_key_3:
        return '3';
    case CGI_input_key_4:
        return '4';
    case CGI_input_key_5:
        return '5';
    case CGI_input_key_6:
        return '6';
    case CGI_input_key_7:
        return '7';
    case CGI_input_key_8:
        return '8';
    case CGI_input_key_9:
        return '9';
    case CGI_input_key_f1:
        return VK_F1;
    case CGI_input_key_f2:
        return VK_F2;
    case CGI_input_key_f3:
        return VK_F3;
    case CGI_input_key_f4:
        return VK_F4;
    case CGI_input_key_f5:
        return VK_F5;
    case CGI_input_key_f6:
        return VK_F6;
    case CGI_input_key_f7:
        return VK_F7;
    case CGI_input_key_f8:
        return VK_F8;
    case CGI_input_key_f9:
        return VK_F9;
    case CGI_input_key_f10:
        return VK_F10;
    case CGI_input_key_f11:
        return VK_F11;
    case CGI_input_key_f12:
        return VK_F12;
    case CGI_input_key_escape:
        return VK_ESCAPE;
    case CGI_input_key_enter:
        return VK_RETURN;
    case CGI_input_key_space:
        return VK_SPACE;
    case CGI_input_key_up:
        return VK_UP;
    case CGI_input_key_down:
        return VK_DOWN;
    case CGI_input_key_left:
        return VK_LEFT;
    case CGI_input_key_right:
        return VK_RIGHT;
    case CGI_input_key_backspace:
        return VK_BACK;
    case CGI_input_key_shift:
        return VK_SHIFT;
    case CGI_input_key_ctrl:
        return VK_CONTROL;
    case CGI_input_key_alt:
        return VK_MENU;

    default:
        break;
    }

    return 0;
}

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
        return NULL;

    DEVMODEA devmode;
    EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &devmode);
    cgi->Display.width = devmode.dmPelsWidth;
    cgi->Display.height = devmode.dmPelsHeight;
    cgi->Display.refresh_rate = devmode.dmDisplayFrequency;

    // Get physical display dimensions
    HDC screen_dc = GetDC(NULL);
    cgi->Display.physical_width = GetDeviceCaps(screen_dc, HORZSIZE);
    cgi->Display.physical_height = GetDeviceCaps(screen_dc, VERTSIZE);
    ReleaseDC(NULL, screen_dc);

    POINT point;
    GetCursorPos(&point);
    cgi->Cursor.cursor_position.x = point.x;
    cgi->Cursor.cursor_position.y = point.y;

    // Fix: GetAsyncKeyState returns non-zero if pressed, convert to boolean
    cgi->Cursor.l_button_pressed = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? CGI_true : CGI_false;
    cgi->Cursor.r_button_pressed = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? CGI_true : CGI_false;

    return cgi;
}

CGIBool CGIUpdate(CGI *cgi)
{
    if (!cgi)
        return CGI_false;

    POINT point;
    GetCursorPos(&point);
    cgi->Cursor.cursor_position.x = point.x;
    cgi->Cursor.cursor_position.y = point.y;

    cgi->Cursor.l_button_pressed = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? CGI_true : CGI_false;
    cgi->Cursor.r_button_pressed = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? CGI_true : CGI_false;

    return CGI_true;
}

CGIBool CGIEnd(CGI *cgi)
{
    if (!cgi)
        return CGI_false;
    free(cgi);
    return CGI_true;
}

CGIBool CGIIsKeyPressed(CGIWindow *window, CGIInputKey key)
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

struct CGIWindow
{
    CGI *cgi;
    struct WindowState windowState;
    char *window_name;
    CGIPoint position;
    CGIPoint cursor;
    unsigned int width;
    unsigned int height;
    COLORREF win_base_color;
    CGIColor_t base_color;
    float scroll_delta_x;
    float scroll_delta_y;
    CGIBool is_scrolled_x;
    CGIBool is_scrolled_y;
    CGIBool open;
    CGIBool focused;
};

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

CGIBool LoadBufferView(void *pixelBuffer, COLORREF *window_main_buffer,
                       int width, int height)
{
    if (!pixelBuffer || !window_main_buffer)
        return CGI_false;

    unsigned char *buf = (unsigned char *)pixelBuffer;

    int rowSize = (width * 3 + 3) & ~3; // align scanline to 4 bytes

    for (int y = 0; y < height; y++)
    {
        unsigned char *row = buf + y * rowSize;

        for (int x = 0; x < width; x++)
        {
            int srcPos = y * width + x;

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

        LoadBufferView(window->windowState.pixelbuffer, window->windowState.buffer,
                       window->windowState.buffer_width, window->windowState.buffer_height);

        BitBlt(window->windowState.hdc, 0, 0, window->windowState.buffer_width,
               window->windowState.buffer_height, window->windowState.offscreenBuffer, 0, 0, SRCCOPY);

        EndPaint(hwnd, &window->windowState.ps);
        return 0;
    }

    case WM_SIZE:
    {
        if (!window)
            return DefWindowProcA(hwnd, msg, wp, lp);

        RECT rect;
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
        return 0;
    }

    case WM_MOVE:
    {
        if (!window)
            return DefWindowProcA(hwnd, msg, wp, lp);

        RECT rect;
        GetWindowRect(window->windowState.hwnd, &rect);
        window->position.x = rect.left;
        window->position.y = rect.top;
        break;
    }

    case WM_MOUSEWHEEL:
    {
        int delta = GET_WHEEL_DELTA_WPARAM(wp);
        window->scroll_delta_y = (float)delta / 120;
        window->is_scrolled_y = CGI_true;
        break;
    }
    case WM_MOUSEHWHEEL:
    {
        int delta = GET_WHEEL_DELTA_WPARAM(wp);
        window->scroll_delta_x = (float)delta / 120;
        window->is_scrolled_x = CGI_true;
        break;
    }

    case WM_DESTROY:
    {
        if (!window)
            return DefWindowProcA(hwnd, msg, wp, lp);

        window->open = CGI_false;
        PostQuitMessage(0);
        return 0;
    }

    default:
    {
        return DefWindowProcA(hwnd, msg, wp, lp);
    }
    }
    return 0;
}

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
    window->focused = CGI_false;
    window->is_scrolled_x = CGI_false;
    window->scroll_delta_x = 0;
    window->is_scrolled_y = CGI_false;
    window->scroll_delta_y = 0;

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

// Will update the non-relying states of window
void internal_window_basic_update(CGIWindow *window)
{
    if (!window || !window->windowState.hwnd)
        return;

    POINT point;
    RECT rect;
    GetClientRect(window->windowState.hwnd, &rect);
    GetCursorPos(&point);
    ScreenToClient(window->windowState.hwnd, &point);
    window->cursor.x = point.x;
    window->cursor.y = point.y;

    window->focused = CGIIsWindowFocused(window);

    window->is_scrolled_x = CGI_false;
    window->scroll_delta_x = 0;
    window->is_scrolled_y = CGI_false;
    window->scroll_delta_y = 0;
}

CGIBool CGIShowWindow(CGIWindow *window)
{
    if (!window || !window->windowState.hwnd)
    {
        return CGI_false;
    }

    ShowWindow(window->windowState.hwnd, SW_SHOW);

    window->open = CGI_true;
    UpdateWindow(window->windowState.hwnd);
    window->focused = CGIIsWindowFocused(window);
    return CGI_true;
}

CGIBool CGIIsWindowOpen(const CGIWindow *window)
{
    if (!window)
        return CGI_false;

    return window->open;
}

CGIBool CGICloseWindow(CGIWindow *window)
{
    if (!window)
        return CGI_false;
    window->open = CGI_false;
    // SendMessageA(window->windowState.hwnd,WM_CLOSE,0,0);
    PostMessageA(window->windowState.hwnd, WM_CLOSE, 0, 0);
    return CGI_true;
}

CGIBool CGIRefreshBuffer(CGIWindow *window)
{
    if (!window || !window->windowState.hwnd)
        return CGI_false;

    InvalidateRect(window->windowState.hwnd, NULL, TRUE);
    UpdateWindow(window->windowState.hwnd);
    return CGI_true;
}

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

CGIBool CGIRefreshWindow(CGIWindow *window)
{
    if (!window || !window->windowState.hwnd)
        return CGI_false;

    if (window->open == CGI_false)
    {
        return CGI_false;
    }

    internal_window_basic_update(window);
    while (PeekMessageA(&window->windowState.msg, NULL, 0, 0, PM_REMOVE))
    {
        if (window->windowState.msg.message == WM_QUIT)
        {
            window->open = CGI_false;
            return CGI_false;
        }
        TranslateMessage(&window->windowState.msg);
        DispatchMessageA(&window->windowState.msg);
    }

    // Update window states
    return CGI_true;
}

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

const void *CGIPerformQuery(CGIQuery query, CGI *cgi, CGIWindow *window)
{
    switch (query)
    {
    // ---------------- Window-related queries ----------------
    case CGI_query_window_internal_win32_HWND:
        if (!window)
            return NULL;
        return &window->windowState.hwnd;

    case CGI_query_window_internal_win32_HDC:
        if (!window)
            return NULL;
        return &window->windowState.hdc;

    case CGI_query_window_internal_win32_BITMAPINFO:
        if (!window)
            return NULL;
        return &window->windowState.bmi;

    case CGI_query_window_internal_win32_PAINTSTRUCT:
        if (!window)
            return NULL;
        return &window->windowState.ps;

    case CGI_query_window_name_charPointer:
        if (!window || !window->window_name)
            return NULL;
        return &window->window_name;

    case CGI_query_window_height_unsigned_int:
        if (!window)
            return NULL;
        return &window->height;

    case CGI_query_window_width_unsigned_int:
        if (!window)
            return NULL;
        return &window->width;

    case CGI_query_window_buffer_height_unsigned_int:
        if (!window)
            return NULL;
        return &window->windowState.buffer_height;

    case CGI_query_window_buffer_width_unsigned_int:
        if (!window)
            return NULL;
        return &window->windowState.buffer_width;

    case CGI_query_window_base_color_CGIColor_t:
        if (!window)
            return NULL;
        return &window->base_color;

    case CGI_query_window_position_CGIPoint:
        if (!window)
            return NULL;
        return &window->position;

    case CGI_query_window_open_status_CGIBool:
        if (!window)
            return NULL;
        return &window->open;

    case CGI_query_window_cursor_position_CGIPoint:
        if (!window)
            return NULL;
        return &window->cursor;

    case CGI_query_window_focus_status_CGIBool:
        if (!window)
            return NULL;
        return &window->focused;

    // ---------------- System/CGI-related queries ----------------
    case CGI_query_system_display_width_unsigned_int:
        if (!cgi)
            return NULL;
        return &cgi->Display.width;

    case CGI_query_system_display_height_unsigned_int:
        if (!cgi)
            return NULL;
        return &cgi->Display.height;

    case CGI_query_system_display_refresh_rate_unsigned_int:
        if (!cgi)
            return NULL;
        return &cgi->Display.refresh_rate;

    case CGI_query_system_display_physical_width_unsigned_int:
        if (!cgi)
            return NULL;
        return &cgi->Display.physical_width;

    case CGI_query_system_display_physical_height_unsigned_int:
        if (!cgi)
            return NULL;
        return &cgi->Display.physical_height;

    case CGI_query_system_cursor_position_CGIPoint:
        if (!cgi)
            return NULL;
        return &cgi->Cursor.cursor_position;

    case CGI_query_system_l_button_pressed_CGIBool:
        if (!cgi)
            return NULL;
        return &cgi->Cursor.l_button_pressed;

    case CGI_query_system_r_button_pressed_CGIBool:
        if (!cgi)
            return NULL;
        return &cgi->Cursor.r_button_pressed;

    case CGI_query_window_scroll_delta_x_float:
        if (!window)
            return NULL;
        return &window->scroll_delta_x;

    case CGI_query_window_scroll_delta_y_float:
    {
        if (!window)
            return NULL;
        return &window->scroll_delta_y;
    }

    case CGI_query_window_is_scrolled_x_CGIBool:
        if (!window)
            return NULL;
        return &window->is_scrolled_x;

    case CGI_query_window_is_scrolled_y_CGIBool:
    {
        if (!window)
            return NULL;
        return &window->is_scrolled_y;
    }

    default:
        return NULL;
    }
}

CGIBool CGIPerformCommand(CGICommand command, const void *args, const void *acceptor)
{
    switch (command)
    {
    case CGI_command_faulty_NULL:
        return CGI_false;

    case CGI_command_CGI_start:
    {
        // if(!args) return CGI_false;
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

        // create window remaining

    case CGI_command_window_cleanup:
    {
        CGIWindow **window = (CGIWindow **)acceptor;
        return CGIWindowCleanup(*window);
    }

    case CGI_command_window_set_pixel:
    {
        if (!args)
            return CGI_false;
        if (!acceptor)
            return CGI_false;
        CGIPixel *pixel = (CGIPixel *)args;
        CGIWindow *window = (CGIWindow *)acceptor;
        CGISetPixel(window, pixel->point.x, pixel->point.y,pixel->color);
        return CGI_true;
    }

    case CGI_command_window_refresh_window:
    {
        if (!acceptor)
            return CGI_false;
        return CGIRefreshWindow(*((CGIWindow **)acceptor));
    }
    case CGI_command_window_refresh_buffer:
    {
        if (!acceptor)
            return CGI_false;
        return CGIRefreshBuffer(*((CGIWindow **)acceptor));
    }

    case CGI_command_window_clear_buffer:
    {
        if (!args)
            return CGI_false;
        if (!acceptor)
            return CGI_false;

        return CGIClearBuffer(*((CGIWindow **)acceptor), *(CGIColor_t *)args);
    }

    case CGI_command_window_set_window_title:
    {
        if (!args)
            return CGI_false;
        if (!acceptor)
            return CGI_false;

        CGIWindow **window = (CGIWindow **)acceptor;
        char **title = (char **)args;
        return (SetWindowTextA((*window)->windowState.hwnd, *title) ? CGI_true : CGI_false);
    }

    case CGI_command_window_set_window_pos:
    {
        if (!args)
            return CGI_false;
        if (!acceptor)
            return CGI_false;

        CGIPoint *point = (CGIPoint *)args;
        CGIWindow **window = (CGIWindow **)acceptor;
        return (SetWindowPos((*window)->windowState.hwnd, 0, point->x, point->y, 0, 0, SWP_NOSIZE | SWP_NOZORDER) ? CGI_true : CGI_false);
    }

    case CGI_command_window_set_window_size:
    {
        if (!acceptor)
            return CGI_false;
        if (!args)
            return CGI_false;

        CGIPoint *point = (CGIPoint *)args;
        CGIWindow **window = (CGIWindow **)acceptor;
        return (SetWindowPos((*window)->windowState.hwnd, 0, 0, 0, point->x, point->y, SWP_NOMOVE | SWP_NOZORDER) ? CGI_true : CGI_false);
    }

    case CGI_command_window_set_window_base_color:
    {
        if (!args)
            return CGI_false;
        if (!acceptor)
            return CGI_false;

        CGIColor_t *color = (CGIColor_t *)args;
        CGIWindow **window = (CGIWindow **)acceptor;

        (*window)->base_color = *color;

        return CGI_true;
    }

    case CGI_command_window_set_window_show_status:
    {
        if (!args)
            return CGI_false;
        if (!acceptor)
            return CGI_false;

        CGIBool *logic = (CGIBool *)args;
        CGIWindow **window = (CGIWindow **)acceptor;

        ShowWindow((*window)->windowState.hwnd, (*logic ? SW_SHOW : SW_HIDE));
        
        UpdateWindow((*window)->windowState.hwnd);
        
        (*window)->focused = CGIIsWindowFocused(*window);
        (*window)->open = CGI_true;

        return CGI_true;
    }

    case CGI_command_window_set_focus_status:
    {
        if (!args)
            return CGI_false;
        if (!acceptor)
            return CGI_false;

        CGIBool *logic = (CGIBool *)args;
        CGIWindow **window = (CGIWindow **)acceptor;

        SetFocus((*logic ? (*window)->windowState.hwnd : NULL));

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
        if (!args)
            return CGI_false;
        if (!acceptor)
            return CGI_false;

        CGIBool *logic = (CGIBool *)args;
        CGIWindow **window = (CGIWindow **)acceptor;

        LONG style = GetWindowLong((*window)->windowState.hwnd, GWL_STYLE);

        if (*logic)
        {
            style |= WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
        }
        else
        {
            style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
        }

        SetWindowLong((*window)->windowState.hwnd, GWL_STYLE, style);

        SetWindowPos((*window)->windowState.hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

        return CGI_true;
    }

    case CGI_command_window_minimizable_logic:
    {
        if (!args)
            return CGI_false;
        if (!acceptor)
            return CGI_true;

        CGIBool *logic = (CGIBool *)args;
        CGIWindow **window = (CGIWindow **)acceptor;

        LONG style = GetWindowLong((*window)->windowState.hwnd, GWL_STYLE);

        if (*logic)
        {
            style |= WS_MINIMIZEBOX;
        }
        else
        {
            style &= ~WS_MINIMIZEBOX;
        }

        SetWindowLong((*window)->windowState.hwnd, GWL_STYLE, style);

        SetWindowPos((*window)->windowState.hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

        return CGI_true;
    }

    case CGI_command_window_maximizable_logic:
    {

        if (!args)
            return CGI_false;
        if (!acceptor)
            return CGI_true;

        CGIBool *logic = (CGIBool *)args;
        CGIWindow **window = (CGIWindow **)acceptor;

        LONG style = GetWindowLong((*window)->windowState.hwnd, GWL_STYLE);

        if (*logic)
        {
            style |= WS_MAXIMIZEBOX;
        }
        else
        {
            style &= ~WS_MAXIMIZEBOX;
        }

        SetWindowLong((*window)->windowState.hwnd, GWL_STYLE, style);

        SetWindowPos((*window)->windowState.hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

        return CGI_true;
    }

    default:
        break;
    }

    return CGI_false;
}
