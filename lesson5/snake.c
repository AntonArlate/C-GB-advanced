#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#define MAX_X 40
#define MAX_Y 20

// Определение цветов ANSI
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

// Направления движения
typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

// Опции
typedef struct options_t
{
    int snake_count;
    short snake2_is_ai;
    char *color1;
    char *color2;
} options_t;

    // До меню инициализируем стандартные значения изменяемых пользователем параметров
    // Переменная опций создаётся глобально, чтобы не перекидывать её постоянно по функциям
    options_t opts = {
        .snake_count = 2,
        .snake2_is_ai = 1,
        .color1 = ANSI_COLOR_CYAN,
        .color2 = ANSI_COLOR_YELLOW};

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
    int x;
    int y;
    struct tail_t *tail;
    size_t tsize;
    Direction direct;
    const char *color;
} snake_t;

void generationFood(food_t *food, snake_t **snakes, int snake_count)
{
    // генерируем позицию
    food->x = (rand() % (MAX_X - 2)) + 1;
    food->y = (rand() % (MAX_Y - 2)) + 1;
    short valid_pos = 1;
    int count_chk = MAX_X * MAX_Y;

    do
    {
        count_chk--;
        // если проверочный счётчик достиг 0, значит подходящей позиции нет на поле
        if (count_chk == 0)
        {
            break;
        }

        // если выяснилось что позиция сгенерированая случайно не доступна
        if (!valid_pos)
        {
            if (food->x < MAX_X - 2)
            {
                // пробуем следующую по X
                food->x++;
                valid_pos = 1;
            }
            else
            {
                food->x = 1;
                if (food->y < MAX_Y - 2)
                {
                    // пробуем следующую по Y
                    food->y++;
                    valid_pos = 1;
                }
                else
                {
                    food->y = 1;
                    valid_pos = 1;
                }
            }
        }

        // Проверяем что позиция не пересекается со змеёй
        for (int i = 0; i < snake_count && valid_pos; i++)
        {
            if (food->x != snakes[i]->x && food->y != snakes[i]->y)
            {
                for (size_t t = 0; t < snakes[i]->tsize; t++)
                {
                    tail_t current_tail = snakes[i]->tail[t];
                    if (food->x == current_tail.x && food->y == current_tail.y)
                    {
                        valid_pos = 0;
                        break;
                    }
                }
            }
            else
            {
                valid_pos = 0;
            }
        }

    } while (!valid_pos);
}

// рисуем еду
void drawFood(food_t food)
{
    COORD coord;
    coord.X = food.x;
    coord.Y = food.y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("+");
}

// инициализация змейки
snake_t initSnake(int x, int y, size_t tsize)
{
    snake_t snake; // создаём объект
    snake.color = ANSI_COLOR_RESET;
    snake.x = x;
    snake.y = y;
    snake.tsize = tsize; // длина хвоста
    snake.tail = (tail_t *)malloc(sizeof(tail_t) * 100);
    for (int i = 0; i < tsize; ++i)
    {
        snake.tail[i].x = x + i + 1;
        snake.tail[i].y = y;
    }
    snake.direct = DOWN;
    return snake;
}

// Кормим змейку
void eatSnake(snake_t *snake, tail_t *tail_tip)
{
    // Увеличить размер массива tsize на 1
    size_t new_tsize = snake->tsize + 1;
    // Перевыделить память для массива tail с увеличенным размером
    snake->tail = (tail_t *)realloc(snake->tail, new_tsize * sizeof(tail_t));
    // Добавить новый элемент tail_t в конец массива
    snake->tail[new_tsize - 1] = *tail_tip;
    // Обновить значение tsize в структуре snake_t
    snake->tsize = new_tsize;
}

