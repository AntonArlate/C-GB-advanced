#include "shake.h"

// До меню инициализируем стандартные значения изменяемых пользователем параметров
// Переменная опций создаётся глобально, чтобы не перекидывать её постоянно по функциям
int MAX_Y = 20;
options_t opts = {
    .snake_count = SNAKE_COUNT_DEFAULT,
    .snake_speed_default = SNAKE_SPEED_DEFAULT,
    .colors[0] = ANSI_COLOR_RED,
    .colors[1] = ANSI_COLOR_GREEN,
    .colors[2] = ANSI_COLOR_YELLOW,
    .colors[3] = ANSI_COLOR_BLUE,
    .colors[4] = ANSI_COLOR_MAGENTA,
    .colors[5] = ANSI_COLOR_CYAN,
    .colors[6] = ANSI_COLOR_ORANGE,
    .colors[7] = ANSI_COLOR_PURPLE,
    .colors[8] = ANSI_COLOR_PINK,
};

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
snake_t *initSnake(int x, int y, size_t tsize)
{
    // snake_t snake; // создаём объект
    snake_t *snake = malloc(sizeof(snake_t));
    snake->score = 0;
    snake->speed = 2;
    snake->color = ANSI_COLOR_RESET;
    snake->x = x;
    snake->y = y;
    snake->tsize = tsize; // длина хвоста
    snake->tail = (tail_t *)malloc(sizeof(tail_t) * 100);
    for (int i = 0; i < tsize; ++i)
    {
        snake->tail[i].x = x + i + 1;
        snake->tail[i].y = y;
    }
    snake->direct = DOWN;
    snake->control_ai = true;
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

/*
// Функция проверки доступности координаты для перемещения
0 - free
1 - snake
2 - wall
*/
int checkCoordState(snake_t **snakes, short num_snakes, int chk_x, int chk_y)
{
    int position_status = 0;

    // Проверяем пересечение со стенами
    if (chk_x <= 0 ||
        chk_x >= MAX_X - 1 ||
        chk_y <= 0 ||
        chk_y >= MAX_Y - 1)
    {
        return 2;
    }

    // Перебираем всех змеек проверяя возможность соседства
    for (short i = 0; i < num_snakes; i++)
    {
        snake_t *another_snake = snakes[i];
        int diff_coordinates; // это расстояние но несколько переопределённое, с учётом что хвост может изгибаться
        diff_coordinates = abs(chk_x - another_snake->x) + abs(chk_y - another_snake->y);
        if (diff_coordinates <= another_snake->tsize)
        {
            for (size_t tail_index = 0; tail_index < another_snake->tsize; tail_index++)
            {
                if (chk_x == another_snake->tail[tail_index].x &&
                    chk_y == another_snake->tail[tail_index].y)
                {
                    return 1;
                }
            }

            short collisions = 0;
            if (chk_x == another_snake->x &&
                chk_y == another_snake->y)
            {
                collisions++;
                if (collisions > 1)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

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

// преобразователь нажатых клавиш в направление
// Принимает 2 игрока
void conversionKeyToDirect(char key_detect, snake_t *snake)
{
    Direction snake_prev_dir = snake->direct;

    switch (key_detect)
    {
    case 'W':
        snake->direct = UP;
        break;
    case 'A':
        snake->direct = LEFT;
        break;
    case 'S':
        snake->direct = DOWN;
        break;
    case 'D':
        snake->direct = RIGHT;
        break;
    default:
        break;
    }
    // запускаем валидатор
    if (!(newDirectIsValid(snake_prev_dir, snake->direct)))
    {
        snake->direct = snake_prev_dir;
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
void aiSnake(snake_t *snake, food_t *food)
{
    int dx = food->x - snake->x;
    int dy = food->y - snake->y;
    Direction new_direct;
    Direction directions[4] = {UP, RIGHT, DOWN, LEFT};

    // Определение направления движения к еде
    if (abs(dx) > abs(dy))
    {
        if (dx > 0)
        {
            new_direct = RIGHT;
        }
        else
        {
            new_direct = LEFT;
        }
    }
    else
    {
        if (dy > 0)
        {
            new_direct = DOWN;
        }
        else
        {
            new_direct = UP;
        }
    }

    // Попытка найти допустимое направление
    for (int i = 0; i < 4; i++)
    {
        if (newDirectIsValid(snake->direct, new_direct))
        {
            snake->direct = new_direct;
            return; // Допустимое направление найдено, выход из функции
        }
        new_direct = directions[i];
    }

    // Если ни одно направление не является допустимым, змейка не меняет направление
}

void cursorDefaulPosition (){
    COORD coord;
    coord.X = 1;
    coord.Y = MAX_Y + 2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void aiSnake2(snake_t **snakes, short num_snakes, snake_t *snake, food_t *food)
{
    cursorDefaulPosition();
    int up_position_status = checkCoordState(snakes, num_snakes, snake->x, snake->y - 1);
    int right_position_status = checkCoordState(snakes, num_snakes, snake->x + 1, snake->y);
    int down_position_status = checkCoordState(snakes, num_snakes, snake->x, snake->y + 1);
    int left_position_status = checkCoordState(snakes, num_snakes, snake->x - 1, snake->y);

    Direction go_in = 9999;
    int algorytm_steps = 5;
    sensor_t *sensors_priority [algorytm_steps];
    short chk_counter = 0; 

    // Создаём массив приоритетов проверки
    while (chk_counter < algorytm_steps && go_in > 999)
    {
        sensors_priority[chk_counter] = malloc(sizeof(sensor_t));
        switch (chk_counter)
        {
        // проверяем лево или право взависимости от той строки на которой находимся
        case 0:
            // если мы сейчас в первом столбце переносимся к 4-ому кейсу
            if (snake->x == 1)
            {
                sensors_priority[chk_counter]->sens_direct = UP;
                sensors_priority[chk_counter]->sens_data = 1;
                break;
            }
            
            if (!(snake->y & 1))
                {
                    sensors_priority[chk_counter]->sens_direct = LEFT;
                    if (snake->x <= 2 && down_position_status != 2)
                        sensors_priority[chk_counter]->sens_data = 2;
                    else
                        sensors_priority[chk_counter]->sens_data = left_position_status;
                } else
                {
                    sensors_priority[chk_counter]->sens_direct = RIGHT;
                    sensors_priority[chk_counter]->sens_data = right_position_status;
                }
            break;
        
        // следующий приоритет верх если прошлый был 1
        case 1:
            if (sensors_priority[chk_counter-1]->sens_data == 1)
            {
                sensors_priority[chk_counter]->sens_direct = UP;
                sensors_priority[chk_counter]->sens_data = up_position_status;
            } else {
                sensors_priority[chk_counter]->sens_direct = DOWN;
                sensors_priority[chk_counter]->sens_data = down_position_status;
            }
            break;

        // тоже что 1 но инвертировано
        case 2:
            if (sensors_priority[chk_counter-1]->sens_direct == UP)
            {
                sensors_priority[chk_counter]->sens_direct = DOWN;
                sensors_priority[chk_counter]->sens_data = down_position_status;
            } else {
                sensors_priority[chk_counter]->sens_direct = UP;
                sensors_priority[chk_counter]->sens_data = up_position_status;
            }
            break;

        // опять проверяем лево и право инвертировано от 0
        case 3:
            if (sensors_priority[0]->sens_direct == LEFT)
            {
                sensors_priority[chk_counter]->sens_direct = RIGHT;
                sensors_priority[chk_counter]->sens_data = right_position_status;
            } else {
                    sensors_priority[chk_counter]->sens_direct = LEFT;
                    if (snake->x == 2 && down_position_status != 2)
                        sensors_priority[chk_counter]->sens_data = 2;
                    else
                        sensors_priority[chk_counter]->sens_data = left_position_status;
            }
            break;

        // проверены все стандартные условия, теперь проверяем случай в левом столбце
        case 4:
        
            if (snake->x == 1)
            {
                if (up_position_status > 0)
                {
                    sensors_priority[chk_counter]->sens_direct = RIGHT;
                    sensors_priority[chk_counter]->sens_data = right_position_status;
                    break;
                }
                if (up_position_status == 0)
                {
                    sensors_priority[chk_counter]->sens_direct = UP;
                    sensors_priority[chk_counter]->sens_data = up_position_status;
                } else if (right_position_status == 0)
                {
                    sensors_priority[chk_counter]->sens_direct = RIGHT;
                    sensors_priority[chk_counter]->sens_data = right_position_status;
                } else if (down_position_status == 0)
                {
                    sensors_priority[chk_counter]->sens_direct = DOWN;
                    sensors_priority[chk_counter]->sens_data = down_position_status;
                }
            } 
            break;
        default:
            break;
        }

        if (sensors_priority[chk_counter] != NULL && sensors_priority[chk_counter]->sens_data == 0)
        {
            go_in = sensors_priority[chk_counter]->sens_direct;
            break;
        }
        
        chk_counter++;
    }
    // если go_in так и не сменилось значит выходим не меняя направления (значит уже погибли)
    if (go_in < 999)
        snake->direct = go_in;

    // очищаем память
    while (chk_counter > 0)
    {
        chk_counter --;
        free(sensors_priority[chk_counter]);
    }
    
}

// Генератор строк с использованием переменных
char *generateString(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int len = vsnprintf(NULL, 0, format, args) + 1;
    va_end(args);

    char *str = malloc(len);
    va_start(args, format);
    vsnprintf(str, len, format, args);
    va_end(args);

    return str;
}

void freeGeneratedString(char *str) {
    free(str);
}

// Отрисовка меню
char displayMenu(char *menuItems[], int numItems)
{
    char choice;

    system("cls");
    for (int i = 0; i < numItems; i++)
    {
        printf("%s\n", menuItems[i]);
    }

    scanf("%c", &choice);

    // Освобождаем память, выделенную для всех строк меню
    for (int i = 0; i < numItems; i++)
    {
        freeGeneratedString(menuItems[i]);
    }
    // free(menuItems);

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
        char *menuItems[] = {
            generateString("SNAKE GAME\n"),
            generateString("Menu -> Options:"),
            generateString("1. Change snakes: current = %d", opts.snake_count),
            generateString("2. Change default speed: current = %d", opts.snake_speed_default),
            generateString("0. Return"),
            generateString("Enter the menu item number: ")};
        int numItems = sizeof(menuItems) / sizeof(menuItems[0]);

        int choice = displayMenu(menuItems, numItems);
        int input;
        switch (choice)
        {
        case '1':
            printf("Enter number (1-9): ");
            scanf("%d", &input);
            if (input >= 1 && input <= 9)
            {
                opts.snake_count = input;
            }
            else
            {
                printf("Invalid input. Please enter a digit from 1 to 9.\n");
            }
            break;
        case '2':
            printf("Enter new speed steps per second (1-200): ");
            scanf("%d", &input);
            if (input >= 1 && input <= 200)
            {
                opts.snake_speed_default = input;
            }
            else
            {
                printf("Invalid input. Please enter a from 1 to 200.\n");
            }
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
        char *menuItems[] = {
            generateString("SNAKE GAME\n"),
            generateString("Main menu:"),
            generateString("1. Start"),
            generateString("2. Options"),
            generateString("0. Exit"),
            generateString("Enter the menu item number: ")};
        int numItems = sizeof(menuItems) / sizeof(menuItems[0]);

        int choice = displayMenu(menuItems, numItems);

        switch (choice)
        {
        case '1':
            // opts.snake_count = 1;
            return;
            // break;
        case '2':
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
    double DEFAULT_SPEED = 100;                          // количество шагов в секунду
    double DEFAULT_SPEED_TIME = 1.0 / DEFAULT_SPEED;   // Время между перемещениями
    double SPEED_INCREASE = 1;
    double SPEED_DECREASE = 1.0 / (MAX_X * MAX_Y);

    // Вход в меню
    startMenu();
    system("cls");

    // Рисуем поле
    // Для AI алгоритма нужна чётная высота. Проверяем и уменьшаем на 1 если необходимо
    MAX_Y = MAX_Y & 1 ? MAX_Y - 1 : MAX_Y;
    drawField();

    clock_t programStartTime = clock(); // Время старта программы

    // После меню создаём программу на основе заданых параметров.
    int snake_count = opts.snake_count;
    double target_time_snakesMove[snake_count];
    snake_t *snakes[snake_count];
    for (short i = 0; i < snake_count; i++)
    {
        snake_t *new_snake = initSnake(i * 2 + 2, i * 2 + 2, 1);
        new_snake->color = opts.colors[i];
        new_snake->speed = opts.snake_speed_default;
        new_snake->ID = i;
        snakes[i] = new_snake;
        target_time_snakesMove[i] = (double)programStartTime / CLOCKS_PER_SEC + DEFAULT_SPEED_TIME;
    }
    // snakes[0]->control_ai = false;
    short current_control = -1; // номер змеки в порядке массива

    // запускаем первый раз генератор еды
    food_t food;
    generationFood(&food, snakes, snake_count);

    double target_time_chkKey = (double)programStartTime / CLOCKS_PER_SEC; // Целевое время выполнения следующего цикла проверки клавиш
    double target_time_move = (double)programStartTime / CLOCKS_PER_SEC;   // Целевое время выполнения следующего перемещения

    char prev_key;
    Direction last_press = DOWN;
    short game_end = 0;

    int score = 0;
    int index_snake_error = -1;

    while (!game_end)
    {
        clock_t currentTime = clock(); // Текущее время
        usleep(100);                   // задержка для снижения процессорной нагрузки от вызова основного цикла программы
        // запускаем цикл кадра. Для начала к нему будет привязана только функция отслеживания нажатий
        if ((double)(currentTime) / CLOCKS_PER_SEC > target_time_chkKey)
        {
            target_time_chkKey += TARGET_FRAME_TIME; // Сдвинуть целевое время выполнения следующей проверки

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
                            target_time_chkKey = clock() / CLOCKS_PER_SEC;                    // Целевое время выполнения следующего кадра
                            target_time_move = clock() / CLOCKS_PER_SEC + DEFAULT_SPEED_TIME; // Целевое время выполнения следующего перемещения
                            break;
                        }
                    }
                }

                if (key_detect >= '1' && key_detect <= '9')
                {
                    int number = key_detect - '0';
                    if (current_control != -1)
                    {
                        snakes[current_control]->control_ai = true;
                    }
                    snakes[number - 1]->control_ai = false;
                    current_control = number - 1;
                }
                else if (key_detect == '0')
                {
                    if (current_control != -1)
                    {
                        snakes[current_control]->control_ai = true;
                    }
                    current_control = -1;
                }

                // преобразуем последнюю зафиксированую нажатой клавишу в направление
                if (prev_key != key_detect && current_control != -1)
                {
                    prev_key = key_detect;
                    conversionKeyToDirect(key_detect, snakes[current_control]);
                }
            }
        }

        // цикл отвечающий за перемещение змейки
        if ((double)(currentTime) / CLOCKS_PER_SEC > target_time_move)
        {
            target_time_move += DEFAULT_SPEED_TIME; // Сдвинуть целевое время выполнения следующего кадра
            for (int i = 0; i < snake_count; i++)
            {
                // Проверяем что пришло вемя двигать змейку
                if (!((double)(currentTime) / CLOCKS_PER_SEC > target_time_snakesMove[i]))
                    continue;
                target_time_snakesMove[i] += 1.0 / snakes[i]->speed;

                {
                    // Перемещаем змею запоминая координаты кончика хвоста
                    tail_t *last_tail_p = &(snakes[i]->tail[snakes[i]->tsize - 1]);
                    tail_t last_tail = *last_tail_p;
                    moveSnake(snakes[i]);
                    // Проверяем что змея не столкнулась ни с чем
                    int state = checkCoordState(snakes, snake_count, snakes[i]->x, snakes[i]->y);
                    if (state)
                    {
                        index_snake_error = i;
                        game_end = state;
                        break;
                    }
                    // Запускаем AI для неуправляемых змей
                    if (snakes[i]->control_ai)
                    {
                        // aiSnake(snakes[i], &food);
                        aiSnake2(snakes, snake_count, snakes[i], &food);
                    }

                    // Проверяем не съела ли змея еду
                    if (food.x == snakes[i]->x && food.y == snakes[i]->y)
                    {
                        // увеличиваем змейку возвращая кончик
                        eatSnake(snakes[i], &last_tail);
                        // генерируем новую еду
                        generationFood(&food, snakes, snake_count);
                        // изменяем скорость
                        // speedSnake += SPEED_INCREASE;
                        // speedFrameTime = 1.0 / speedSnake;
                        snakes[i]->score += 1;
                        snakes[i]->speed = opts.snake_speed_default * (1.0 - (SPEED_DECREASE * (snakes[i]->tsize + 1)));
                    }
                }
                drawFood(food);

                // устанавливаем указатель в сторону, чтобы не мешался
                coord.X = 1;
                coord.Y = MAX_Y + 2;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            }
        }
    }
    printf("\n");
    system("cls");

    if (index_snake_error >= 0)
    {
        printf("Error move snake [%d] \nerror code: %d\nCoords: X-%d;Y-%d\n \n", 
        index_snake_error+1, game_end, snakes[index_snake_error]->x, snakes[index_snake_error]->y);
    }
    

    for (size_t i = 0; i < snake_count; i++)
    {   
        // Устанавливаем цветовой код
        printf("%s", snakes[i]->color);
        printf("Score snake [%d]: %d\n", i+1, snakes[i]->score);
        printf("Final lenght: %d\n\n", snakes[i]->tsize);
        score += snakes[i]->score;
    }
    printf("%s", ANSI_COLOR_RESET);
    
    printf("Score ALL: %d\n", score);
    system("\npause");
    return 0;
}