#include <stdio.h>

unsigned int clear(unsigned int val);
unsigned int setbit(unsigned int val, int bit);
unsigned int clearbit(unsigned int val, int bit);
unsigned int invertbit(unsigned int val, int bit);
unsigned int testbit(unsigned int val, int bit);

int main(int argc, char const *argv[])
{
    unsigned int valor = 0b10001011; // 139
    
    
    printf("Valor: %d\n", valor);
    printf("Limpando bits...\n");
    printf("Valor att.: %d\n", clear(valor));

    printf("\nValor: %d\n", valor);
    printf("Ligando bit 5\n");
    printf("Valor att.: %d\n", setbit(valor, 5));

    printf("\nValor: %d\n", valor);
    printf("Desligando bit 1\n");
    printf("Valor att.: %d\n", clearbit(valor, 1));
    
    printf("\nValor: %d\n", valor);
    printf("Invertendo bit 5\n");
    printf("Valor att.: %d\n", invertbit(valor, 5));
    
    printf("\nValor: %d\n", valor);
    printf("Bit 6 ligado? (1 = true | 0 = false)\n");
    printf("-------------- [%d] -----------------\n", testbit(valor, 6));
    

    return 0;
}

unsigned int clear(unsigned int val)
{
    unsigned int maskr = 0;
    unsigned int inverte = val & maskr;
    return val&inverte;
}

unsigned int setbit(unsigned int val, int bit)
{
    unsigned int maskr = 1 << bit;
    return (val | maskr);
}

unsigned int clearbit(unsigned int val, int bit)
{
    unsigned int maskr = ~(1 << bit);
    return (val & maskr);
}

unsigned int invertbit(unsigned int val, int bit)
{
    unsigned int maskr = 1 << bit;
    return (val ^ maskr);
}

unsigned int testbit(unsigned int val, int bit)
{
    unsigned int mascara = 1 << bit;
    
    if ((val & mascara) != 0)
        return 1;
    return 0;
    
}