#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>

#define MAX_X 20
#define MAX_Y 20

// Направления движения
typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

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

void drawField(){
    char matrix[MAX_X][MAX_Y];
    for (int i = 0; i < MAX_X; ++i)
    {
        for (int j = 0; j < MAX_Y; ++j)
        {
            matrix[i][j] = ' ';
        }
    }
}

void draw_snake(snake_t *snake, tail_t *to_remove, int remove_count);
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
    draw_snake(snake, &last_tail, 1);
}

// Функция для перерисовки змеи
void draw_snake(snake_t *snake, tail_t *to_remove, int remove_count) {
    
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

// Детектор нажатия клавиш
void *keyboardThread(void *buffer_link) {
    while(1) {
        usleep(100);
        // Считываем ввод с клавиатуры
        * (char  *)buffer_link = toupper(getch());
    }
    return NULL;
}

// преобразователь нажатых клавиш в направление
Direction conversionKeyToDirect(char key_detect){
    Direction direct;
    if (key_detect == 'W')
    {
        direct = UP;
    }
    if (key_detect == 'A')
    {
        direct = LEFT;
    }
    if (key_detect == 'S')
    {
        direct = DOWN;
    }
    if (key_detect == 'D')
    {
        direct = RIGHT;
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
    // создаём поток для чтения нажатых клавиш
    char key_detect;
    pthread_t tid;
        if(pthread_create(&tid, NULL, keyboardThread, &key_detect) != 0) {
        printf("Thread creation error.\n");
        return 1;
    }

    // Направление движения
    Direction direction = DOWN;
    snake_t snake = initSnake(10, 5, 5);
    // рисуем поле
    drawField();

    // основной цикл программы 
    unsigned long long cicle = 0;
    unsigned long long cicle_limit = 24000000 * 100;
    unsigned long long move_dalay = 3000000;
    unsigned long long target_move_cile = move_dalay;

    char prev_key;
    Direction last_press = DOWN;
    while (cicle < cicle_limit)
    {
        cicle += 1;
        // задержка для торможения программы из расчёта ~240 кадров в сек
        usleep(4);

        // преобразуем последнюю зафиксированую нажатой клавишу в направление
        if (prev_key != key_detect)
        {
            prev_key = key_detect;
            last_press = conversionKeyToDirect(key_detect);
        }        

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
        

        // цикл отвечающий за перемещение змейки
        if (cicle > target_move_cile)
        {
            // сразу переписываем время следующего движения
            target_move_cile = cicle + move_dalay;
            // Перемещаем змею
            moveSnake(&snake, direction);
        }
    }
    
    system("pause");
    return 0;
}
