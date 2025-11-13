#include <stdio.h>

int main(int argc, char const *argv[])
{
    unsigned int valor = 0b10001011; // 139
    unsigned int maskr = 0b00101010; // 42
                        // 10100001    
    printf("%d\n", valor);
    printf("%d\n", (valor & maskr));
    printf("%d\n", (valor^maskr));
    return 0;
}



