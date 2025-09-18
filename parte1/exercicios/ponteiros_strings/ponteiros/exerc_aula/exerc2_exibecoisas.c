#include <stdio.h>

void exibeCoisas(unsigned char *ptr, int tam);

int main()
{
    int v[] = {0, 1, 2, 3, 4};
    int matriz[4][4] = {
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {8, 9, 10, 11},
        {12, 13, 14, 15}};
    printf("Vetor v:\n");
    // Lembrando: o operador sizeof(x) retorna o tamanho
    // EM BYTES de x (x pode ser um tipo de dado ou uma variável)
    exibeCoisas((unsigned char *)v, 5 * sizeof(int));
    printf("\nMatriz:\n");
    // A matriz é 4x4, ou seja, tem 16 elementos (cada um é um int)
    exibeCoisas((unsigned char *)matriz,
                16 * sizeof(int));
}

void exibeCoisas(unsigned char *ptr, int tam)
{
    for (int i = 0; i < tam; i++)
    {
        // Pula linha e mostra o endereço de memória
        // a cada 16 bytes
        if (i % 16 == 0)
        {
            printf("\n%p: ", ptr + i);
        }
        // Exibe em hexadecimal no formato 00..FF
        printf("%02X ", ptr[i]);
    }
    printf("\n");
}