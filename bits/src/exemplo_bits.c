#include <stdio.h>

int main()
{
    unsigned int var;
    var = 254;
    var = 0xfe;
    var = 0b11111110;
    printf("var: %02X\n", var);
    printf("sizeof(int)    = %d\n", sizeof(int));
    printf("sizeof(char)   = %d\n", sizeof(char));
    printf("sizeof(float)  = %d\n", sizeof(float));
    printf("sizeof(double) = %d\n", sizeof(double));
    printf("\n");
    unsigned int x = 25;
    printf("x: %d\n", x);
    x = x << 1; // desloca UM bit para a esquerda
    printf("x: %d\n", x);
    x = x << 1; // desloca UM bit para a esquerda
    printf("x: %d\n", x);
}