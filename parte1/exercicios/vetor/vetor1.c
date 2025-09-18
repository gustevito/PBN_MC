#include <stdio.h>

int buscaValor(int v[], int tam, int valor);

int main()
{
    int nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int valor;
    int pos;
    printf("Digite o valor a procurar: ");
    scanf("%d", &valor);

    pos = buscaValor(nums, 10, valor);

    if (pos != -1) printf("Valor encontrado na posição %d\n", pos);
    else printf("Posição não encontrada.");

}

int buscaValor(int v[], int tam, int valor)
{
    int *ptr = v;
        for (int i = 0; i < tam; ptr++, i++)
        {
            if (*ptr == valor) return i;
        }
    return -1;

    // jeito tradicional:
    /* for (int i = 0; i < tam; i++)
    {
        if (v[i] == valor)
        {
            return i;
        }
        
    }*/ 
    
}
