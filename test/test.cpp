#include "../include/cgi.h"
#include "../include/cgi_font.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

    // Start system
    CGI* cgi = CGIStart();

    // Create window
    CGIWindow * window = CGICreateWindow(
        "fps_test", "FPS Test",
        40, 40, 600, 600,
        CGIMakeColor(220, 220, 220)
    );

    CGIShowWindow(window);

    // FPS Variables
    double lastTime = (double)clock() / CLOCKS_PER_SEC;
    int frames = 0;
    double fps = 0.0;

    while (CGIIsWindowOpen(window)) {

        // Update timers
        double currentTime = (double)clock() / CLOCKS_PER_SEC;
        frames++;

        if (currentTime - lastTime >= 1.0) {
            fps = frames / (currentTime - lastTime);
            frames = 0;
            lastTime = currentTime;
        }

        // CGI update cycle
        CGIUpdate(cgi);
        CGIRefreshWindow(window,CGI_window_refresh_mode_triggered);

        // Clear
        // CGIClearBuffer(window, CGIMakeColor(220, 220, 220));

        CGIClearBufferRegion(window,0,0,500,100,CGIMakeColor(100,0,0));

        // Convert FPS to string
        char fpsText[64];
        snprintf(fpsText, sizeof(fpsText), "FPS: %.1f", fps);

        // Draw FPS
        CGIWriteText(
            window,
            fpsText,
            20,               // x
            20,               // y
            1,1,              // sx, sy
            5,5,              // px, py
            0,                // rotation
            CGI_true,
            CGIMakeColor(0,0,0)
        );

        // Push to screen
        CGIRefreshBufferRegion(window,0,0,500,100);
        // CGIRefreshBuffer(window);
    }

    // Cleanup
    CGIWindowCleanup(window);
    CGICloseWindow(window);
    CGIEnd(cgi);

    return 0;
}
