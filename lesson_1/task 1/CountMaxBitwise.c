/*
Задание 1. Сколько раз встречается максимум

На стандартном потоке ввода задается натуральное число N (N > 0), после которого следует последовательность из N целых чисел.
На стандартный поток вывода напечатайте, сколько раз в этой последовательности встречается максимум.

Указание: использовать массивы запрещается.

Данные на входе: Натуральное число N (N > 0), после которого следует
последовательность из N целых чисел.
Данные на выходе: Одно целое число.

Пример №1
Данные на входе: 6 1 2 3 6 2 0 3
Данные на выходе: 2

Пример №2
Данные на входе: 3 5 2 -1
Данные на выходе: 1.
*/

#include <stdio.h>

int countMax(int N)
{
    static int max = -9999999;
    static int count = 0;
    static int iterCount = 0;

    if (iterCount == 0)
    {
        iterCount = N;
    }
    else
    {

        // printf("iter count = %d \n", iterCount);
        // printf("number = %d \n", N);
        if (iterCount > 1)
        {
            int diff = N - max;
            // int diff = N ^ max;

            if (diff == 0)
            {
                count++;
            }
            else if (diff > 0)
            {
                max = N;
                count = 1;
            }
            // N < max: ничего не делаем
        }
        else
        {
            printf("max number = %d; \tbeen received = %d\n", max, count);
            max = -9999999;
            count = 0;
            return count;
        }
        iterCount--;
    }

    return iterCount;
}

int main()
{
    int N;
    int num;

    printf("Enter N: ");
    scanf("%d", &N);
    countMax(N);

    printf("Enter the sequence: ");

    while (scanf("%d", &num) == 1)
    {
        if (N > 0)
        {
            countMax(num);
            N--;
        }
        if (N == 0)
        {
            char c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            printf("Complete sequence\n");
            break;
        }
    }

    // while (scanf("%d", &num) == 1) {
    //     if (!countMax(num))
    //     {
    //         char c;
    //         while ((c = getchar()) != '\n' && c != EOF) {}
    //         printf("Complete sequence\n");
    //         break;
    //     }
    // }

    return 0;
}