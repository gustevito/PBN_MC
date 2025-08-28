#include <stdio.h>

void inverteVetor(int v[], int tam);
void inverteVetor2(int v[], int tam);
void exibeVetor(int v[], int tam);

int main()
{
    int vet1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    printf("Antes : ");
    exibeVetor(vet1, 10);
    inverteVetor(vet1, 10);
    // inverteVetor2(vet1, 10);
    printf("Depois: ");
    exibeVetor(vet1, 10);
}

// Esta implementação é MAIS EFICIENTE que
// a implementação com índices, pois só usa
// operações de adição e subtração dentro do
// loop
void inverteVetor(int v[], int tam)
{
    int *ptr1 = v;
    int *ptr2 = v + tam - 1;
    while (ptr1 < ptr2)
    {
        int tmp = *ptr1;
        *ptr1 = *ptr2;
        *ptr2 = tmp;
        ptr1++;
        ptr2--;
    }
}

// Esta implementação é MENOS eficiente que a anterior,
// pois cada acesso v[i] na verdade se transforma
// em um *(v+i), o que na prática é *(v+(i*4))
void inverteVetor2(int v[], int tam)
{
    int i = 0;
    int j = tam - 1;
    while (i < j)
    {
        int tmp = v[i];
        v[i] = v[j];
        v[j] = tmp;
        i++;
        j--;
    }
}

void exibeVetor(int v[], int tam)
{
    for (int i = 0; i < tam; i++)
        printf("%d ", v[i]);
    printf("\n");
}