#include <stdio.h>

void swap(int *var1, int *var2)
{
    int temp = *var1;
    *var1 = *var2;
    *var2 = temp;
}

int main()
{
    int x = 10;
    int y = 5;

    printf("Antes X: %d, Y: %d\n", x, y);
    swap(&x, &y);
    printf("Depois X: %d, Y: %d\n", x, y);
    return 0;
}
