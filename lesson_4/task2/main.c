/*
Задание 2. Обратная польская запись

Написать алгоритм перевода из инфиксной записи в обратную польскую.
Для его реализации нужны данные по приоритетам операций.
Реализовать алгоритм, используя побитовые операции (&, |, ^).
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_SIZE 100

#define PLUS_MASK 0x2  // +
#define MINUS_MASK 0x2 // -
#define MUL_MASK 0x4   // *
#define DIV_MASK 0x4   // /
#define AND_MASK 0x32  // &
#define XOR_MASK 0x16  // ^
#define OR_MASK 0x8    // |
#define BRACE_MASK 0x1 // (

// Устанавливаем приоритет операций используя битовую маску
int getPriority(char op)
{
    switch (op)
    {
    case '+':
        return PLUS_MASK;
    case '-':
        return MINUS_MASK;
    case '*':
        return MUL_MASK;
    case '/':
        return DIV_MASK;
    case '^':
        return XOR_MASK;
    case '&':
        return AND_MASK;
    case '|':
        return OR_MASK;
    case '(':
        return BRACE_MASK;
    default:
        return 0;
    }
}

// тут лучше создать структуру со стеком, но пока чтобы не запутаться в учебных целях были использованы глобальные переменные
char stack[MAX_SIZE];
int top = -1;
int last_priority = 0;

void push(char c)
{
    if (top == MAX_SIZE - 1)
    {
        printf("Стек переполнен\n");
        return;
    }
    stack[++top] = c;
    last_priority = getPriority(c);
}

char pop()
{
    if (top == -1)
    {
        printf("Стек пуст\n");
        return '\0';
    }
    if (top == 0)
    {
        last_priority = 0;
    }
    else
    {
        last_priority = getPriority(stack[top - 1]);
    }

    return stack[top--];
}

char *infixToPostfix(char *infix)
{
    static char postfix[MAX_SIZE];
    int i, j = 0;

    for (i = 0; infix[i] != '\0'; i++)
    {
        // проверяем является ли символ буквой или числом
        if (isalnum(infix[i]))
        {
            // если да, записываем в строку
            postfix[j++] = infix[i];

            // проверяем на открывающую скобку
        }
        else if (infix[i] == '(')
        {
            // Записываем её в стек хоть она и имеет очень низкий приоритет
            push(infix[i]);
            // проверяем закрывающую скобку
        }
        else if (infix[i] == ')')
        {
            // переписываем из стека пока не встретим открывающую
            char c = 0;
            do
            {
                c = pop();
                if (c != '(')
                {
                    postfix[j++] = c;
                    continue; // так как цикл проверяет тоже самое условие можно сразу перейти на итерацию
                }
            } while (c != '(');
            // На выходе у нас должны быть переписаны все знаки внутри текущих скобок, сама скобка должна быть удалена
            // Сравниваем приоритет текущего действия с последним в стеке
        }
        else if (last_priority < getPriority(infix[i]))
        {
            // отправляем наш символ в стек и переходим дальше
            push(infix[i]);
        }
        else
        {
            // Переписываем последний символ из стека и повторяем проверку
            postfix[j++] = pop();
            i--;
            continue;
        }
    }

    // опусташаем стек
    while (top != -1)
    {
        postfix[j++] = pop();
    }
    postfix[j] = '\0';

    return postfix;
}

int main()
{
    char infix[] = "8^2&5|(6&1)";
    char *postfix = infixToPostfix(infix);
    printf("Инфиксное выражение: %s\n", infix);
    printf("Обратная польская запись: %s\n", postfix);
    return 0;
}