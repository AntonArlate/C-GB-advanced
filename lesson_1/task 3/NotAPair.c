/*
Задание 3. Не пара

Преподаватели Geek Brains создали набор, содержащий N (1 ≤ N ≤ 1 000 000) натуральных чисел, не превосходящих 1 000 000. Известно, что ровно одно число в этом наборе встречается один раз, а остальные — ровно по два раза. Помогите им найти это число. Входные данные: в первой входной строке вводится число N, затем по одному числу в строке вводятся N натуральных чисел, не превосходящих 2 000 000 000. Выходные данные: ваша программа должна определить число, встречающееся один раз, и вывести его на экран.

Указание: использовать массивы запрещается.

Данные на входе: Натуральное число N (N > 0), после которого следует
последовательность из N целых чисел.
Данные на выходе: Одно целое число.

Пример №1
Данные на входе: 3 12 15 12
Данные на выходе: 15

Пример №2
Данные на входе: 5 1 2 0 2 1
Данные на выходе: 0
*/

#include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

#define MAX_SIZE 1000000

// Структура, хранящая результат
struct Memory {
    int value;
};

// Инициализация результата
void reInitMem(struct Memory* memory) {
    memory->value = 0;
}

// Добавление числа из последовательности
void addNumber(struct Memory* memory, int number) {
    memory->value ^= number;
}

/*
// Получение результата в виде строки
char* getResult(struct Memory* memory) {
    char* result = malloc(sizeof(char) * 20); // предполагаем, что число не будет превышать 20 символов
    sprintf(result, "%d", memory->value);
    return result;
}
*/

// Получение результата
int getResult(struct Memory* memory) {
    return memory->value;
}

// Пример использования
int main() {
    struct Memory memory;

    // example 1
    reInitMem(&memory);
    addNumber(&memory, 3);
    addNumber(&memory, 12);
    addNumber(&memory, 3);
    addNumber(&memory, 15);
    addNumber(&memory, 12);
    printf("Result 1: %d\n", getResult(&memory));


    // example 1
    reInitMem(&memory);
    addNumber(&memory, 5);
    addNumber(&memory, 1);
    addNumber(&memory, 2);
    addNumber(&memory, 0);
    addNumber(&memory, 5);
    addNumber(&memory, 2);
    addNumber(&memory, 1);
    printf("Result 2: %d\n", getResult(&memory));

    return 0;
}