#include <stdio.h>

typedef struct
{
    int r;
    int g;
    int b;
} RGB;

// 60 bits
typedef struct
{
    int r : 20;
    int g : 20;
    int b : 20;
} RGBBit;

int main()
{
    printf("Tam. de RGB: %d\n", sizeof(RGB));
    // Por que agora ocupa 12, se só precisaria ocupar 8?

    // Resposta: NÃO sabemos como o compilador aloca a memória para
    // essa struct - possivelmente ele está reservando algum espaço no final
    // por questões de alinhamento em múltiplos de 16 ou algo
    // similar
    printf("Tam. de RGBBit: %d\n", sizeof(RGBBit));
}