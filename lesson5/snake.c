#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#define MAX_X 40
#define MAX_Y 20

// Направления движения
typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

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
    struct tail_t * tail;
    size_t tsize;
} snake_t;

void generationFood(food_t *food, snake_t *snake){
    // генерируем позицию
    food->x = (rand() % (MAX_X - 2)) + 1;
    food->y = (rand() % (MAX_Y - 2)) + 1;
    short valid_pos = 1;
    int count_chk = MAX_X * MAX_Y;

    do
    {
        count_chk --;
        // если проверочный счётчик достиг 0, значит подходящей позиции нет на поле
        if (count_chk == 0)
        {
            break;
        }

        // если выяснилось что позиция сгенерированая случайно не доступна
        if (!valid_pos)
        {   
            if (food->x < MAX_X-2)
            {
                // пробуем следующую по X
                food->x ++;
                valid_pos = 1;
            } else {
                food->x = 1;
                if (food->y < MAX_Y-2)
                {
                    // пробуем следующую по Y
                    food->y ++;
                    valid_pos = 1;
                } else {
                    food->y = 1;
                    valid_pos = 1;
                }
            }
        }
        
        // Проверяем что позиция не пересекается со змеёй
        if (food->x != snake->x && food->y != snake->y)
        {
            for (size_t i = 0; i < snake->tsize; i++){
                tail_t current_tail = snake->tail[i];
                if (food->x == current_tail.x && food->y != current_tail.y)
                {
                    valid_pos = 0;
                    break;
                }
            }
        } else {
            valid_pos = 0;
        }
    } while (!valid_pos);
}

// рисуем еду
void drawFood (food_t food) {
    COORD coord;
    coord.X = food.x;
    coord.Y = food.y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("+");
}

// инициализация змейки
snake_t initSnake(int x, int y, size_t tsize) {
    snake_t snake; // создаём объект
    snake.x = x;
    snake.y = y;
    snake.tsize = tsize; // длина хвоста
    snake.tail = (tail_t *) malloc (sizeof(tail_t) * 100);
    for (int i = 0; i < tsize; ++i)
    {
        snake.tail[i].x = x + i + 1;
        snake.tail[i].y = y;
    }
    return snake;
}

// Кормим хмейку
void eatSnake (snake_t *snake, tail_t *tail_tip){
    // Увеличить размер массива tsize на 1
    size_t new_tsize = snake->tsize + 1;
    // Перевыделить память для массива tail с увеличенным размером
    snake->tail = (tail_t *)realloc(snake->tail, new_tsize * sizeof(tail_t));
    // Добавить новый элемент tail_t в конец массива
    snake->tail[new_tsize - 1] = *tail_tip;
    // Обновить значение tsize в структуре snake_t
    snake->tsize = new_tsize;
}

void drawField(){
    // char matrix[MAX_X][MAX_Y];
    for (int i = 0; i < MAX_Y; ++i)
    {
        for (int j = 0; j < MAX_X; ++j)
        {
            if (i == 0 || i == MAX_Y-1 || j == 0 || j == MAX_X-1){
                printf("X");
            } else
            {
                printf(" ");
            }
        } 
        printf("\n");
    }
}

