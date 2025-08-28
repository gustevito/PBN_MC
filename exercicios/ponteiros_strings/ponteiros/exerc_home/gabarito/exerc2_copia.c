#include <stdio.h>

void copiaVetor(int v1[], int v2[], int tam);
void exibeVetor(int v[], int tam);

int main()
{
    int vet1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int vet2[10]; // vazio (lembrando: tem LIXO)
    printf("vet1: ");
    exibeVetor(vet1, 10);
    printf("vet2:\n");
    printf("ANTES da cópia : ");
    exibeVetor(vet2, 10);
    copiaVetor(vet1, vet2, 10);
    printf("DEPOIS da cópia: ");
    exibeVetor(vet2, 10);
}

void copiaVetor(int v1[], int v2[], int tam)
{
    int *ptr1 = v1;
    int *ptr2 = v2;
    for (int i = 0; i < tam; i++, ptr1++, ptr2++)
    {
        // v2[i] = v1[i];
        // *(v2+i) = *(v1+i)
        //*ptr2++ = *ptr1++;
        *ptr2 = *ptr1;
        // ptr1++;
        // ptr2++;
    }
}

void exibeVetor(int v[], int tam)
{
    for (int i = 0; i < tam; i++)
        printf("%d ", v[i]);
    printf("\n");
}