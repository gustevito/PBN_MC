#include <stdio.h>

void exibeInt(unsigned int v);
int testBit(unsigned int v, int bit);
unsigned int clearBit(unsigned int v, int bit);

int main(int argc, char const *argv[])
{
    unsigned int valor = 132;
    printf("%d", valor);
    
    printf("\nDesligando bit 6");
    valor = clearBit(valor, 6);

    printf("\n%d", valor);
    printf("\n");
}

void exibeInt(unsigned int v)
{

}

int testBit(unsigned int v, int bit)
{

}

unsigned int clearBit(unsigned int v, int bit)
{
    unsigned int mascara = ~(1 << bit);
    
    v = v & mascara;
    return v;
}