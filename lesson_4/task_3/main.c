/*
Задача 3
На стандартном потоке ввода задаются две символьные строки, разделённые символом перевода
строки. Каждая из строк не превышает по длине 10000
. В строках не встречаются пробельные символы.
На стандартный поток вывода напечатайте два числа, разделённых пробелом: первое число — длина
наибольшего префикса первой строки, являющегося суффиксом второй; второе число — наоборот, длина
наибольшего суффикса первой строки, являющегося префиксом второй. Сравнение символов проводите
с учётом регистра (т.е. символы 'a' и 'A' различны).
Пример №1
Данные на вход:
don't_panic
nick_is_a_mastodon
Данные на выход: 3 3
Пример №2
Данные на вход:
monty_python
python_has_list_comprehensions
Данные на выход: 0 6
*/

#include <stdio.h>
#include <string.h>

#define MAX_SIZE 10000

int search_prefix(char *s1, char *s2)
{
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    int min_l;
    if (l2 < l1)
    {
        min_l = l2;
    }
    else
    {
        min_l = l1;
    }
    int result[l2];
    memset(result, 0, l2 * sizeof(int));

    int p2 = l2 - min_l;
    int p1 = 0;

    while (p2 < l2)
    {
        if (s1[p1] != s2[p2])
        {
            if (p1 == 0)
            {
                result[p2] = 0;
            }
            else
            {
                p1 = result[p1 - 1];
            }

            p2++;
        }
        else
        {
            result[p2] = p1 + 1;
            p2++;
            p1++;
        }
    }

    return result[l2 - 1];
}

int main()
{
    char string1[MAX_SIZE];
    char string2[MAX_SIZE];

    printf("Enter string 1: ");
    scanf("%s", string1);
    printf("\n");
    printf("Enter string 2: ");
    scanf("%s", string2);
    printf("\n");
    int result;

    printf("s1: %s\n", string1);
    printf("s2: %s\n", string2);

    result = search_prefix(string1, string2);
    printf("s1:s2 = %d\n", result);

    result = search_prefix(string2, string1);
    printf("s2:s1 = %d\n", result);

    return 0;
}