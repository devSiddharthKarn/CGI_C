#define CGI_IMPLEMENTATION_MODE_VERBOSE
#include "cgi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BALL_RADIUS 15
#define GRAVITY 0.5
#define DAMPING 0.85
#define TRAIL_LENGTH 50

typedef struct {
    float x, y;
    float vx, vy;
    CGIColor_t color;
} Ball;

typedef struct {
    int x, y;
    CGIColor_t color;
    int age;
} TrailPoint;

// Circle drawing function
void drawCircle(CGIWindow* window, int cx, int cy, int radius, CGIColor_t color) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x*x + y*y <= radius*radius) {
                int px = cx + x;
                int py = cy + y;
                if (px >= 0 && px < WINDOW_WIDTH && py >= 0 && py < WINDOW_HEIGHT) {
                    CGISetPixel(window, px, py, color);
                }
            }
        }
    }
}

// Draw a filled rectangle
void drawRect(CGIWindow* window, int x, int y, int width, int height, CGIColor_t color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int px = x + j;
            int py = y + i;
            if (px >= 0 && px < WINDOW_WIDTH && py >= 0 && py < WINDOW_HEIGHT) {
                CGISetPixel(window, px, py, color);
            }
        }
    }
}

// Generate random color
CGIColor_t randomColor() {
    return CGIMakeColor(
        rand() % 156 + 100,  // 100-255 range for brightness
        rand() % 156 + 100,
        rand() % 156 + 100
    );
}

// Interpolate between two colors
CGIColor_t lerpColor(CGIColor_t a, CGIColor_t b, float t) {
    return CGIMakeColor(
        (unsigned char)(a.r + (b.r - a.r) * t),
        (unsigned char)(a.g + (b.g - a.g) * t),
        (unsigned char)(a.b + (b.b - a.b) * t)
    );
}

