/*
Задание 1. Биномиальный коэффициент

Разыгрывается тираж лотереи. Участвует N номеров. Из них выпадает M номеров.
Сколькими вариантами набора номеров это может произойти?
!!! Реализовать программу с помощью биномиального коэффициента.

Данные на вход: Два целых числа N и M
Данные на выход: Одно целое число – количество вариантов

Пример №1
Данные на вход: 10 5
Данные на выход: 252

Пример №2
Данные на вход: 35 5
Данные на выход: 324632

Пример №3
Данные на вход: 40 10
Данные на выход: 847660528
*/

#include <stdio.h>

// C(n, k) = factorial(n) / (factorial(k) * factorial(n - k));
// Вычисление через факториал не подходит, так как факториал слишком большой и легко вызывает переполнение даже int64
// Найдена следующая рекурентная формула для вычисления биноминального коофициента
// C(n, k) = C(n-1, k-1) + C(n-1, k)

long long binomial_coefficient(int n, int k)
{
    if (k == 0 || k == n)
        return 1;
    return binomial_coefficient(n - 1, k - 1) + binomial_coefficient(n - 1, k);
}

int main()
{
    int N, M;
    printf("Enter the total number of numbers: ");
    scanf("%d", &N);
    printf("Enter the number of drawn numbers: ");
    scanf("%d", &M);

    if (M > N)
    {
        printf("The number of numbers can not exceed the total number of numbers.\n");
        return 1;
    }

    long long combinations = binomial_coefficient(N, M);
    printf("Number of dialling options: %lld\n", combinations);

    return 0;
}

// Решение оказалось очень долго вычисляемым по сравнению с предложенным на семинаре.
// Однако в решении семинара использовалось деление и тип double, что как мне кажется может повлияь на точность.
// Для вычислений биноминального коэффициента со сниженой точностью но без факториала есть также формулы с потерей точности