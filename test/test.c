/*
 * Pixel Art Drawing Application using CGI Library
 * Create colorful pixel art with various drawing tools
 * 
 * Controls:
 * - Left Mouse: Draw with current color
 * - Right Mouse: Erase (draw with background color)
 * - Keys 1-9: Change brush size
 * - R: Red color
 * - G: Green color
 * - B: Blue color
 * - Y: Yellow color
 * - W: White color
 * - K: Black color
 * - C: Clear canvas
 * - S: Rainbow mode toggle
 * - ESC: Exit
 */

#include "../include/cgi.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BRUSH_MIN_SIZE 1
#define BRUSH_MAX_SIZE 20

typedef struct {
    CGIColor_t current_color;
    CGIColor_t bg_color;
    int brush_size;
    int rainbow_mode;
    float hue;
} DrawState;

CGIColor_t HSVtoRGB(float h, float s, float v) {
    float c = v * s;
    float x = c * (1.0f - fabsf(fmodf(h / 60.0f, 2.0f) - 1.0f));
    float m = v - c;
    
    float r, g, b;
    if (h >= 0 && h < 60) {
        r = c; g = x; b = 0;
    } else if (h >= 60 && h < 120) {
        r = x; g = c; b = 0;
    } else if (h >= 120 && h < 180) {
        r = 0; g = c; b = x;
    } else if (h >= 180 && h < 240) {
        r = 0; g = x; b = c;
    } else if (h >= 240 && h < 300) {
        r = x; g = 0; b = c;
    } else {
        r = c; g = 0; b = x;
    }
    
    return CGIMakeColor(
        (unsigned char)((r + m) * 255),
        (unsigned char)((g + m) * 255),
        (unsigned char)((b + m) * 255)
    );
}

void draw_circle(CGIWindow *window, int cx, int cy, int radius, CGIColor_t color) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                int px = cx + x;
                int py = cy + y;
                if (px >= 0 && px < WINDOW_WIDTH && py >= 0 && py < WINDOW_HEIGHT) {
                    CGISetPixel(window, px, py, color);
                }
            }
        }
    }
}

void draw_ui_indicator(CGIWindow *window, DrawState *state) {
    // Draw color indicator in top-left corner
    for (int y = 10; y < 40; y++) {
        for (int x = 10; x < 40; x++) {
            CGISetPixel(window, x, y, state->current_color);
        }
    }
    
    // Draw white border around color indicator
    CGIColor_t white = CGIMakeColor(255, 255, 255);
    for (int x = 8; x < 42; x++) {
        CGISetPixel(window, x, 8, white);
        CGISetPixel(window, x, 41, white);
    }
    for (int y = 8; y < 42; y++) {
        CGISetPixel(window, 8, y, white);
        CGISetPixel(window, 41, y, white);
    }
    
    // Draw brush size indicator (small squares)
    int indicator_x = 50;
    for (int i = 1; i <= state->brush_size && i <= 10; i++) {
        for (int y = 15; y < 25; y++) {
            for (int x = indicator_x; x < indicator_x + 6; x++) {
                CGISetPixel(window, x, y, white);
            }
        }
        indicator_x += 8;
    }
}

