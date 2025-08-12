#include <stdio.h>

void troco(int r, int *n100, int *n50, int *n10, int *n5, int *n1)
{
    *n100 = r / 100;
    *n50 = r % 100 / 50;
    *n10 = r % 100 % 50 / 10;
    *n5 = r % 100 % 50 % 10 / 5;
    *n1 = r % 100 % 50 % 10 % 5 / 1;
}

int main()
{
    int valor;
    int notas100, notas50, notas10, notas5, notas1;
    printf("Digite um nº inteiro: ");
    scanf("%d", &valor);
    troco(valor, &notas100, &notas50, &notas10, &notas5, &notas1);

    printf("Notas de 100: %d\n", notas100);
    printf("Notas de 50: %d\n", notas50);
    printf("Notas de 10: %d\n", notas10);
    printf("Notas de 5: %d\n", notas5);
    printf("Notas de 1: %d\n", notas1);
    return 0;
}
