#include <stdio.h>

void ordenaVetor(int v[], int tam);
void exibeVetor(int v[], int tam);

int main()
{
    int vet1[] = {7, 1, 5, 2, 4, 9, 8, 3, 10, 6};
    printf("Antes : ");
    exibeVetor(vet1, 10);
    ordenaVetor(vet1, 10);
    printf("Depois: ");
    exibeVetor(vet1, 10);
}

void ordenaVetor(int v[], int tam)
{
    for (int i = 0; i < tam; i++)
    {
        int *ptr1 = v;
        int *ptr2 = v + 1;
        for (int j = 0; j < tam - 1 - i; j++)
        {
            if (*ptr1 > *ptr2) // v[j] > v[j+1]
            {
                int tmp = *ptr1; // tmp = v[j];
                *ptr1 = *ptr2;   // v[j] = v[j + 1];
                *ptr2 = tmp;     // v[j + 1] = tmp;
            }
            ptr1++;
            ptr2++;
        }
    }
}

void exibeVetor(int v[], int tam)
{
    for (int i = 0; i < tam; i++)
        printf("%d ", v[i]);
    printf("\n");
}