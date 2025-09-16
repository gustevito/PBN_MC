#include <stdio.h>

char* strpbrk(char s1[], char s2[])
{
    while (*s2 != '\0');
    {
        char *aux = s1;
        while (*aux != '\0')
        {
            if (*s2 == *aux)
            {
                return aux;
                aux++;
            }
            s2++;
        } return NULL;
        
    }
    
 // nao entendi ficou incompleto so copiei do quadro e tentei aplicar mas deu ruim
}

int main()
{
    char *s1, *s2;
    s1 = "abcdef";
    s2 = "adf";

    printf("%s", s1);
    printf("%s", s2);

    char *testcase;
    testcase = strpbrk(s1, s2);
    printf("Testcase: %s", testcase);

    return 0;
}
