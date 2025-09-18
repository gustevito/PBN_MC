#include <stdio.h>

void func1();
void func2();

int main()
{
    // func1();
    func2();
}

void func1()
{
    int vet[] = {4, 9, 12};
    int i, *ptr;
    ptr = vet;
    for (i = 0; i < 3; i++)
    {
        // Melhor prática: SEPARAR em duas linhas
        // uma com o acesso e outra com o incremento
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
}

void func2()
{
    int vet[] = {4, 9, 12};
    int i, *ptr;
    ptr = vet;
    for (i = 0; i < 3; i++)
        // Erro: usando incorretamente o operador
        // de incremento
        printf("%d ", (*ptr)++);
    printf("\n");
    printf("vet[0]: %d\n", vet[0]);
}