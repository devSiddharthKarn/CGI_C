#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../include/cgi.h"
#include "../include/cgi_font.h"
#include "time.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stb_image_write.h"

#define GRID_SIZE 20
#define CELL_SIZE 25
#define WINDOW_SIZE (GRID_SIZE * CELL_SIZE)
#define MAX_SNAKE_LENGTH (GRID_SIZE * GRID_SIZE)

typedef struct
{
    int x;
    int y;
} Position;

typedef struct
{
    Position body[MAX_SNAKE_LENGTH];
    int length;
    int dx;
    int dy;
} Snake;

typedef struct
{
    Position pos;
    CGIBool active;
} Food;

void init_snake(Snake *snake)
{
    snake->length = 3;
    snake->dx = 1;
    snake->dy = 0;
    for (int i = 0; i < snake->length; i++)
    {
        snake->body[i].x = GRID_SIZE / 2 - i;
        snake->body[i].y = GRID_SIZE / 2;
    }
}

void spawn_food(Food *food, Snake *snake)
{
    CGIBool valid;
    do
    {
        valid = CGI_true;
        food->pos.x = rand() % GRID_SIZE;
        food->pos.y = rand() % GRID_SIZE;

        for (int i = 0; i < snake->length; i++)
        {
            if (snake->body[i].x == food->pos.x && snake->body[i].y == food->pos.y)
            {
                valid = CGI_false;
                break;
            }
        }
    } while (!valid);
    food->active = CGI_true;
}

CGIBool check_collision(Snake *snake)
{
    Position head = snake->body[0];

    if (head.x < 0 || head.x >= GRID_SIZE || head.y < 0 || head.y >= GRID_SIZE)
    {
        return CGI_true;
    }

    for (int i = 1; i < snake->length; i++)
    {
        if (snake->body[i].x == head.x && snake->body[i].y == head.y)
        {
            return CGI_true;
        }
    }

    return CGI_false;
}

void update_snake(Snake *snake, Food *food, int *score)
{
    Position new_head = {
        snake->body[0].x + snake->dx,
        snake->body[0].y + snake->dy};

    for (int i = snake->length - 1; i > 0; i--)
    {
        snake->body[i] = snake->body[i - 1];
    }
    snake->body[0] = new_head;

    if (food->active && new_head.x == food->pos.x && new_head.y == food->pos.y)
    {
        if (snake->length < MAX_SNAKE_LENGTH)
        {
            snake->length++;
            (*score) += 10;
        }
        food->active = CGI_false;
    }
}

void draw_game(CGIWindow *window, Snake *snake, Food *food, int score, double fps)
{
    CGIClearBuffer(window, CGIMakeColor(20, 20, 30));

    for (int i = 0; i < snake->length; i++)
    {
        int x = snake->body[i].x * CELL_SIZE;
        int y = snake->body[i].y * CELL_SIZE;
        CGIColor_t color = (i == 0) ? CGIMakeColor(100, 255, 100) : CGIMakeColor(50, 200, 50);
        CGIClearBufferRegion(window, x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, color);
    }

    if (food->active)
    {
        int fx = food->pos.x * CELL_SIZE;
        int fy = food->pos.y * CELL_SIZE;
        CGIClearBufferRegion(window, fx + 1, fy + 1, CELL_SIZE - 2, CELL_SIZE - 2, CGIMakeColor(255, 50, 50));
    }

    char fps_text[50];
    snprintf(fps_text, 50, "FPS: %.1f  Score: %d", fps, score);
    CGIWriteText(window, fps_text, 5, 5, 1, 1, 2, 2, 0, CGI_false, CGIMakeColor(255, 255, 255));

    CGIRefreshBuffer(window);
}

int main()
{
    srand((unsigned int)time(NULL));

    CGIWindow *window = CGICreateWindow("SnakeGame", "Snake Game - Use Arrow Keys", 100, 100,
                                        WINDOW_SIZE, WINDOW_SIZE, CGIMakeColor(20, 20, 30));
    CGIShowWindow(window);

    Snake snake;
    Food food = {.active = CGI_false};
    int score = 0;
    double move_timer = 0;
    double move_delay = 0.15;

    init_snake(&snake);
    spawn_food(&food, &snake);

    clock_t last_time = clock();
    double fps = 0.0;

    while (CGIIsWindowOpen(window))
    {
        clock_t current_time = clock();
        double delta_time = (double)(current_time - last_time) / CLOCKS_PER_SEC;
        last_time = current_time;

        if (delta_time > 0)
        {
            fps = 1.0 / delta_time;
        }

        CGIRefreshWindow(window, CGI_window_refresh_mode_rapid);

        if (CGIIsWindowKeyDown(window, CGI_KEYCODE_UP) && snake.dy == 0)
        {
            snake.dx = 0;
            snake.dy = -1;
        }
        if (CGIIsWindowKeyDown(window, CGI_KEYCODE_DOWN) && snake.dy == 0)
        {
            snake.dx = 0;
            snake.dy = 1;
        }
        if (CGIIsWindowKeyDown(window, CGI_KEYCODE_LEFT) && snake.dx == 0)
        {
            snake.dx = -1;
            snake.dy = 0;
        }
        if (CGIIsWindowKeyDown(window, CGI_KEYCODE_RIGHT) && snake.dx == 0)
        {
            snake.dx = 1;
            snake.dy = 0;
        }
        if (CGIIsWindowKeyDown(window, CGI_KEYCODE_ESCAPE))
        {
            CGICloseWindow(window);
        }

        move_timer += delta_time;
        if (move_timer >= move_delay)
        {
            move_timer = 0;

            update_snake(&snake, &food, &score);

            if (check_collision(&snake))
            {
                init_snake(&snake);
                score = 0;
                food.active = CGI_false;
            }

            if (!food.active)
            {
                spawn_food(&food, &snake);
            }
        }

        draw_game(window, &snake, &food, score, fps);
    }

    CGIWindowCleanup(window);
    return 0;
}