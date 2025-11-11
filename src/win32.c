#define CGI_WINDOWS_IMPLEMENTATION_ACTIVE

#include "Windows.h"
#include "cgi.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"


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

struct SYSDISPLAY
{
    unsigned int width;
    unsigned int height;
    CGIPoint cursor;
};

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
    struct WindowState windowState;
    char *window_name;
    CGIPoint position;
    struct SYSDISPLAY display;
    unsigned int width;
    unsigned int height;
    COLORREF win_base_color;
    CGIColor_t base_color;
    CGIBool open;
};

CGIBool CGIWindowCleanup(CGIWindow *window)
{
    if (!window)
        return CGI_false;
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
    bmi->bmiHeader.biHeight = -height; // top down view + is bottom up view;
    bmi->bmiHeader.biPlanes = 1;
    bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi->bmiHeader.biWidth = width;

    if (window->windowState.hBitMap)
    {
        SelectObject(window->windowState.offscreenBuffer, GetStockObject(NULL_BRUSH)); // deselect
        DeleteObject(window->windowState.hBitMap);
        window->windowState.hBitMap = NULL;
    }
    if (window->windowState.offscreenBuffer)
    {
        DeleteDC(window->windowState.offscreenBuffer);
        window->windowState.offscreenBuffer = NULL;
    }

    window->windowState.offscreenBuffer = CreateCompatibleDC(window->windowState.hdc);

    window->windowState.hBitMap = CreateDIBSection(window->windowState.offscreenBuffer, &window->windowState.bmi, DIB_RGB_COLORS, &window->windowState.pixelbuffer, NULL, 0);


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
        return TRUE;
    }

    case WM_PAINT:
    {
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

                // Store old dimensions
                unsigned int old_width = window->windowState.buffer_width;
                unsigned int old_height = window->windowState.buffer_height;

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
        window->open = CGI_false;
        DestroyWindow(hwnd);
        return 0;
    }

    case WM_MOVE:
    {
        RECT rect;
        GetWindowRect(window->windowState.hwnd, &rect);
        window->position.x = rect.left;
        window->position.y = rect.top;
        break;
    }

    case WM_DESTROY:
    {
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

CGIPoint CGIGetCursorPoint()
{
    POINT point;
    GetCursorPos(&point);
    CGIPoint ret;
    ret.x = point.x;
    ret.y = point.y;

    return ret;
}

// CGIBool initialize_OpenGL(CGIWindow *window)
// {
//     window->glState.hdc = (HDC)CGIQueryWindow(CGI_query_window_internal_win32_HDC, window);

//     window->glState.pfd.nVersion = 1;
//     window->glState.pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
//     window->glState.pfd.iPixelType = PFD_TYPE_RGBA;
//     window->glState.pfd.iLayerType = PFD_MAIN_PLANE;
//     window->glState.pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
//     window->glState.pfd.cColorBits = 32;

//     window->glState.pf = ChoosePixelFormat(window->glState.hdc, &window->glState.pfd);

//     if (!SetPixelFormat(window->glState.hdc, window->glState.pf, &window->glState.pfd))
//     {
//         return CGI_false;
//     }

//     window->glState.glrc = wglCreateContext(window->glState.hdc);
//     wglMakeCurrent(window->glState.hdc, window->glState.glrc);

//     // gladLoadGL();
//     if (!gladLoadGL())
//     {
//         wglDeleteContext(window->glState.glrc);
//         return CGI_false;
//     }

//     window->glState.v_m_el_size = 8;
//     window->glState.vertices_mapped = (GLfloat *)malloc(window->glState.v_m_el_size * sizeof(GLfloat));
//     window->glState.vertices_mapped[0] = -1.0f;
//     window->glState.vertices_mapped[1] = -1.0f;
//     window->glState.vertices_mapped[2] = 1.0f;
//     window->glState.vertices_mapped[3] = -1.0f;
//     window->glState.vertices_mapped[4] = -1.0f;
//     window->glState.vertices_mapped[5] = 1.0f;
//     window->glState.vertices_mapped[6] = 1.0f;
//     window->glState.vertices_mapped[7] = 1.0f;

//     window->glState.i_m_el_size = 6;
//     window->glState.indices = (GLuint *)malloc(window->glState.i_m_el_size * sizeof(GLuint));
//     window->glState.indices[0] = 0;
//     window->glState.indices[1] = 1;
//     window->glState.indices[2] = 2;
//     window->glState.indices[3] = 1;
//     window->glState.indices[4] = 2;
//     window->glState.indices[5] = 3;

//     window->glState.tc_m_el_size = window->glState.v_m_el_size;
//     window->glState.texture_cord_mapped = (GLfloat *)malloc(window->glState.tc_m_el_size * sizeof(GLfloat));
//     window->glState.texture_cord_mapped[0] = 0.0f;
//     window->glState.texture_cord_mapped[1] = 1.0f;
//     window->glState.texture_cord_mapped[2] = 1.0f;
//     window->glState.texture_cord_mapped[3] = 1.0f;
//     window->glState.texture_cord_mapped[4] = 0.0f;
//     window->glState.texture_cord_mapped[5] = 0.0f;
//     window->glState.texture_cord_mapped[6] = 1.0f;
//     window->glState.texture_cord_mapped[7] = 0.0f;

//     glGenVertexArrays(1, &window->glState.vao);
//     glBindVertexArray(window->glState.vao);

//     glGenBuffers(1, &window->glState.vbo);
//     glBindBuffer(GL_ARRAY_BUFFER, window->glState.vbo);
//     glBufferData(GL_ARRAY_BUFFER, window->glState.v_m_el_size * sizeof(GLfloat), window->glState.vertices_mapped, GL_STATIC_DRAW);

//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *)0);

//     glGenBuffers(1, &window->glState.ebo);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, window->glState.ebo);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, window->glState.i_m_el_size * sizeof(GLuint), window->glState.indices, GL_STATIC_DRAW);

//     glGenBuffers(1, &window->glState.texCord_bo);
//     glBindBuffer(GL_ARRAY_BUFFER, window->glState.texCord_bo);
//     glBufferData(GL_ARRAY_BUFFER, window->glState.tc_m_el_size * sizeof(GLfloat), window->glState.texture_cord_mapped, GL_STATIC_DRAW);
//     glEnableVertexAttribArray(1);
//     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *)0);

//     int buffer_width = window->windowState.buffer_width;
//     int buffer_height = window->windowState.buffer_height;

//     glGenTextures(1, &window->glState.texture);
//     glBindTexture(GL_TEXTURE_2D, window->glState.texture);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, buffer_width, buffer_height, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

//     // glGenerateMipmap(GL_TEXTURE_2D);

//     glGenBuffers(1, &window->glState.pbo);
//     glBindBuffer(GL_PIXEL_UNPACK_BUFFER, window->glState.pbo);
//     glBufferData(GL_PIXEL_UNPACK_BUFFER, buffer_width * buffer_height * sizeof(COLORREF), NULL, GL_STREAM_DRAW);

//     GLint result;
//     window->glState.vertexShaderSRC = readShaderFile("src/shaders/vertexShader.glsl");
//     window->glState.fragmentShaderSRC = readShaderFile("src/shaders/fragmentShader.glsl");

//     window->glState.vertexShader = glCreateShader(GL_VERTEX_SHADER);
//     glShaderSource(window->glState.vertexShader, 1, (const GLchar *const *)&window->glState.vertexShaderSRC, NULL);
//     glCompileShader(window->glState.vertexShader);

//     glGetShaderiv(window->glState.vertexShader, GL_COMPILE_STATUS, &result);
//     if (result == GL_FALSE)
//     {
//         return CGI_false;
//     }

//     window->glState.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//     glShaderSource(window->glState.fragmentShader, 1, (const GLchar *const *)&window->glState.fragmentShaderSRC, NULL);
//     glCompileShader(window->glState.fragmentShader);

//     glGetShaderiv(window->glState.fragmentShader, GL_COMPILE_STATUS, &result);

//     if (result == GL_FALSE)
//     {
//         // printf("failed");
//         return CGI_false;
//     }

//     window->glState.shaderProgram = glCreateProgram();
//     glAttachShader(window->glState.shaderProgram, window->glState.vertexShader);
//     glAttachShader(window->glState.shaderProgram, window->glState.fragmentShader);
//     glLinkProgram(window->glState.shaderProgram);

//     glGetProgramiv(window->glState.shaderProgram, GL_LINK_STATUS, &result);
//     if (result == GL_FALSE)
//         return CGI_false;

//     glDeleteShader(window->glState.vertexShader);
//     glDeleteShader(window->glState.fragmentShader);

//     float r = (float)window->base_color.r / 255;
//     float g = (float)window->base_color.g / 255;
//     float b = (float)window->base_color.b / 255;

//     glClearColor(r, g, b, 1.0f);
//     glUseProgram(window->glState.shaderProgram);
//     glBindVertexArray(window->glState.vao);
//     return CGI_true;
// }

// CGIBool Opengl_render(CGIWindow *window)
// {

//     // glBindTexture(GL_TEXTURE_2D,window->glState.texture);
//     glBindBuffer(GL_PIXEL_UNPACK_BUFFER, window->glState.pbo);

//     GLubyte *ptr = (GLubyte *)glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, window->windowState.buffer_width * window->windowState.buffer_height * sizeof(COLORREF), GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_WRITE_BIT);

//     if (ptr)
//     {
//         memcpy(ptr, window->windowState.buffer, window->windowState.buffer_height * window->windowState.buffer_width * sizeof(COLORREF));
//         glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
//     }
//     else
//         return CGI_false;

//     // glBindBuffer(GL_PIXEL_UNPACK_BUFFER,window->glState.pbo);
//     glBindTexture(GL_TEXTURE_2D, window->glState.texture);
//     glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, window->windowState.buffer_width, window->windowState.buffer_height, GL_BGRA, GL_UNSIGNED_BYTE, 0);

//     glDrawElements(GL_TRIANGLES, window->glState.i_m_el_size, GL_UNSIGNED_INT, 0);

//     SwapBuffers(window->glState.hdc);
//     // printf("Drawing via opengl swap");

//     return CGI_true;
// }
// #endif

CGIWindow *CGICreateWindow(char *classname, char *window_name, unsigned int x_pos, unsigned int y_pos, unsigned int width, unsigned int height, CGIColor_t color)
{
    CGIWindow *window = (CGIWindow *)malloc(sizeof(CGIWindow));

    memset(window, 0, sizeof(CGIWindow));

    window->window_name = _strdup(window_name);
    window->position.x = x_pos;
    window->position.y = y_pos;
    window->height = height;
    window->width = width;
    window->win_base_color = RGB(color.r, color.g, color.b);
    window->base_color = color;
    window->open = CGI_false;

    // handle os level setup
    ZeroMemory(&window->windowState.wc, sizeof(WNDCLASSA));
    window->windowState.wc.style = CS_HREDRAW | CS_VREDRAW;
    window->windowState.wc.lpfnWndProc = windows_procedure;
    window->windowState.wc.hInstance = GetModuleHandle(NULL);
    window->windowState.wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    window->windowState.wc.lpszClassName = classname;

    RegisterClassA(&window->windowState.wc);

    window->windowState.hwnd = CreateWindowA(window->windowState.wc.lpszClassName, window->window_name, WS_OVERLAPPEDWINDOW, window->position.x, window->position.y, window->width, window->height, NULL, NULL, window->windowState.wc.hInstance, window);

    if (!window->windowState.hwnd)
    {
        CGIWindowCleanup(window);
        return NULL;
    }

    window->windowState.hdc = GetDC(window->windowState.hwnd);

    // Get actual client area size
    RECT rect;
    GetClientRect(window->windowState.hwnd, &rect);
    window->windowState.buffer_width = rect.right - rect.left;
    window->windowState.buffer_height = rect.bottom - rect.top;

    // Allocate buffer with correct client area size
    window->windowState.buffer = (COLORREF *)malloc(window->windowState.buffer_width * window->windowState.buffer_height * sizeof(COLORREF));

    if (window->windowState.buffer == NULL)
    {
        CGIWindowCleanup(window);
        return NULL;
    }

    // Fill buffer with base color
    int buffer_size = window->windowState.buffer_width * window->windowState.buffer_height;
    for (int i = 0; i < buffer_size; i++)
    {
        window->windowState.buffer[i] = window->win_base_color;
    }

    // Create bitmap with CLIENT AREA dimensions
    MakeBMI(window, &window->windowState.bmi, window->windowState.buffer_height, window->windowState.buffer_width);

    window->display.width = GetSystemMetrics(SM_CXSCREEN);
    window->display.height = GetSystemMetrics(SM_CYSCREEN);
    POINT point;
    GetCursorPos(&point);
    window->display.cursor.x = point.x;
    window->display.cursor.y = point.y;

    return window;
}

CGIBool CGIShowWindow(CGIWindow *window)
{
    if (!window)
    {
        return CGI_false;
    }
    ShowWindow(window->windowState.hwnd, SW_SHOW);

    window->open = CGI_true;

    RECT rect;
    GetClientRect(window->windowState.hwnd, &rect);
    window->windowState.buffer_width = rect.right - rect.left;
    window->windowState.buffer_height = rect.bottom - rect.top;

    UpdateWindow(window->windowState.hwnd);
    return CGI_true;
}

CGIBool CGIIsWindowOpen(const CGIWindow *window)
{
    return window->open;
}

CGIBool CGIRefreshBuffer(CGIWindow *window)
{
    if (!window)
        return CGI_false;
    InvalidateRect(window->windowState.hwnd, NULL, TRUE);
    UpdateWindow(window->windowState.hwnd);
    return CGI_true;
}

CGIBool CGIClearBuffer(CGIWindow *window, CGIColor_t color)
{
    if (!window)
        return CGI_false;

    int size = window->windowState.buffer_height * window->windowState.buffer_width;
    for (int i = 0; i < size; i++)
    {
        window->windowState.buffer[i] = RGB(color.r, color.g, color.b);
    }
    return CGI_true;
}

CGIBool CGIRefreshWindow(CGIWindow *window)
{
    if (!window)
        return CGI_false;
    if (window->open == CGI_false)
    {
        return CGI_false;
    }
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
    // CGIRefreshBuffer(window);
    return CGI_true;
}

void CGISetPixel(CGIWindow *window, int x_pos, int y_pos, CGIColor_t color)
{
    if (x_pos < 0 || y_pos < 0 ||
        x_pos >= (int)window->windowState.buffer_width ||
        y_pos >= (int)window->windowState.buffer_height)
        return;

    window->windowState.buffer[y_pos * window->windowState.buffer_width + x_pos] = RGB(color.r, color.g, color.b);
}

const void *CGIQueryWindow(CGIQuery query, CGIWindow *window)
{
    switch (query)
    {
    case CGI_query_window_internal_win32_HWND:
    {
        return &window->windowState.hwnd;
    }
    case CGI_query_window_internal_win32_HDC:
    {
        return &window->windowState.hdc;
    }
    case CGI_query_window_internal_win32_BITMAPINFO:
    {
        return &window->windowState.bmi;
    }
    case CGI_query_window_internal_win32_PAINTSTRUCT:
    {
        return &window->windowState.ps;
    }
    case CGI_query_window_name:
    {
        return &window->window_name;
    }
    case CGI_query_window_height:
    {
        return &window->height;
    }
    case CGI_query_window_width:
    {
        return &window->width;
    }
    case CGI_query_window_buffer_height:
    {
        return &window->windowState.buffer_height;
    }
    case CGI_query_window_buffer_width:
    {
        return &window->windowState.buffer_width;
    }
    case CGI_query_window_base_color:
    {
        return &window->base_color;
    }
    case CGI_query_window_position:
    {
        return &window->position;
    }
    case CGI_query_system_display_width:
    {
        window->display.width = GetSystemMetrics(SM_CXSCREEN);
        return &window->display.width;
    }
    case CGI_query_system_display_height:
    {
        window->display.height = GetSystemMetrics(SM_CYSCREEN);
        return &window->display.height;
    }
    case CGI_query_window_open_status:
    {
        return &window->open;
    }
    default:
        break;
    }

    return NULL;
}

CGIBool CGIIsWindowFocused(CGIWindow *window)
{
    if (GetForegroundWindow() == window->windowState.hwnd)
    {
        return CGI_true;
    }

    return CGI_false;
}

