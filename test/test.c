#include "cgi.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <X11/Xlib.h>
#endif

int main() {
    // 1️⃣ Create a window
    CGIWindow* win = CGICreateWindow(
        "CGIWindow", "Rainbow Square Demo",
        100, 100, 600, 400,
        CGIMakeColor(0,0,0)  // black background
    );

    if (!win) {
        printf("Failed to create window!\n");
        return 1;
    }

    CGIShowWindow(win);

    // 2️⃣ Initial position of the square
    int x = 300, y = 200;
    int size = 50;
    CGIColor_t color = CGIMakeColor(255,0,0);

    while (CGIIsWindowOpen(win)) {
        // Clear the window
        // CGIRefreshWindow(win);
        CGIBufferClear(win, CGIMakeColor(0,0,0));

        // Draw a rainbow square
        for(int i=0;i<size;i++) {
            for(int j=0;j<size;j++) {
                color.r = (x+i) % 256;
                color.g = (y+j) % 256;
                color.b = (i+j) % 256;
                CGISetPixel(win, x+i, y+j, color);
            }
        }

        CGIRefreshWindow(win);

        // 3️⃣ Handle arrow keys
        if(CGIIsKeyPressed(win, CGI_input_key_up))    y -= 5;
        if(CGIIsKeyPressed(win, CGI_input_key_down))  y += 5;
        if(CGIIsKeyPressed(win, CGI_input_key_left))  x -= 5;
        if(CGIIsKeyPressed(win, CGI_input_key_right)) x += 5;

        // 4️⃣ Handle scroll (optional)
#ifdef _WIN32
        if(CGIIsKeyPressed(win, CGI_input_key_space)) size += 2; // space increases size
#elif __linux__
        // scroll events handled inside CGI if implemented
#endif

        // Keep inside window bounds
        if(x<0) x=0;
        if(y<0) y=0;
        if(x>*(int*)CGIQueryWindow(CGI_query_window_buffer_width, win)-size) x=*(int*)CGIQueryWindow(CGI_query_window_buffer_width, win)-size;
        if(y>*(int*)CGIQueryWindow(CGI_query_window_buffer_height, win)-size) y=*(int*)CGIQueryWindow(CGI_query_window_buffer_height, win)-size;

#ifdef _WIN32
        Sleep(16); // ~60 FPS
#else
        usleep(16000);
#endif
    }

    CGIWindowCleanup(win);
    return 0;
}