void drawField()
{
    // char matrix[MAX_X][MAX_Y];
    for (int i = 0; i < MAX_Y; ++i)
    {
        for (int j = 0; j < MAX_X; ++j)
        {
            if (i == 0 || i == MAX_Y - 1 || j == 0 || j == MAX_X - 1)
            {
                printf("X");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void drawSnakeWindows(snake_t *snake, tail_t *to_remove, int remove_count);
void drawSnakeANSI(snake_t *snake, tail_t *to_remove, int remove_count);
int newDirectIsValid(Direction current_direct, Direction new_direct);
// Функция для перемещения змеи
void moveSnake(snake_t *snake)
{
    Direction direction = snake->direct;
    // Сохраняем последний элемент хвоста
    tail_t *last_tail_p = &(snake->tail[snake->tsize - 1]);
    tail_t last_tail = *last_tail_p;

    // Сохраняем координаты головы
    int prev_head_x = snake->x;
    int prev_head_y = snake->y;

    // Обновляем позицию головы в зависимости от направления
    switch (direction)
    {
    case UP:
        snake->y--;
        break;
    case DOWN:
        snake->y++;
        break;
    case LEFT:
        snake->x--;
        break;
    case RIGHT:
        snake->x++;
        break;
    }

    // Перемещаем хвост
    if (snake->tsize > 0)
    {

        // Обновляем координаты хвоста сдвигаем от нулевого к хвосту
        for (size_t i = snake->tsize - 1; i > 0; i--)
        {
            snake->tail[i] = snake->tail[i - 1];
        }

        // Первый элемент должен равняться старому положению головы
        snake->tail[0].x = prev_head_x;
        snake->tail[0].y = prev_head_y;
    }
    // Вызываем функцию отрисовки
    drawSnakeWindows(snake, &last_tail, 1);
}

// Функция для перерисовки змеи
void drawSnakeWindows(snake_t *snake, tail_t *to_remove, int remove_count)
{
    COORD coord;
    // Удаляем хвост
    for (int i = 0; i < remove_count; i++)
    {
        coord.X = to_remove[i].x;
        coord.Y = to_remove[i].y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        printf(" ");
    }

    // Устанавливаем цветовой код
    printf("%s", snake->color);

    // Рисуем голову
    coord.X = snake->x;
    coord.Y = snake->y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("@");

    // Рисуем хвост
    for (int i = 0; i < snake->tsize; i++)
    {
        coord.X = snake->tail[i].x;
        coord.Y = snake->tail[i].y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        printf("*");
    }

    // Сбрасываем цветовой код
    printf("%s", ANSI_COLOR_RESET);
}

void drawSnakeANSI(snake_t *snake, tail_t *to_remove, int remove_count)
{
    COORD coord;
    // Удаляем хвост
    for (int i = 0; i < remove_count; i++)
    {
        printf("\033[%d;%dH ", to_remove[i].y, to_remove[i].x);
    }

    // Рисуем голову
    printf("\033[%d;%dH@", snake->y, snake->x);

    // Рисуем хвост
    for (int i = 0; i < snake->tsize; i++)
    {
        printf("\033[%d;%dH#", snake->tail[i].y, snake->tail[i].x);
    }
}

// преобразователь нажатых клавиш в направление
// Принимает 2 игрока
void conversionKeyToDirect(char key_detect, snake_t *snake1, snake_t *snake2)
{
    Direction snake1_prev_dir = snake1->direct;
    Direction snake2_prev_dir = snake2->direct;

    switch (key_detect)
    {
    case 'W':
        snake1->direct = UP;
        break;
    case 'A':
        snake1->direct = LEFT;
        break;
    case 'S':
        snake1->direct = DOWN;
        break;
    case 'D':
        snake1->direct = RIGHT;
        break;
    case 'I':
        snake2->direct = UP;
        break;
    case 'J':
        snake2->direct = LEFT;
        break;
    case 'K':
        snake2->direct = DOWN;
        break;
    case 'L':
        snake2->direct = RIGHT;
        break;
    default:
        break;
    }
    // запускаем валидатор
    if (!(newDirectIsValid(snake1_prev_dir, snake1->direct)))
    {
        snake1->direct = snake1_prev_dir;
    }
    if (!(newDirectIsValid(snake2_prev_dir, snake2->direct)))
    {
        snake2->direct = snake2_prev_dir;
    }
}

// валидатор направления
int newDirectIsValid(Direction current_direct, Direction new_direct)
{
    int valid = 0;
    if (current_direct == UP && new_direct != DOWN)
    {
        valid = 1;
    }
    if (current_direct == LEFT && new_direct != RIGHT)
    {
        valid = 1;
    }
    if (current_direct == DOWN && new_direct != UP)
    {
        valid = 1;
    }
    if (current_direct == RIGHT && new_direct != LEFT)
    {
        valid = 1;
    }
    return valid;
}

// Функция AI для змейки. Принимает структуры еды и змеи. Переписывает направление движения провереное валидатором.
void aiSnake(snake_t* snake, food_t* food) {
    int dx = food->x - snake->x;
    int dy = food->y - snake->y;
    Direction new_direct;
    Direction directions[4] = {UP, RIGHT, DOWN, LEFT};

    // Определение направления движения к еде
    if (abs(dx) > abs(dy)) {
        if (dx > 0) {
            new_direct = RIGHT;
        } else {
            new_direct = LEFT;
        }
    } else {
        if (dy > 0) {
            new_direct = DOWN;
        } else {
            new_direct = UP;
        }
    }

    // Попытка найти допустимое направление
    for (int i = 0; i < 4; i++) {
        if (newDirectIsValid(snake->direct, new_direct)) {
            snake->direct = new_direct;
            return; // Допустимое направление найдено, выход из функции
        }
        new_direct = directions[i];
    }

    // Если ни одно направление не является допустимым, змейка не меняет направление
}

// Генератор строк с использованием переменных
char* generateString(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int len = vsnprintf(NULL, 0, format, args) + 1;
    va_end(args);

    char* str = malloc(len);
    va_start(args, format);
    vsnprintf(str, len, format, args);
    va_end(args);

    return str;
}

// Отрисовка меню
char displayMenu(const char *menuItems[], int numItems)
{
    char choice;

    system("cls");
    for (int i = 0; i < numItems; i++)
    {
        printf("%s\n", menuItems[i]);
    }

    scanf("%c", &choice);
    return choice;
}

// преобразователь символа в цветовую константу
char *getColorSequence(char color)
{
    char lowColor = tolower(color);

    switch (lowColor)
    {
    case 'r':
        return ANSI_COLOR_RED;
    case 'g':
        return ANSI_COLOR_GREEN;
    case 'y':
        return ANSI_COLOR_YELLOW;
    case 'b':
        return ANSI_COLOR_BLUE;
    case 'm':
        return ANSI_COLOR_MAGENTA;
    case 'c':
        return ANSI_COLOR_CYAN;
    default:
        return ANSI_COLOR_RESET;
    }
}

// Меню настроек
void options_f()
{
    short running = 1;
    char color;
    char *test = "test";
    while (running)
    {
        const char *menuItems[] = {
            "SNAKE GAME\n",
            "Menu -> Options:",
            generateString("%s1. Color 1 player%s", opts.color1, ANSI_COLOR_RESET),
            generateString("%s2. Color 2 player%s", opts.color2, ANSI_COLOR_RESET),
            generateString("3. Switch AI: current = %d", opts.snake2_is_ai),
            "0. Return",
            "Enter the menu item number: "};
        int numItems = sizeof(menuItems) / sizeof(menuItems[0]);

        int choice = displayMenu(menuItems, numItems);

        switch (choice)
        {
        case '1':
            printf("Choice color then enter firs char RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN or another RESET: ");
            scanf(" %c", &color);
            opts.color1 = getColorSequence(color);
            break;
        case '2':
            printf("Choice color then enter firs char RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN or another RESET");
            scanf(" %c", &color);
            opts.color2 = getColorSequence(color);
            break;
        case '3':
            opts.snake2_is_ai = !opts.snake2_is_ai;
            break;
        case '0':
            running = 0;
            break;
        default:
            printf("Недопустимый выбор. Попробуйте еще раз.\n");
        }
    }
}

// Главное меню
void startMenu()
{
    short running = 1;
    while (running)
    {
        const char *menuItems[] = {
            "SNAKE GAME\n",
            "Main menu:",
            "1. Start 1 player",
            "2. Start 2 player",
            "3. Options",
            "0. Exit",
            "Enter the menu item number: "};
        int numItems = sizeof(menuItems) / sizeof(menuItems[0]);

        int choice = displayMenu(menuItems, numItems);

        switch (choice)
        {
        case '1':
            opts.snake_count = 1;
            return;
            // break;
        case '2':
            opts.snake_count = 2;
            return;
            // break;
        case '3':
            options_f();
            break;
        case '0':
            exit(EXIT_FAILURE);
            break;
        default:
            printf("Недопустимый выбор. Попробуйте еще раз.\n");
        }
    }
}

int main(int argc, char **argv)
{

    srand(time(NULL));
    COORD coord;
    char key_detect;

    // основной цикл программы
    const int TARGET_FPS = 240;                        // Целевая частота кадров в секунду
    const double TARGET_FRAME_TIME = 1.0 / TARGET_FPS; // Время между кадрами в секундах
    double speedSnake = 2;
    double speedFrameTime = 1.0 / speedSnake; // Время между перемещениями
    double SPEED_INCREASE = 1;

    // Вход в меню
    startMenu();
    system("cls");

    // После меню создаём программу на основе заданых параметров.
    int snake_count = opts.snake_count;
    snake_t *snakes[snake_count];
    snake_t snake1 = initSnake(10, 5, 5);
    snake1.color = opts.color1;
    snakes[0] = &snake1;
    snake_t snake2 = initSnake(5, 7, 5);
    snake2.color = opts.color2;
    snakes[1] = &snake2;
    // рисуем поле
    drawField();

    // запускаем первый раз генератор еды
    food_t food;
    generationFood(&food, snakes, snake_count);

    clock_t programStartTime = clock(); // Время старта программы

    double targetFrameTime = (double)programStartTime / CLOCKS_PER_SEC; // Целевое время выполнения следующего кадра
    double targetSpeedTime = (double)programStartTime / CLOCKS_PER_SEC; // Целевое время выполнения следующего перемещения

    char prev_key;
    Direction last_press = DOWN;
    short game_end = 0;

    int score = 0;

    while (!game_end)
    {

        clock_t currentTime = clock(); // Текущее время
        usleep(100);                   // задержка для снижения процессорной нагрузки от вызова основного цикла программы
        // запускаем цикл кадра. Для начала к нему будет привязана только функция отслеживания нажатий
        if ((double)(currentTime) / CLOCKS_PER_SEC > targetFrameTime)
        {
            targetFrameTime += TARGET_FRAME_TIME; // Сдвинуть целевое время выполнения следующего кадра

            // Проверка нажатия клавиш
            if (kbhit())
            {
                key_detect = toupper(getch());
                if (key_detect == 'Q')
                {
                    game_end = 1;
                }

                if (key_detect == 'P')
                {
                    printf("<<< PAUSE Press 'P' to continue >>>");
                    while (1)
                    {
                        key_detect = toupper(getch());
                        if (key_detect == 'P')
                        {
                            // Стираем надпись
                            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                            printf("                                      ");
                            // обновляем таймеры
                            targetFrameTime = clock() / CLOCKS_PER_SEC;                  // Целевое время выполнения следующего кадра
                            targetSpeedTime = clock() / CLOCKS_PER_SEC + speedFrameTime; // Целевое время выполнения следующего перемещения
                            break;
                        }
                    }
                }

                // преобразуем последнюю зафиксированую нажатой клавишу в направление
                if (prev_key != key_detect)
                {
                    prev_key = key_detect;
                    conversionKeyToDirect(key_detect, &snake1, &snake2);
                }
            }
        }

        // цикл отвечающий за перемещение змейки
        if ((double)(currentTime) / CLOCKS_PER_SEC > targetSpeedTime)
        {

            targetSpeedTime += speedFrameTime; // Сдвинуть целевое время выполнения следующего кадра
            for (int i = 0; i < snake_count; i++)
            {
                /* code */

                // Перемещаем змею запоминая координаты кончика хвоста
                tail_t *last_tail_p = &(snakes[i]->tail[snakes[i]->tsize - 1]);
                tail_t last_tail = *last_tail_p;
                moveSnake(snakes[i]);
                // Запускаем AI для второй змейки
                if (opts.snake2_is_ai && i == 1)
                {
                    aiSnake(snakes[i], &food);
                }
                
                // Проверяем не съела ли змея еду
                if (food.x == snakes[i]->x && food.y == snakes[i]->y)
                {
                    // увеличиваем змейку возвращая кончик
                    eatSnake(snakes[i], &last_tail);
                    // генерируем новую еду
                    generationFood(&food, snakes, snake_count);
                    // изменяем скорость
                    speedSnake += SPEED_INCREASE;
                    speedFrameTime = 1.0 / speedSnake;
                    score++;
                }
            }
            drawFood(food);

            // устанавливаем указатель в сторону, чтобы не мешался
            coord.X = 1;
            coord.Y = MAX_Y + 2;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        }
    }
    printf("\n");
    system("cls");
    printf("Score: %d\n", score);
    system("\npause");
    return 0;
}
