#include "cgi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "math.h"

#ifdef _WIN32
#include <windows.h> // for Sleep
#else
#include <unistd.h>  // for usleep
#endif

int main() {
    srand((unsigned int)time(NULL));

    CGI *cgi = NULL;
    if (!CGIPerformCommand(CGI_command_CGI_start, NULL, &cgi)) {
        printf("Failed to start CGI\n");
        return -1;
    }

    // Create window
    CGIWindow *win = CGICreateWindow("FunnyClass", "Bouncing Fun!",
                                     100, 100, 500, 400,
                                     (CGIColor_t){0, 0, 0});
    if (!win) return -1;

    CGIBool show = CGI_true;
    CGIPerformCommand(CGI_command_window_set_window_show_status, &show, &win);

    // Allow resizing
    CGIPerformCommand(CGI_command_window_resizable_logic, &show, &win);

    int dx = 4, dy = 3;         // window movement
    int dr = 2, dg = 3, db = 4; // color change
    CGIPoint pos = {100, 100};
    CGIPoint size = {500, 400};
    CGIColor_t color = {0, 0, 0};

    CGIPoint cursor = {0, 0};

    printf("Press ESC to exit!\n");

    while (CGIIsWindowOpen(win)) {
        // Move window
        pos.x += dx; pos.y += dy;
        if (pos.x < 0 || pos.x + size.x > 800) dx = -dx;
        if (pos.y < 0 || pos.y + size.y > 600) dy = -dy;
        CGIPerformCommand(CGI_command_window_set_window_pos, &pos, &win);

        // Resize window like breathing
        size.x += (dx>0?1:-1); size.y += (dy>0?1:-1);
        if (size.x < 300 || size.x > 600) dx=-dx;
        if (size.y < 200 || size.y > 450) dy=-dy;
        CGIPerformCommand(CGI_command_window_set_window_size, &size, &win);

        // Disco color
        color.r = (color.r + dr) % 256;
        color.g = (color.g + dg) % 256;
        color.b = (color.b + db) % 256;
        CGIPerformCommand(CGI_command_window_set_window_base_color, &color, &win);

        // Fireworks pixels
        for (int i = 0; i < 30; i++) {
            CGIPixel p = {
                {rand() % size.x, rand() % size.y},
                {(unsigned char)(rand()%256), (unsigned char)(rand()%256), (unsigned char)(rand()%256)}
            };
            CGIPerformCommand(CGI_command_window_set_pixel, &p, &win);
        }

        // Move cursor in a circle
        static double angle = 0;
        cursor.x = (int)(pos.x + size.x/2 + 100 * cos(angle));
        cursor.y = (int)(pos.y + size.y/2 + 100 * sin(angle));
        angle += 0.1;
        CGIPerformCommand(CGI_command_CGI_set_cursor_position, &cursor, &cgi);

        // Refresh window & buffer
        CGIPerformCommand(CGI_command_window_refresh_window, NULL, &win);
        CGIPerformCommand(CGI_command_window_clear_buffer, &color, &win);
        CGIPerformCommand(CGI_command_window_refresh_buffer, NULL, &win);

        // Close on ESC
        if (CGIIsKeyPressed(win, CGI_input_key_escape))
            CGIPerformCommand(CGI_command_window_close, NULL, &win);

        // Sleep ~60 FPS
        #ifdef _WIN32
        Sleep(16);
        #else
        usleep(16000);
        #endif
    }

    CGIPerformCommand(CGI_command_CGI_end, NULL, &cgi);
    return 0;
}
