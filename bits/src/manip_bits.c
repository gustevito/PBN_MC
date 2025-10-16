#include <stdio.h>

void exibeInt(unsigned int v);
unsigned int clearBit(unsigned int v, int bit);
unsigned int invertBit(unsigned int v, int bit);
unsigned int setBit(unsigned int v, int bit);
int testBit(unsigned int v, int bit);

int main()
{
    unsigned int valor;
    valor = 0b10010100;
    printf("Bit 7 está ligado: ");
    printf("%d\n", testBit(valor, 7));
    printf("Bit 5 está ligado: ");
    printf("%d\n", testBit(valor, 5));

    printf("Valor: %d\n", valor);
    exibeInt(valor);

    printf("Ligando o bit 3\n");
    valor = setBit(valor, 3);
    printf("Valor: %d\n", valor);
    exibeInt(valor);

    printf("Desligando o bit 7\n");
    valor = clearBit(valor, 7);
    printf("Valor: %d\n", valor);
    exibeInt(valor);

    printf("Inverte o bit 3\n");
    valor = invertBit(valor, 3);
    printf("Valor: %d\n", valor);
    exibeInt(valor);

    printf("Inverte o bit 0\n");
    valor = invertBit(valor, 0);
    printf("Valor: %d\n", valor);
    exibeInt(valor);
}

void exibeInt(unsigned int v)
{
    for (int bit = 31; bit >= 0; bit--)
    {
        if (testBit(v, bit))
            printf("1");
        else
            printf("0");
    }
    printf("\n");
}

unsigned int clearBit(unsigned int v, int bit)
{
    // Mesma forma de montar a máscara,
    // MAS invertendo os bits no final
    unsigned int mascara = ~(1 << bit);
    exibeInt(mascara);
    // DESLIGA o bit
    v = v & mascara;
    return v;
}

unsigned int invertBit(unsigned int v, int bit)
{
    unsigned int mascara = 1 << bit;
    // INVERTE o bit
    v = v ^ mascara;
    return v;
}
int testBit(unsigned int v, int bit)
{
    unsigned int mascara = 1 << bit;
    // Bit está ligado?
    if ((mascara & v) != 0)
        return 1;
    return 0;
}

unsigned int setBit(unsigned int v, int bit)
{
    unsigned int mascara = 1 << bit;
    // LIGA o bit
    v = v | mascara;
    return v;
}
