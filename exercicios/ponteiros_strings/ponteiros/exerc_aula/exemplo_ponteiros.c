#include <stdio.h>

void incArray(int v[]);
void exibeArray(int v[], int tam);

int main()
{
    int b = 200;
    printf("%p\n", &b);
    int *a = &b;

    printf("Valor de b: %d\n", b);
    printf("Valor de a: %p\n", a);
    printf("Conteúdo apontado por a: %d\n", *a);

    // Vetores são SEMPRE passados por referência!
    long data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // exibeArray(data, 10);
    // incArray(data);
    // exibeArray(data, 10);

    // Aritmética de ponteiros
    char c[10] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
    char *p = c;                // não precisa &, c já é um ponteiro
    printf("*p  : %c\n", *p);   // exibe o primeiro elemento do array
    printf("p[0]: %c\n", p[0]); // idem

    printf("p    : %p\n", p);     // exibe o endereço do primeiro elemento
    printf("c    : %p\n", c);     // idem
    printf("&c[0]: %p\n", &c[0]); // idem

    printf("\n");
    printf("*p    : %c\n", *p);       // exibe 'a'
    printf("*(p+1): %c\n", *(p + 1)); // exibe 'b'
    printf("*(p+2): %c\n", *(p + 2)); // exibe 'c'
    printf("*(p+3): %c\n", *(p + 3)); // exibe 'd'

    // Aritmética de ponteiros é inteligente!
    printf("\n");
    printf("data[0]    : %d\n", data[0]);
    printf("*data      : %d\n", *data);
    printf("*(data+1)  : %d\n", *(data + 1));
    printf("*(data+2)  : %d\n", *(data + 2));
    printf("data       : %p\n", data);
    printf("data+1     : %p\n", data + 1);
    printf("data+2     : %p\n", data + 2);
}

void incArray(int v[])
{
    int c;
    for (c = 0; c < 10; c++)
        v[c]++;
}

void exibeArray(int v[], int tam)
{
    for (int i = 0; i < tam; i++)
        printf("%d ", v[i]);
    printf("\n");
}