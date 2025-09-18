#include <stdio.h>

char* minhastr(char* string, char* substring);

int main(int argc, char const *argv[])
{
    char *str = "teste do carai";
    char *sub = "lala";
    
    printf("\nString completa: [%p] | %s", str, str);
    printf("\nSubstring: [%p] | %s", sub, sub);

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


char* minhastr(char* string, char* substring){

}