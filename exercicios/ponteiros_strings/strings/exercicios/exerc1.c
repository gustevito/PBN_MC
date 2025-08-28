#include <stdio.h>
#include <string.h>

char* minhastr(char* string, char* substring);

int main(int argc, char const *argv[])
{
    char *str = "teste do carai";
    char *sub = "do";
    
    printf("\nString completa: [%p] | %s", str, str);
    printf("\nSubstring: [%p] | %s\n", sub, sub);

    char *posicaoptr = minhastr(str, sub);
    if (posicaoptr != NULL)
    {
        printf("\nSubstring encontrada na posicao: [%p]\n", posicaoptr);
    }
    else
    {
        printf("\nSubstring nao encontrada.\n");
    }
    return 0;
}


char* minhastr(char* string, char* substring)
{
    int tamanhoString = strlen(string);
    int tamanhoSub = strlen(substring);
    // int n = (tamanhoString - tamanhoSub);
    int i, j;

    printf("\nTamanho da String: [%d]", tamanhoString);
    printf("\nTamanho da Substring: [%d]\n", tamanhoSub);
    
    for (i = 0; i < tamanhoString - tamanhoSub; i++)
    {
        for (j = 0; i < tamanhoSub; i++)
        {
            if (string[i + j] != substring[j]) break;
        }

        if (j == tamanhoSub) return string + i;
    }
    return NULL;
}