void drawSnakeWindows(snake_t *snake, tail_t *to_remove, int remove_count);
void drawSnakeANSI(snake_t *snake, tail_t *to_remove, int remove_count);
// Функция для перемещения змеи
void moveSnake(snake_t *snake, Direction direction) {
    // Сохраняем последний элемент хвоста
    tail_t * last_tail_p = &(snake->tail[snake->tsize - 1]);
    tail_t last_tail = *last_tail_p;

    // Сохраняем координаты головы
    int prev_head_x = snake->x;
    int prev_head_y = snake->y;
    
    // Обновляем позицию головы в зависимости от направления
    switch(direction) {
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
    if (snake->tsize > 0) {

        // Обновляем координаты хвоста сдвигаем от нулевого к хвосту
        for (size_t i = snake->tsize - 1; i > 0; i--) {
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
void drawSnakeWindows(snake_t *snake, tail_t *to_remove, int remove_count) {
    COORD coord;
    // Удаляем хвост
    for (int i = 0; i < remove_count; i++) {
        coord.X = to_remove[i].x;
        coord.Y = to_remove[i].y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        printf(" ");
    }
    
    // Рисуем голову
        coord.X = snake->x;
        coord.Y = snake->y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        printf("@");

    // Рисуем хвост
    for (int i = 0; i < snake->tsize; i++) {
        coord.X = snake->tail[i].x;
        coord.Y = snake->tail[i].y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        printf("*");
    }
}

void drawSnakeANSI(snake_t *snake, tail_t *to_remove, int remove_count) {
    COORD coord;
    // Удаляем хвост
    for (int i = 0; i < remove_count; i++) {
        printf("\033[%d;%dH ", to_remove[i].y, to_remove[i].x);
    }
    
    // Рисуем голову
    printf("\033[%d;%dH@", snake->y, snake->x);

    // Рисуем хвост
    for (int i = 0; i < snake->tsize; i++) {
        printf("\033[%d;%dH#", snake->tail[i].y, snake->tail[i].x);
    }
}

// преобразователь нажатых клавиш в направление
Direction conversionKeyToDirect(char key_detect){
    Direction direct;

    switch (key_detect) {
        case 'W':
            direct = UP;
            break;
        case 'A':
            direct = LEFT;
            break;
        case 'S':
            direct = DOWN;
            break;
        case 'D':
            direct = RIGHT;
            break;
        default:
            direct = -1; // Если соответствия не найдено, возвращаем -1
            break;
    }

    return direct;
}

// валидатор направления
int newDirectIsValid (Direction current_direct, Direction new_direct){
    int valid = 0;
    if (current_direct == UP && new_direct != DOWN) {valid = 1;}
    if (current_direct == LEFT && new_direct != RIGHT) {valid = 1;}
    if (current_direct == DOWN && new_direct != UP) {valid = 1;}
    if (current_direct == RIGHT && new_direct != LEFT) {valid = 1;}
    return valid;
}

int	main(int argc, char **argv)
{
    srand(time(NULL));
    COORD coord;
    // создаём поток для чтения нажатых клавиш
    char key_detect;

    // Направление движения
    Direction direction = DOWN;
    snake_t snake = initSnake(10, 5, 5);
    // рисуем поле
    drawField();

    // запускаем первый раз генератор еды
    food_t food;
    generationFood(&food, &snake);


    // основной цикл программы 
    const int TARGET_FPS = 240; // Целевая частота кадров в секунду
    const double TARGET_FRAME_TIME = 1.0 / TARGET_FPS; // Время между кадрами в секундах
    double speedSnake = 2; 
    double speedFrameTime = 1.0 / speedSnake; // Время между перемещениями 
    double SPEED_INCREASE = 1;

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
        usleep(100); // задержка для снижения процессорной нагрузки от вызова основного цикла программы
        // запускаем цикл кадра. Для начала к нему будет привязана только функция отслеживания нажатий
        if ((double)(currentTime) / CLOCKS_PER_SEC > targetFrameTime) {
            targetFrameTime += TARGET_FRAME_TIME; // Сдвинуть целевое время выполнения следующего кадра

            // Проверка нажатия клавиш
            if (kbhit()) {
                key_detect = toupper(getch());
                if (key_detect == 'Q')
                {
                    game_end = 1;
                }

                if (key_detect == 'P')
                {
                    printf("<<< PAUSE Press 'P' to continue >>>");
                    while (1){
                        key_detect = toupper(getch());
                            if (key_detect == 'P') {
                                // Стираем надпись
                                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                                printf("                                      ");
                                // обновляем таймеры
                                targetFrameTime = clock() / CLOCKS_PER_SEC; // Целевое время выполнения следующего кадра
                                targetSpeedTime = clock() / CLOCKS_PER_SEC + speedFrameTime; // Целевое время выполнения следующего перемещения
                                break;
                            }
                    }
                }

                // преобразуем последнюю зафиксированую нажатой клавишу в направление
                if (prev_key != key_detect)
                {
                    prev_key = key_detect;
                    last_press = conversionKeyToDirect(key_detect);
                    if (last_press == -1)
                    {
                        last_press = direction;
                    }
                    
                }        
            }
        }
       
        // цикл отвечающий за перемещение змейки
        if ((double)(currentTime) / CLOCKS_PER_SEC > targetSpeedTime) {

            // если последняя зафиксированая кнопка отличается от предыдущей
            if (last_press != direction)
                {
                    // запускаем валидатор
                    if (newDirectIsValid(direction, last_press))
                    {       
                        direction = last_press;
                    } else {
                        last_press = direction;
                    }
                }

            targetSpeedTime += speedFrameTime; // Сдвинуть целевое время выполнения следующего кадра
            // Перемещаем змею запоминая координаты кончика хвоста
            tail_t * last_tail_p = &(snake.tail[snake.tsize - 1]);
            tail_t last_tail = *last_tail_p;
            moveSnake(&snake, direction);
            // Проверяем не съела ли змея еду
            if (food.x == snake.x && food.y == snake.y)
            {
                // увеличиваем змейку возвращая кончик
                eatSnake(&snake, &last_tail);
                // генерируем новую еду
                generationFood(&food, &snake);
                // изменяем скорость
                speedSnake += SPEED_INCREASE; 
                speedFrameTime = 1.0 / speedSnake;
                score ++;
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
