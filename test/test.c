#include "cgi.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PARTICLES 2000
#define GRAVITY 0.3f
#define BOUNCE_DAMPING 0.7f
#define PARTICLE_SIZE 3

typedef struct {
    float x, y;
    float vx, vy;
    CGIColor_t color;
    int active;
    float life;
} Particle;

Particle particles[MAX_PARTICLES];
int particle_count = 0;

void init_particle(Particle* p, float x, float y) {
    p->x = x;
    p->y = y;
    p->vx = ((rand() % 100) - 50) / 10.0f;
    p->vy = ((rand() % 100) - 50) / 10.0f;
    p->active = 1;
    p->life = 1.0f;
    
    // Random vibrant colors
    int color_choice = rand() % 6;
    switch(color_choice) {
        case 0: p->color = CGIMakeColor(255, 100, 100); break;
        case 1: p->color = CGIMakeColor(100, 255, 100); break;
        case 2: p->color = CGIMakeColor(100, 100, 255); break;
        case 3: p->color = CGIMakeColor(255, 255, 100); break;
        case 4: p->color = CGIMakeColor(255, 100, 255); break;
        case 5: p->color = CGIMakeColor(100, 255, 255); break;
    }
}

void update_particles(CGIWindow* window) {
    unsigned int width = *(unsigned int*)CGIPerformQuery(CGI_query_window_width_unsigned_int, NULL, window);
    unsigned int height = *(unsigned int*)CGIPerformQuery(CGI_query_window_height_unsigned_int, NULL, window);
    
    for(int i = 0; i < MAX_PARTICLES; i++) {
        if(!particles[i].active) continue;
        
        // Apply gravity
        particles[i].vy += GRAVITY;
        
        // Update position
        particles[i].x += particles[i].vx;
        particles[i].y += particles[i].vy;
        
        // Bounce off walls
        if(particles[i].x < 0) {
            particles[i].x = 0;
            particles[i].vx = -particles[i].vx * BOUNCE_DAMPING;
        }
        if(particles[i].x >= width) {
            particles[i].x = width - 1;
            particles[i].vx = -particles[i].vx * BOUNCE_DAMPING;
        }
        
        // Bounce off floor and ceiling
        if(particles[i].y >= height) {
            particles[i].y = height - 1;
            particles[i].vy = -particles[i].vy * BOUNCE_DAMPING;
            particles[i].vx *= 0.95f; // Friction
        }
        if(particles[i].y < 0) {
            particles[i].y = 0;
            particles[i].vy = -particles[i].vy * BOUNCE_DAMPING;
        }
        
        // Fade out over time
        particles[i].life -= 0.001f;
        if(particles[i].life <= 0) {
            particles[i].active = 0;
        }
    }
}

void draw_particle(CGIWindow* window, Particle* p) {
    if(!p->active) return;
    
    CGIColor_t color = p->color;
    // Apply life fading
    color.r *= p->life;
    color.g *= p->life;
    color.b *= p->life;
    
    // Draw particle with size
    for(int dy = -PARTICLE_SIZE/2; dy <= PARTICLE_SIZE/2; dy++) {
        for(int dx = -PARTICLE_SIZE/2; dx <= PARTICLE_SIZE/2; dx++) {
            if(dx*dx + dy*dy <= (PARTICLE_SIZE/2)*(PARTICLE_SIZE/2)) {
                CGIPixel pixel;
                pixel.point.x = (int)(p->x + dx);
                pixel.point.y = (int)(p->y + dy);
                pixel.color = color;
                CGIPerformCommand(CGI_command_window_set_pixel, &pixel, &window);
                // CGISetPixel(window,pixel.point.x,pixel.point.y,pixel.color);
            }
        }
    }
}

void spawn_particles(CGIWindow* window, int x, int y, int count) {
    for(int i = 0; i < count; i++) {
        int idx = particle_count % MAX_PARTICLES;
        init_particle(&particles[idx], x, y);
        particle_count++;
    }
}

int main() {
    srand(time(NULL));

    CGIBool logic;
    
    CGI* cgi = NULL;
    if(!CGIPerformCommand(CGI_command_CGI_start, NULL, &cgi)) {
        return 1;
    }
    
    CGIColor_t bg_color = CGIMakeColor(15, 15, 25);
    CGIWindow* window = CGICreateWindow("ParticleSandbox", "Particle Physics Sandbox - Click and Drag!", 
                                        100, 100, 1000, 700, bg_color);
    if(!window) {
        CGIPerformCommand(CGI_command_CGI_end, NULL, &cgi);
        return 1;
    }
    
    CGIShowWindow(window);
    
    int last_mouse_x = -1;
    int last_mouse_y = -1;
    int frames = 0;
    
    while(CGIIsWindowOpen(window)) {
        CGIPerformCommand(CGI_command_window_refresh_window, NULL, &window);
        
        CGIPoint cursor = *(CGIPoint*)CGIPerformQuery(CGI_query_window_cursor_position_CGIPoint, NULL, window);
        CGIBool focused = *(CGIBool*)CGIPerformQuery(CGI_query_window_focus_status_CGIBool, NULL, window);
        
        // Clear with fade effect for trails
        if(frames % 1 == 0) {
            CGIColor_t fade_color = CGIMakeColor(15, 15, 25);
            CGIPerformCommand(CGI_command_window_clear_buffer, &fade_color, &window);
        }
        
        // Spawn particles on mouse drag
        if(focused && CGIIsKeyPressed(window, CGI_input_key_space)) {
            spawn_particles(window, cursor.x, cursor.y, 5);
        }
        
        // Left click spawns particles
        if(focused) {
            if(cursor.x >= 0 && cursor.y >= 0 && 
               cursor.x < *(unsigned int*)CGIPerformQuery(CGI_query_window_width_unsigned_int, NULL, window) &&
               cursor.y < *(unsigned int*)CGIPerformQuery(CGI_query_window_height_unsigned_int, NULL, window)) {
                
                // Detect mouse movement for drawing
                if(last_mouse_x != cursor.x || last_mouse_y != cursor.y) {
                    spawn_particles(window, cursor.x, cursor.y, 3);
                    last_mouse_x = cursor.x;
                    last_mouse_y = cursor.y;
                }
            }
        }
        
        // Clear all particles with 'C' key
        if(CGIIsKeyPressed(window, CGI_input_key_c)) {
            for(int i = 0; i < MAX_PARTICLES; i++) {
                particles[i].active = 0;
            }
        }
        
        // Explosion with 'E' key
        if(CGIIsKeyPressed(window, CGI_input_key_e) && focused) {
            spawn_particles(window, cursor.x, cursor.y, 100);
        }
        
        // Update and draw all particles
        update_particles(window);
        
        for(int i = 0; i < MAX_PARTICLES; i++) {
            draw_particle(window, &particles[i]);
        }
        
        CGIPerformCommand(CGI_command_window_refresh_buffer, NULL, &window);
        
        frames++;
        
        // Exit with ESC
        if(CGIIsKeyPressed(window, CGI_input_key_escape)) {
            CGICloseWindow(window);
        }
    }
    
    CGIWindowCleanup(window);
    CGIPerformCommand(CGI_command_CGI_end, NULL, &cgi);
    
    return 0;
}