#include "../include/cgi.h"
#include "../include/cgi_font.h"

#include <string.h>
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include "Windows.h"
#define SLEEP(seconds) Sleep(seconds*1000);
#elif __linux__
#include "unistd.h"
#define SLEEP(seconds) sleep(seconds);
#endif

int main(){

    // Start CGI system
    CGI* cgi = CGIStart();

    // Create window
    CGIWindow *window = CGICreateWindow(
        "window", "window",
        50, 50, 500, 500,
        CGIMakeColor(123,222,12)
    );

    CGIShowWindow(window);

    // FPS variables
    int fps = 0;
    int frames = 0;
    double lastTime = 0;

    char sentence[10000] = "Hello and this is me! and this is a long sentence to test the text rendering capabilities of the CGI library. Let's see how well it can handle this text and display it on the screen. Hopefully, it works well and looks good!";

    // Main loop
    while (CGIIsWindowOpen(window)) {

        // Time tracking for FPS
        double currentTime = clock() / (double)CLOCKS_PER_SEC;
        frames++;

        if (currentTime - lastTime >= 1.0) {
            fps = frames;
            frames = 0;
            lastTime = currentTime;
        }

        CGIUpdate(cgi);
        CGIRefreshWindow(window);
        CGIClearBuffer(window, CGIMakeColor(123,222,12));

        // ========================
        //      DRAW FPS TEXT
        // ========================
        char fpsText[64];
        sprintf(fpsText, "FPS: %d", fps);

        CGIWriteText(
            window,
            fpsText,
            10, 10,        // Position
            1, 1,          // Spacing
            2, 2,          // Scale (2x bigger)
            1,
            CGI_true,             // thickness or your parameter
            CGIMakeColor(0,0,0)
        );

        // Example: draw another text
        CGIWriteText(
            window,
            sentence,
            10, 60,
            1, 1,
            1, 1,
            1,
            CGI_false,
            CGIMakeColor(0,0,0)
        );

        CGIRefreshBuffer(window);
        // Optional lower CPU usage
        // SLEEP(0.001);
    }

    return 0;
}
