#include <stdio.h>
#include <malloc.h>

int main()
{
    int linhas = 5;
    int colunas = 8;
    int** mat1 = malloc(sizeof(int*) * linhas);
    int(*mat2)[colunas] = malloc (sizeof *mat2 * linhas);

    for(int i=0; i<linhas; i++)
        mat1[i] = malloc(sizeof(int) * colunas);

    int cont =0;
    for(int i=0; i<linhas; i++)
        for(int j=0; j<colunas; j++) {
            mat1[i][j] = cont;
            mat2[i][j] = cont++;
        }

    // Observe que ha "buracos" entre uma
    // linha e outra, ou seja, o inicio
    // da linha seguinte nao corresponde
    // ao tamanho exato da linha anterior
    printf("Mat1:\n");
    for(int i=0; i<linhas; i++) {
        printf("%d: %p\n", i, &mat1[i][0]);
    }

    // Note que agora a linha seguinte comeca
    // exatamente depois da linha anterior, ou
    // seja, e' um bloco unico de memoria
    printf("\nMat2:\n");
    for(int i=0; i<linhas; i++) {
        printf("%d: %p\n", i, &mat2[i][0]);
    }

    for(int i=0; i<linhas; i++)
        free(mat1[i]);
    free(mat1);

    free(mat2);
}