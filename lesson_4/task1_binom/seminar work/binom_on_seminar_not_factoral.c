#include <stdio.h>
int binom(int n, int k)
{
    double res = 1;
    for (int i = 1; i <= (n-k); ++i)
    {
        res *= (n - i + 1);
        res /= i;
    }
    return (int) res;
}
int main()
{
    int n, k;
    scanf("%d%d", &n, &k);
    printf("res = %d", binom(n, k));
    return 0;
}