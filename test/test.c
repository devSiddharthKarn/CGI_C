#include "cgi.h"
#include <windows.h>
#include <stdio.h>

int main() {
    CGI *cgi = NULL;

    // Start CGI library
    if (!CGIPerformCommand(CGI_command_CGI_start, NULL, &cgi)) {
        printf("Failed to start CGI\n");
        return -1;
    }

    // Create a window
    CGIWindow *window = CGICreateWindow("MyWindowClass", "Hello CGI",
                                        100, 100, 400, 300, (CGIColor_t){0, 0, 0});
    if (!window) {
        printf("Failed to create window\n");
        return -1;
    }

    // Show window
    CGIBool show = CGI_true;
    CGIPerformCommand(CGI_command_window_set_window_show_status, &show, &window);

    // Make window resizable, minimizable, maximizable
    CGIBool logic = CGI_true;
    CGIPerformCommand(CGI_command_window_resizable_logic, &logic, &window);
    CGIPerformCommand(CGI_command_window_minimizable_logic, &logic, &window);
    CGIPerformCommand(CGI_command_window_maximizable_logic, &logic, &window);

    

    // Set base color to black
    CGIColor_t black = {0, 0, 0};
    CGIPerformCommand(CGI_command_window_set_window_base_color, &black, &window);

    // Focus the window
    CGIPerformCommand(CGI_command_window_set_focus_status, &show, &window);

    printf("Window created! Press ESC to exit or close the window.\n");

    // Animation variables
    CGIPoint pos = {100, 100};
    CGIPoint size = {400, 300};
    CGIColor_t color = {0, 0, 0};
    int dx = 2, dy = 2;
    int dr = 1, dg = 2, db = 3;

    CGIPoint p={100,0};
    CGIPerformCommand(CGI_command_CGI_set_cursor_position,&p,&cgi);

    // Main loop
    while (CGIIsWindowOpen(window)) {
        // Move window around
        pos.x += dx;
        pos.y += dy;
        if (pos.x < 0 || pos.x + size.x > 800) dx = -dx;
        if (pos.y < 0 || pos.y + size.y > 600) dy = -dy;
        CGIPerformCommand(CGI_command_window_set_window_pos, &pos, &window);

        // Change window size
        size.x += (dx > 0 ? 1 : -1);
        size.y += (dy > 0 ? 1 : -1);
        if (size.x < 200 || size.x > 600) dx = -dx;
        if (size.y < 150 || size.y > 450) dy = -dy;
        CGIPerformCommand(CGI_command_window_set_window_size, &size, &window);

        // Change base color gradually
        color.r = (color.r + dr) % 256;
        color.g = (color.g + dg) % 256;
        color.b = (color.b + db) % 256;
        CGIPerformCommand(CGI_command_window_set_window_base_color, &color, &window);

        // Draw some pixels randomly
        for (int i = 0; i < 50; i++) {
            CGIPixel p = {{rand() % size.x, rand() % size.y}, {rand() % 256, rand() % 256, rand() % 256}};
            CGIPerformCommand(CGI_command_window_set_pixel, &p, &window);
        }

        // Refresh
        CGIRefreshWindow(window);
        CGIClearBuffer(window, color);
        CGIRefreshBuffer(window);

        if(CGIIsKeyPressed(window,CGI_input_key_escape)) CGICloseWindow(window);

        // Small delay
        Sleep(16); // ~60 FPS
    }

    // Cleanup
    CGIPerformCommand(CGI_command_window_cleanup, NULL, &window);
    CGIPerformCommand(CGI_command_CGI_end, NULL, &cgi);

    return 0;
}
