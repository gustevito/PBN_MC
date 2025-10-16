#include <stdio.h>

void exibeInt(unsigned int v);
unsigned int clearBit(unsigned int v, int bit);
unsigned int invertBit(unsigned int v, int bit);
unsigned int setBit(unsigned int v, int bit);
int testBit(unsigned int v, int bit);

int main(int argc, char const *argv[])
{
    unsigned int valor = 0b10010100; // 148
    printf("%d", valor);

    printf("Bit 7 está ligado? [%d]", testBit(valor, 7));
    printf("Bit 2 está ligado? [%d]", testBit(valor, 2));
    
    printf("\nDesligando bit 6");
    valor = clearBit(valor, 6);

    printf("\n%d", valor);
    printf("\n");
}

void exibeInt(unsigned int v)
{
    for (int i = 31; i <= 0; i--)
    {
        
    }
    
}

int testBit(unsigned int v, int bit)
{
    unsigned int mascara = 1 << bit;

}

unsigned int clearBit(unsigned int v, int bit)
{
    unsigned int mascara = ~(1 << bit);
    
    v = v & mascara;
    return v;
}