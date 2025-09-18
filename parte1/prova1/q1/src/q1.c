#include <stdio.h>
#define MAX 10

void subVets(int *vetOrig, int *vetInis, int *vetTams, int *total)
{
    // Varre o vetor original e armazena o início e o tamanho de cada subvetor
    int pos = 1;
    int tamVetOrig = vetOrig[0];
    int qtdSubVets = 0;

    while (pos < tamVetOrig)
    {
        int qtdElemSub = vetOrig[pos];
        int posIniSub = pos + 1;
        printf("pos: %d, %d elems.\n", posIniSub, qtdElemSub);
        vetInis[qtdSubVets] = posIniSub;
        vetTams[qtdSubVets] = qtdElemSub;
        qtdSubVets++;
        pos += qtdElemSub + 1;
    }
    *total = qtdSubVets;
}

int main()
{
    int vetorOriginal[] = {11, 2, 0, 1, 6, 1, 0, 0, 1, 1, 1};
    int vetInicios[MAX], vetTamanhos[MAX];
    int totalSubVets;
    // Chama a função subVets para preencher os vetores vetInicios e vetTamanhos
    subVets(vetorOriginal, vetInicios, vetTamanhos, &totalSubVets);

    // Exibe cada um dos subvetores, utilizando os dados em vetInicios e vetTamanhos
    printf("Total de subvetores: %d\n", totalSubVets);
    for (int i = 0; i < totalSubVets; i++)
    {
        printf("%d: ", i);
        // pos é a posição DENTRO do subvetor
        for (int pos = 0; pos < vetTamanhos[i]; pos++)
        {
            // Pega a pos. de início do subvetor
            int posInicio = vetInicios[i];
            // Acessa o elemento em posInicio + o contador de posições
            printf("%d ", vetorOriginal[posInicio + pos]);
        }
        printf("\n");
    }
}