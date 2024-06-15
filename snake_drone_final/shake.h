#ifndef SNAKE_H
#define SNAKE_H

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>

// Значения по умолчанию
#define MAX_X 40
extern int MAX_Y;
#define SNAKE_COUNT_DEFAULT 5

// Определение цветов ANSI
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_ORANGE "\x1b[38;5;208m"
#define ANSI_COLOR_PURPLE "\x1b[38;5;93m"
#define ANSI_COLOR_PINK "\x1b[38;5;205m"
#define ANSI_COLOR_RESET "\x1b[0m"

// Направления движения
typedef enum
{
    UP,
    RIGHT,
    DOWN,
    LEFT,
} Direction;

// Опции
typedef struct options_t
{
    int snake_count;
    char *colors[9];
} options_t;

typedef struct sensor_t
{
    Direction sens_direct;
    int sens_data;
} sensor_t;

// До меню инициализируем стандартные значения изменяемых пользователем параметров
extern options_t opts;

// еда
typedef struct food_t
{
    int x;
    int y;
} food_t;

// элемент хвоста
typedef struct tail_t
{
    int x;
    int y;
} tail_t;

// змея
typedef struct snake_t
{
    long score;
    double speed;
    int x;
    int y;
    struct tail_t *tail;
    size_t tsize;
    Direction direct;
    const char *color;
    short ID;
    bool control_ai;
} snake_t;

void generationFood(food_t *food, snake_t **snakes, int snake_count);
void drawFood(food_t food);
snake_t* initSnake(int x, int y, size_t tsize);
void eatSnake(snake_t *snake, tail_t *tail_tip);
void drawField();
void drawSnakeWindows(snake_t *snake, tail_t *to_remove, int remove_count);
void drawSnakeANSI(snake_t *snake, tail_t *to_remove, int remove_count);
int newDirectIsValid(Direction current_direct, Direction new_direct);
int checkCoordState(snake_t **snakes, short num_snakes, int chk_x, int chk_y);
void moveSnake(snake_t *snake);
void drawSnakeWindows(snake_t *snake, tail_t *to_remove, int remove_count);
void drawSnakeANSI(snake_t *snake, tail_t *to_remove, int remove_count);
void conversionKeyToDirect(char key_detect, snake_t *snake);
void aiSnake(snake_t *snake, food_t *food);
void cursorDefaulPosition();
void aiSnake2(snake_t **snakes, short num_snakes, snake_t *snake, food_t *food);
char* generateString(const char *format, ...);
void freeGeneratedString(char *str);
char displayMenu(char *menuItems[], int numItems);
char *getColorSequence(char color);
void options_f();
void startMenu();

#endif // SNAKE_H
