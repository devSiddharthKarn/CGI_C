/*
 * CGI Input Testing Program
 * Tests all keyboard and mouse inputs
 */

#include "../include/cgi.h"
#include <stdio.h>
#include <stdlib.h>

// Helper function to print key states
void PrintKeyState(const char* keyName, CGIWindow* window, CGIKeyCode keyCode) {
    if (CGIIsWindowKeyDown(window, keyCode)) {
        printf("[DOWN] %s\n", keyName);
    }
}

void PrintCursorButtonState(const char* buttonName, CGIWindow* window, CGIKeyCode buttonCode) {
    if (CGIIsWindowCursorKeyDown(window, buttonCode)) {
        printf("[MOUSE DOWN] %s\n", buttonName);
    }
}

int main() {
    // Create window
    CGIWindow* window = CGICreateWindow(
        "InputTestClass",
        "CGI Input Test - Press keys and move mouse",
        100, 100,
        800, 600,
        CGIMakeColor(30, 30, 40)
    );

    if (!window) {
        printf("Failed to create window!\n");
        return -1;
    }

    CGIShowWindow(window);

    printf("=== CGI Input Test Started ===\n");
    printf("Press ESC to exit\n");
    printf("Try pressing multiple keys simultaneously\n\n");

    int frameCount = 0;

    while (CGIIsWindowOpen(window)) {
        // Refresh window state
        CGIRefreshWindow(window, CGI_window_refresh_mode_rapid);

        // Check for ESC to exit
        if (CGIIsWindowKeyDown(window, CGI_KEYCODE_ESCAPE)) {
            printf("\nESC pressed - Exiting...\n");
            break;
        }

        // Only print status every 10 frames to avoid spam
        if (frameCount % 10 == 0) {
            // Clear console (Windows)
            system("clear");
            
            printf("=== CGI Input Test ===\n");
            printf("Frame: %d\n\n", frameCount);

            // Cursor position
            CGIPoint cursor = CGIGetWindowCursorPosition(window);
            printf("Cursor Position: (%d, %d)\n\n", cursor.x, cursor.y);

            // Window state
            printf("Window Focused: %s\n", CGIIsWindowFocused(window) ? "YES" : "NO");
            printf("Window Resized: %s\n\n", CGIIsWindowResized(window) ? "YES" : "NO");

            // Scroll state
            if (CGIIsWindowScrolledX(window)) {
                printf("Scroll X: %.2f\n", CGIGetWindowScrollDeltaX(window));
            }
            if (CGIIsWindowScrolledY(window)) {
                printf("Scroll Y: %.2f\n", CGIGetWindowScrollDeltaY(window));
            }

            // Mouse buttons
            printf("\n--- Mouse Buttons ---\n");
            PrintCursorButtonState("Left Mouse", window, CGI_KEYCODE_MOUSE_L);
            PrintCursorButtonState("Right Mouse", window, CGI_KEYCODE_MOUSE_R);

            // Alphabet keys
            printf("\n--- Alphabet Keys ---\n");
            PrintKeyState("A", window, CGI_KEYCODE_A);
            PrintKeyState("B", window, CGI_KEYCODE_B);
            PrintKeyState("C", window, CGI_KEYCODE_C);
            PrintKeyState("D", window, CGI_KEYCODE_D);
            PrintKeyState("E", window, CGI_KEYCODE_E);
            PrintKeyState("F", window, CGI_KEYCODE_F);
            PrintKeyState("G", window, CGI_KEYCODE_G);
            PrintKeyState("H", window, CGI_KEYCODE_H);
            PrintKeyState("I", window, CGI_KEYCODE_I);
            PrintKeyState("J", window, CGI_KEYCODE_J);
            PrintKeyState("K", window, CGI_KEYCODE_K);
            PrintKeyState("L", window, CGI_KEYCODE_L);
            PrintKeyState("M", window, CGI_KEYCODE_M);
            PrintKeyState("N", window, CGI_KEYCODE_N);
            PrintKeyState("O", window, CGI_KEYCODE_O);
            PrintKeyState("P", window, CGI_KEYCODE_P);
            PrintKeyState("Q", window, CGI_KEYCODE_Q);
            PrintKeyState("R", window, CGI_KEYCODE_R);
            PrintKeyState("S", window, CGI_KEYCODE_S);
            PrintKeyState("T", window, CGI_KEYCODE_T);
            PrintKeyState("U", window, CGI_KEYCODE_U);
            PrintKeyState("V", window, CGI_KEYCODE_V);
            PrintKeyState("W", window, CGI_KEYCODE_W);
            PrintKeyState("X", window, CGI_KEYCODE_X);
            PrintKeyState("Y", window, CGI_KEYCODE_Y);
            PrintKeyState("Z", window, CGI_KEYCODE_Z);

            // Number keys
            printf("\n--- Number Keys ---\n");
            PrintKeyState("0", window, CGI_KEYCODE_0);
            PrintKeyState("1", window, CGI_KEYCODE_1);
            PrintKeyState("2", window, CGI_KEYCODE_2);
            PrintKeyState("3", window, CGI_KEYCODE_3);
            PrintKeyState("4", window, CGI_KEYCODE_4);
            PrintKeyState("5", window, CGI_KEYCODE_5);
            PrintKeyState("6", window, CGI_KEYCODE_6);
            PrintKeyState("7", window, CGI_KEYCODE_7);
            PrintKeyState("8", window, CGI_KEYCODE_8);
            PrintKeyState("9", window, CGI_KEYCODE_9);

            // Arrow keys
            printf("\n--- Arrow Keys ---\n");
            PrintKeyState("UP", window, CGI_KEYCODE_UP);
            PrintKeyState("DOWN", window, CGI_KEYCODE_DOWN);
            PrintKeyState("LEFT", window, CGI_KEYCODE_LEFT);
            PrintKeyState("RIGHT", window, CGI_KEYCODE_RIGHT);

            // Special keys
            printf("\n--- Special Keys ---\n");
            PrintKeyState("SPACE", window, CGI_KEYCODE_SPACE);
            PrintKeyState("ENTER", window, CGI_KEYCODE_ENTER);
            PrintKeyState("BACKSPACE", window, CGI_KEYCODE_BACKSPACE);
            PrintKeyState("SHIFT", window, CGI_KEYCODE_SHIFT);
            PrintKeyState("CTRL", window, CGI_KEYCODE_CTRL);
            PrintKeyState("ALT", window, CGI_KEYCODE_ALT);

            // Function keys
            printf("\n--- Function Keys ---\n");
            PrintKeyState("F1", window, CGI_KEYCODE_F1);
            PrintKeyState("F2", window, CGI_KEYCODE_F2);
            PrintKeyState("F3", window, CGI_KEYCODE_F3);
            PrintKeyState("F4", window, CGI_KEYCODE_F4);
            PrintKeyState("F5", window, CGI_KEYCODE_F5);
            PrintKeyState("F6", window, CGI_KEYCODE_F6);
            PrintKeyState("F7", window, CGI_KEYCODE_F7);
            PrintKeyState("F8", window, CGI_KEYCODE_F8);
            PrintKeyState("F9", window, CGI_KEYCODE_F9);
            PrintKeyState("F10", window, CGI_KEYCODE_F10);
            PrintKeyState("F11", window, CGI_KEYCODE_F11);
            PrintKeyState("F12", window, CGI_KEYCODE_F12);

            printf("\n\nPress ESC to exit\n");
        }

        frameCount++;

        // Small delay to prevent CPU spinning
        // You might want to use a proper frame timing mechanism
    }

    // Cleanup
    CGICloseWindow(window);
    CGIWindowCleanup(window);

    printf("\n=== Test Completed ===\n");
    return 0;
}