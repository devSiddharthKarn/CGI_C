#include "cgi.h"
#include <stdio.h>
#include <Windows.h>

int main() {
    // Start the CGI system
    CGI* cgi = CGIStart();

    // Create a window
    CGIWindow* window = CGICreateWindow(
        "mywindow", "Scroll Test", 100, 100, 500, 500,
        CGIMakeColor(200, 200, 200)
    );

    CGIShowWindow(window);

    // Main loop
    while (CGIIsWindowOpen(window)) {
        CGIUpdate(cgi);           // Process events
        CGIRefreshWindow(window); // Refresh the window

        // Query scroll state
        CGIBool is_scrolled_y = *(CGIBool*)CGIPerformQuery(CGI_query_window_is_scrolled_y, cgi, window);
        float scroll_delta_y = *(float*)CGIPerformQuery(CGI_query_window_scroll_delta_y, cgi, window);

        if (is_scrolled_y) {
            printf("Scroll detected! Delta Y: %f\n", scroll_delta_y);

          
        }

        Sleep(50); // small delay to prevent spamming
    }

    return 0;
}