int main(void) {
    srand((unsigned int)time(NULL));
    
    CGI *cgi = CGIStart();
    if (!cgi) {
        return 1;
    }
    
    CGIColor_t bg_color = CGIMakeColor(30, 30, 40);
    CGIWindow *window = CGICreateWindow(
        "PixelArtApp",
        "Pixel Art Drawing - Use mouse to draw!",
        100, 100,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        bg_color
    );
    
    if (!window) {
        CGIEnd(cgi);
        return 1;
    }
    
    CGIShowWindow(window);
    
    DrawState state = {
        .current_color = CGIMakeColor(255, 100, 100),
        .bg_color = bg_color,
        .brush_size = 5,
        .rainbow_mode = 0,
        .hue = 0.0f
    };
    
    CGIClearBuffer(window, state.bg_color);
    CGIRefreshBuffer(window);
    
    int last_mouse_x = -1;
    int last_mouse_y = -1;
    
    while (CGIIsWindowOpen(window)) {
        CGIUpdate(cgi);
        
        // Handle keyboard input
        if (CGIIsKeyPressed(window, CGI_input_key_escape)) {
            CGICloseWindow(window);
        }
        
        // Color selection
        if (CGIIsKeyPressed(window, CGI_input_key_r)) {
            state.current_color = CGIMakeColor(255, 50, 50);
            state.rainbow_mode = 0;
        }
        if (CGIIsKeyPressed(window, CGI_input_key_g)) {
            state.current_color = CGIMakeColor(50, 255, 50);
            state.rainbow_mode = 0;
        }
        if (CGIIsKeyPressed(window, CGI_input_key_b)) {
            state.current_color = CGIMakeColor(50, 50, 255);
            state.rainbow_mode = 0;
        }
        if (CGIIsKeyPressed(window, CGI_input_key_y)) {
            state.current_color = CGIMakeColor(255, 255, 50);
            state.rainbow_mode = 0;
        }
        if (CGIIsKeyPressed(window, CGI_input_key_w)) {
            state.current_color = CGIMakeColor(255, 255, 255);
            state.rainbow_mode = 0;
        }
        if (CGIIsKeyPressed(window, CGI_input_key_k)) {
            state.current_color = CGIMakeColor(0, 0, 0);
            state.rainbow_mode = 0;
        }
        
        // Rainbow mode
        if (CGIIsKeyPressed(window, CGI_input_key_s)) {
            state.rainbow_mode = !state.rainbow_mode;
        }
        
        // Clear canvas
        if (CGIIsKeyPressed(window, CGI_input_key_c)) {
            CGIClearBuffer(window, state.bg_color);
        }
        
        // Brush size control
        if (CGIIsKeyPressed(window, CGI_input_key_1)) state.brush_size = 1;
        if (CGIIsKeyPressed(window, CGI_input_key_2)) state.brush_size = 3;
        if (CGIIsKeyPressed(window, CGI_input_key_3)) state.brush_size = 5;
        if (CGIIsKeyPressed(window, CGI_input_key_4)) state.brush_size = 7;
        if (CGIIsKeyPressed(window, CGI_input_key_5)) state.brush_size = 10;
        if (CGIIsKeyPressed(window, CGI_input_key_6)) state.brush_size = 12;
        if (CGIIsKeyPressed(window, CGI_input_key_7)) state.brush_size = 15;
        if (CGIIsKeyPressed(window, CGI_input_key_8)) state.brush_size = 18;
        if (CGIIsKeyPressed(window, CGI_input_key_9)) state.brush_size = 20;
        
        // Get cursor position
        CGIPoint cursor = CGIGetWindowCursorPosition(window);
        
        // Handle mouse drawing
        if (CGIIsMouseButtonPressed(cgi, CGI_input_key_mouse_l)) {
            if (state.rainbow_mode) {
                state.hue += 2.0f;
                if (state.hue >= 360.0f) state.hue = 0.0f;
                state.current_color = HSVtoRGB(state.hue, 1.0f, 1.0f);
            }
            draw_circle(window, cursor.x, cursor.y, state.brush_size, state.current_color);
        }
        
        // Right mouse button for eraser
        if (CGIIsMouseButtonPressed(cgi, CGI_input_key_mouse_r)) {
            draw_circle(window, cursor.x, cursor.y, state.brush_size, state.bg_color);
        }
        
        // Draw UI elements
        draw_ui_indicator(window, &state);
        
        CGIRefreshBuffer(window);
        CGIRefreshWindow(window);
    }
    
    CGIWindowCleanup(window);
    CGIEnd(cgi);
    
    return 0;
}