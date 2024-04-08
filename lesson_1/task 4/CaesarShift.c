#include <stdio.h>

int main()
{
    int N;
    char c;

    // Ввод числа N
    printf("enter the shift key: ");
    scanf("%d", &N);

    // Шифрование сообщения
    printf("enter the message: ");
    while (1)
    {
        // Считывание символа
        scanf("%c", &c);

        // Проверка на символ точки (конец сообщения)
        if (c == '.')
            break;

        // Проверка на букву
        if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z'))
        {
            printf("%c", c);
            continue;
        }
        else
        {
            // Кодирование
            char encrypted = c + N;
            // Вывод зашифрованной буквы
            printf("%c", encrypted);
        }
    }

    // Вывод точки
    printf(".");

    return 0;
}