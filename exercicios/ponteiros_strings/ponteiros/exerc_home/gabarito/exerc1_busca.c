#include <stdio.h>

int buscaValor(int v[], int tam, int valor);

int main()
{
    int nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int valor;
    printf("Digite o valor a procurar: ");
    scanf("%d", &valor);
    int pos = buscaValor(nums, 10, valor);
    if (pos == -1)
    {
        printf("Não encontrado!\n");
    }
    else
    {
        printf("Achei na posição %d\n", pos);
    }
}

int buscaValor(int v[], int tam, int valor)
{
    int *ptr = v;
    for (int i = 0; i < tam; i++, ptr++)
    {
        if (*ptr == valor)
            return i;
        // ptr++;
    }
    return -1;
}