int main() {
    srand((unsigned int)time(NULL));
    
    CGI* cgi = CGIStart();
    if (!cgi) {
        printf("Failed to initialize CGI\n");
        return 1;
    }
    
    CGIWindow* window = CGICreateWindow(
        "BouncingBall",
        "CGI Bouncing Ball Demo - Press SPACE to change color, R to reset",
        100, 100,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        CGIMakeColor(15, 15, 25)  // Dark blue background
    );
    
    if (!window) {
        printf("Failed to create window\n");
        CGIEnd(cgi);
        return 1;
    }
    
    CGIShowWindow(window);
    
    // Initialize ball
    Ball ball = {
        .x = WINDOW_WIDTH / 2.0f,
        .y = 100.0f,
        .vx = (rand() % 10 - 5),
        .vy = 0,
        .color = randomColor()
    };
    
    // Trail system
    TrailPoint trail[TRAIL_LENGTH] = {0};
    int trailIndex = 0;
    
    // Platform for bounce
    int platformY = WINDOW_HEIGHT - 80;
    int platformHeight = 20;
    
    // FPS counter
    int frameCount = 0;
    clock_t lastTime = clock();
    float fps = 0;
    
    // Rainbow mode
    int rainbowMode = 0;
    float hue = 0;
    
    CGIColor_t backgroundColor = CGIMakeColor(15, 15, 25);
    
    printf("=== Bouncing Ball Demo ===\n");
    printf("Controls:\n");
    printf("  SPACE - Change ball color\n");
    printf("  R     - Reset ball position\n");
    printf("  T     - Toggle rainbow trail mode\n");
    printf("  Arrow Keys - Apply force to ball\n");
    printf("  ESC   - Close window\n");
    printf("========================\n\n");
    
    // Main loop
    while (CGIIsWindowOpen(window)) {
        // Clear screen with fade effect
        for (int y = 0; y < WINDOW_HEIGHT; y++) {
            for (int x = 0; x < WINDOW_WIDTH; x++) {
                CGIColor_t fadeColor = CGIMakeColor(
                    backgroundColor.r * 0.95f,
                    backgroundColor.g * 0.95f,
                    backgroundColor.b * 0.95f
                );
                CGISetPixel(window, x, y, fadeColor);
            }
        }
        
        // Handle input
        if (CGIIsKeyPressed(window, CGI_input_key_space)) {
            ball.color = randomColor();
            // Small delay to prevent rapid color changes
            for (volatile int i = 0; i < 1000000; i++);
        }
        
        if (CGIIsKeyPressed(window, CGI_input_key_r)) {
            ball.x = WINDOW_WIDTH / 2.0f;
            ball.y = 100.0f;
            ball.vx = (rand() % 10 - 5);
            ball.vy = 0;
        }
        
        if (CGIIsKeyPressed(window, CGI_input_key_t)) {
            rainbowMode = !rainbowMode;
            for (volatile int i = 0; i < 1000000; i++);
        }
        
        // Apply forces with arrow keys
        if (CGIIsKeyPressed(window, CGI_input_key_left)) {
            ball.vx -= 0.5f;
        }
        if (CGIIsKeyPressed(window, CGI_input_key_right)) {
            ball.vx += 0.5f;
        }
        if (CGIIsKeyPressed(window, CGI_input_key_up)) {
            ball.vy -= 1.0f;
        }
        if (CGIIsKeyPressed(window, CGI_input_key_down)) {
            ball.vy += 0.5f;
        }
        
        // Physics
        ball.vy += GRAVITY;
        ball.x += ball.vx;
        ball.y += ball.vy;
        
        // Wall collisions
        if (ball.x - BALL_RADIUS < 0) {
            ball.x = BALL_RADIUS;
            ball.vx = -ball.vx * DAMPING;
        }
        if (ball.x + BALL_RADIUS > WINDOW_WIDTH) {
            ball.x = WINDOW_WIDTH - BALL_RADIUS;
            ball.vx = -ball.vx * DAMPING;
        }
        
        // Platform collision
        if (ball.y + BALL_RADIUS > platformY && ball.y + BALL_RADIUS < platformY + platformHeight) {
            if (ball.vy > 0) {
                ball.y = platformY - BALL_RADIUS;
                ball.vy = -ball.vy * DAMPING;
                
                // Add some horizontal bounce variation
                if (fabs(ball.vx) < 0.5f) {
                    ball.vx = (rand() % 3 - 1) * 2;
                }
            }
        }
        
        // Bottom collision
        if (ball.y + BALL_RADIUS > WINDOW_HEIGHT) {
            ball.y = WINDOW_HEIGHT - BALL_RADIUS;
            ball.vy = -ball.vy * DAMPING;
        }
        
        // Apply friction
        ball.vx *= 0.99f;
        
        // Rainbow mode color cycling
        if (rainbowMode) {
            hue += 0.02f;
            if (hue > 6.0f) hue = 0;
            
            int region = (int)hue;
            float frac = hue - region;
            unsigned char p = 0;
            unsigned char q = (unsigned char)(255 * (1 - frac));
            unsigned char t = (unsigned char)(255 * frac);
            
            switch (region) {
                case 0: ball.color = CGIMakeColor(255, t, p); break;
                case 1: ball.color = CGIMakeColor(q, 255, p); break;
                case 2: ball.color = CGIMakeColor(p, 255, t); break;
                case 3: ball.color = CGIMakeColor(p, q, 255); break;
                case 4: ball.color = CGIMakeColor(t, p, 255); break;
                case 5: ball.color = CGIMakeColor(255, p, q); break;
            }
        }
        
        // Add to trail
        trail[trailIndex].x = (int)ball.x;
        trail[trailIndex].y = (int)ball.y;
        trail[trailIndex].color = ball.color;
        trail[trailIndex].age = TRAIL_LENGTH;
        trailIndex = (trailIndex + 1) % TRAIL_LENGTH;
        
        // Draw trail with fading effect
        for (int i = 0; i < TRAIL_LENGTH; i++) {
            if (trail[i].age > 0) {
                float alpha = (float)trail[i].age / TRAIL_LENGTH;
                CGIColor_t fadeTrailColor = CGIMakeColor(
                    (unsigned char)(trail[i].color.r * alpha),
                    (unsigned char)(trail[i].color.g * alpha),
                    (unsigned char)(trail[i].color.b * alpha)
                );
                
                int trailRadius = (int)(BALL_RADIUS * alpha * 0.7f);
                drawCircle(window, trail[i].x, trail[i].y, trailRadius, fadeTrailColor);
                trail[i].age--;
            }
        }
        
        // Draw platform with gradient
        for (int i = 0; i < platformHeight; i++) {
            float t = (float)i / platformHeight;
            CGIColor_t platformColor = CGIMakeColor(
                (unsigned char)(100 + 50 * t),
                (unsigned char)(100 + 50 * t),
                (unsigned char)(150 + 50 * t)
            );
            drawRect(window, 0, platformY + i, WINDOW_WIDTH, 1, platformColor);
        }
        
        // Draw ball with glow effect
        for (int r = BALL_RADIUS + 8; r > BALL_RADIUS; r--) {
            float glowAlpha = 0.3f * (1.0f - (float)(r - BALL_RADIUS) / 8.0f);
            CGIColor_t glowColor = CGIMakeColor(
                (unsigned char)(ball.color.r * glowAlpha),
                (unsigned char)(ball.color.g * glowAlpha),
                (unsigned char)(ball.color.b * glowAlpha)
            );
            drawCircle(window, (int)ball.x, (int)ball.y, r, glowColor);
        }
        drawCircle(window, (int)ball.x, (int)ball.y, BALL_RADIUS, ball.color);
        
        // Draw highlight on ball
        CGIColor_t highlight = CGIMakeColor(255, 255, 255);
        drawCircle(window, (int)ball.x - 5, (int)ball.y - 5, 3, highlight);
        
        // Draw velocity vector
        int vectorEndX = (int)(ball.x + ball.vx * 5);
        int vectorEndY = (int)(ball.y + ball.vy * 5);
        CGIColor_t vectorColor = CGIMakeColor(255, 200, 0);
        
        // Simple line drawing
        float dx = vectorEndX - ball.x;
        float dy = vectorEndY - ball.y;
        float steps = sqrtf(dx*dx + dy*dy);
        for (int i = 0; i < (int)steps; i++) {
            float t = i / steps;
            int px = (int)(ball.x + dx * t);
            int py = (int)(ball.y + dy * t);
            if (px >= 0 && px < WINDOW_WIDTH && py >= 0 && py < WINDOW_HEIGHT) {
                CGISetPixel(window, px, py, vectorColor);
            }
        }
        
        // FPS counter
        frameCount++;
        clock_t currentTime = clock();
        double elapsed = (double)(currentTime - lastTime) / CLOCKS_PER_SEC;
        if (elapsed >= 1.0) {
            fps = frameCount / (float)elapsed;
            printf("FPS: %.1f | Ball pos: (%.1f, %.1f) | Velocity: (%.2f, %.2f)\n", 
                   fps, ball.x, ball.y, ball.vx, ball.vy);
            frameCount = 0;
            lastTime = currentTime;
        }
        
        CGIRefreshWindow(window);
        CGIRefreshBuffer(window);
    }
    
    printf("\nCleaning up...\n");
    CGIWindowCleanup(window);
    CGIEnd(cgi);
    
    return 0;
}
