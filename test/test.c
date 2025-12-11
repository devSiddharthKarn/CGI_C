/*
 * CGI Library - Dynamic Particle System Demo
 * Demonstrates buffer region updates and FPS display
 */

#include "../include/cgi.h"
// #include "cgi_font_raster.h"
#include "../include/cgi_font.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PARTICLE_COUNT 150
#define PARTICLE_SIZE 3
#define TRAIL_LENGTH 8

typedef struct {
    float x, y;
    float vx, vy;
    CGIColor_t color;
    float trail_x[TRAIL_LENGTH];
    float trail_y[TRAIL_LENGTH];
    int trail_index;
} Particle;

void init_particle(Particle *p, int width, int height) {
    p->x = rand() % width;
    p->y = rand() % height;
    
    float angle = (rand() % 360) * 3.14159f / 180.0f;
    float speed = 1.5f + (rand() % 100) / 50.0f;
    
    p->vx = cos(angle) * speed;
    p->vy = sin(angle) * speed;
    
    unsigned char r = 100 + rand() % 156;
    unsigned char g = 100 + rand() % 156;
    unsigned char b = 100 + rand() % 156;
    p->color = CGIMakeColor(r, g, b);
    
    for (int i = 0; i < TRAIL_LENGTH; i++) {
        p->trail_x[i] = p->x;
        p->trail_y[i] = p->y;
    }
    p->trail_index = 0;
}

void update_particle(Particle *p, int width, int height) {
    // Store old position in trail
    p->trail_x[p->trail_index] = p->x;
    p->trail_y[p->trail_index] = p->y;
    p->trail_index = (p->trail_index + 1) % TRAIL_LENGTH;
    
    // Update position
    p->x += p->vx;
    p->y += p->vy;
    
    // Bounce off edges with slight randomness
    if (p->x < 0 || p->x >= width) {
        p->vx = -p->vx * (0.95f + (rand() % 10) / 100.0f);
        p->x = (p->x < 0) ? 0 : width - 1;
    }
    if (p->y < 0 || p->y >= height) {
        p->vy = -p->vy * (0.95f + (rand() % 10) / 100.0f);
        p->y = (p->y < 0) ? 0 : height - 1;
    }
}

void draw_particle(CGIWindow *window, Particle *p) {
    // Draw trail with fading effect
    for (int i = 0; i < TRAIL_LENGTH; i++) {
        int age = (p->trail_index - i + TRAIL_LENGTH) % TRAIL_LENGTH;
        float alpha = (float)age / TRAIL_LENGTH;
        
        int tx = (int)p->trail_x[i];
        int ty = (int)p->trail_y[i];
        
        // Extract RGB and apply alpha
        unsigned char r = ((p->color.r >> 16) & 0xFF) * alpha;
        unsigned char g = ((p->color.g >> 8) & 0xFF) * alpha;
        unsigned char b = (p->color.b & 0xFF) * alpha;
        
        CGIColor_t trail_color = CGIMakeColor(r, g, b);
        
        // Draw smaller trail pixels
        for (int dy = 0; dy < 2; dy++) {
            for (int dx = 0; dx < 2; dx++) {
                CGISetPixel(window, tx + dx, ty + dy, trail_color);
            }
        }
    }
    
    // Draw main particle
    int px = (int)p->x;
    int py = (int)p->y;
    
    for (int dy = 0; dy < PARTICLE_SIZE; dy++) {
        for (int dx = 0; dx < PARTICLE_SIZE; dx++) {
            CGISetPixel(window, px + dx, py + dy, p->color);
        }
    }
}

int main() {
    srand(time(NULL));
    
    CGI *cgi = CGIStart();
    if (!cgi) {
        printf("Failed to initialize CGI\n");
        return 1;
    }
    
    CGIWindow *window = CGICreateWindow(
        "ParticleDemo",
        "CGI Particle System - Dynamic Buffer Updates",
        100, 100,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        CGIMakeColor(10, 10, 20)
    );
    
    if (!window) {
        printf("Failed to create window\n");
        CGIEnd(cgi);
        return 1;
    }
    
    CGIShowWindow(window);
    
    // Initialize particles
    Particle particles[PARTICLE_COUNT];
    for (int i = 0; i < PARTICLE_COUNT; i++) {
        init_particle(&particles[i], WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    
    // FPS tracking
    clock_t last_time = clock();
    int frame_count = 0;
    int fps = 0;
    char fps_text[32];
    
    CGIColor_t bg_color = CGIMakeColor(10, 10, 20);
    CGIColor_t fps_color = CGIMakeColor(0, 255, 100);
    
    printf("Particle System Running...\n");
    printf("Controls: ESC to exit\n");
    
    while (CGIIsWindowOpen(window)) {
        CGIUpdate(cgi);
        CGIRefreshWindow(window,CGI_window_refresh_mode_rapid);
        // Check for ESC key
        if (CGIIsKeyPressed(window, CGI_input_key_escape)) {
            break;
        }
        
        // Clear the entire buffer
        CGIClearBuffer(window, bg_color);
        
        // Update and draw all particles
        for (int i = 0; i < PARTICLE_COUNT; i++) {
            update_particle(&particles[i], WINDOW_WIDTH, WINDOW_HEIGHT);
            draw_particle(window, &particles[i]);
        }
        
        // Calculate FPS
        frame_count++;
        clock_t current_time = clock();
        double elapsed = (double)(current_time - last_time) / CLOCKS_PER_SEC;
        
        if (elapsed >= 0.5) {
            fps = (int)(frame_count / elapsed);
            frame_count = 0;
            last_time = current_time;
        }
        
        // Draw FPS counter using region update
        snprintf(fps_text, sizeof(fps_text), "FPS: %d", fps);
        
        // Clear FPS region
        CGIClearBufferRegion(window, 5, 5, 120, 25, CGIMakeColor(0, 0, 0));
        
        // Write FPS text
        CGIWriteText(window, fps_text, 10, 10, 1, 1, 2, 2, 0, CGI_false, fps_color);
        
        // Refresh only the FPS region (efficient!)
        CGIRefreshBufferRegion(window, 5, 5, 120, 25);
        
        // Refresh the main particle area
        CGIRefreshBufferRegion(window, 0, 30, WINDOW_WIDTH, WINDOW_HEIGHT - 30);
    }
    
    printf("Shutting down...\n");
    CGICloseWindow(window);
    CGIWindowCleanup(window);
    CGIEnd(cgi);
    
    return 0;
}