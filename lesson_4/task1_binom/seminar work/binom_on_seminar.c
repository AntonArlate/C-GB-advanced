#include <stdio.h>
int cnk(int n, int k)
{
    int res = 1;
    for (int i = n - k + 1; i <= n; ++i)
        res *= i;
    for (int i = 2; i <= k; ++i)
        res /= i;
    return res;
}
int main()
{
    int a, b;
    scanf("%d%d", &a, &b);
    printf("%d", cnk(a, b));
    return 0;
}