#include "cgi.h"
#include <stdio.h>

int main() {
    // 1️⃣ Start CGI system
    CGI* cgi = CGIStart();
    if (!cgi) {
        printf("Failed to start CGI\n");
        return 1;
    }

    // 2️⃣ Create a window
    CGIColor_t baseColor = CGIMakeColor(100, 150, 200);
    CGIWindow* window = CGICreateWindow("mywindow", "My CGI Window",
                                        100, 100, 500, 400, baseColor);
    if (!window) {
        printf("Failed to create window\n");
        CGIEnd(cgi);
        return 1;
    }

    // 3️⃣ Show the window
    CGIShowWindow(window);

    // 4️⃣ Fill the window with a color
    for (int y = 0; y < 400; y++) {
        for (int x = 0; x < 500; x++) {
            CGISetPixel(window, x, y, baseColor);
        }
    }
    CGIRefreshWindow(window);

    printf("Click inside the window to see the cursor position.\n");

    // 5️⃣ Main loop
    while (CGIIsWindowOpen(window)) {
        CGIRefreshWindow(window);
        CGIUpdate(cgi);

        // Get cursor position
        CGIPoint cursor = *(CGIPoint*)CGIPerformQuery(CGI_query_system_cursor_position, cgi, window);

        // Detect left mouse button press
        CGIBool l_pressed = *(CGIBool*)CGIPerformQuery(CGI_query_system_l_button_pressed, cgi, window);

        if (l_pressed) {
            printf("Left click at (%d, %d)\n", cursor.x, cursor.y);
        }
    }

    // 6️⃣ Cleanup
    CGICloseWindow(window);
    CGIWindowCleanup(window);
    CGIEnd(cgi);

    return 0;
}
