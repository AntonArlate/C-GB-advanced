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
#define SNAKE_SPEED_DEFAULT 100

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
    int snake_speed_default;
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

// Генерация еды. Принимает указатель food_t в который записывает новые координаты
void generationFood(food_t *food, snake_t **snakes, int snake_count);

// Алшоритм "сбора"
void eatSnake(snake_t *snake, tail_t *tail_tip);

/*Функции рендера информации на экране*/
// Отображение еды
void drawFood(food_t food);
// Отображение поля с статичными перпятситвиями
void drawField();
/* Отрисовка змейки ориентированая на зауск в стандартной консоли Windows 10. В других системах работоспособность не гарантируется
 tail_t *to_remove : массив устаревших элементов хвоста которые должны быть стёрты на следующем кадре
 int remove_count : количество элементов в переданом массиве*/ 
void drawSnakeWindows(snake_t *snake, tail_t *to_remove, int remove_count);

// Инициализация змейки с настройками по умолчанию
snake_t* initSnake(int x, int y, size_t tsize);

// Стандартная проверка физической возможности смены направления
int newDirectIsValid(Direction current_direct, Direction new_direct);

/* Сложный алгоритм определения свободности ячейки. Может быть заменён на другой с условием возврата валидных значений
0 - free
1 - snake
2 - wall
*/
int checkCoordState(snake_t **snakes, short num_snakes, int chk_x, int chk_y);

// Перемещение змейки на одну позицию. Считывает Direction из переданой структуры и переписывает необходимые данные о местоположении.
void moveSnake(snake_t *snake);

// Функция конвертирует переданую клавишу в направление. Если успешно - обновлят поле direct в структуре
void conversionKeyToDirect(char key_detect, snake_t *snake);

// Тестовый AI алгоритм.
void aiSnake(snake_t *snake, food_t *food);
// AI алгоритм v2
void aiSnake2(snake_t **snakes, short num_snakes, snake_t *snake, food_t *food);

// Установка курсора в позицию за игровым полем.
void cursorDefaulPosition();

/* Функция генерации строк с переменной информацией.
 ВНИМАНИЕ: возвращает указатель на динамически выделенную память. После использования указатель должен быть очищен.*/ 
char* generateString(const char *format, ...);
// Очистка памяти выделенной для строки
void freeGeneratedString(char *str);
/* Отображение нескольких строк меню.
 ВНИМАНИЕ: Ожидает указатели на динамическую память, после отображения память освобождается.*/
char displayMenu(char *menuItems[], int numItems);

/*Функционал меню*/
// Стартовое меню
void startMenu();
// Меню опций
void options_f();

// Преобразователь символа в цветовую константу. Неиспользуемый функционал, требовался для настройки цвета пользователем.
char *getColorSequence(char color);

#endif // SNAKE_H
