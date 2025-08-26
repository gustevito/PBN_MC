#include <stdio.h>

int main()
{
    int valor;
    printf("Digite um valor inteiro: ");
    scanf("%d", &valor);
    int n100 = valor / 100;
    int n50 = valor % 100 / 50;
    int n10 = valor % 100 % 50 / 10;
    int n5 = valor % 100 % 50 % 10 / 5;
    int n1 = valor % 100 % 50 % 10 % 5; // / 1;
    if (n100 > 0)
    {
        printf("Notas de 100: %d\n", n100);
    }
    if (n50 > 0)
    {
        printf("Notas de 50 : %d\n", n50);
    }
    if (n10 > 0)
    {
        printf("Notas de 10 : %d\n", n10);
    }
    if (n5 > 0)
    {
        printf("Notas de 5  : %d\n", n5);
    }
    if (n1 > 0)
    {
        printf("Notas de 1  : %d\n", n1);
    }
    return 0;
}