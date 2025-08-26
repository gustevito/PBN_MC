#include <stdio.h>

int main()
{
    int v[10] = {1, 2, 3, 4, 5, 3, 6, 3, 2, 8};

    int totalRepetidos = 0;
    int totalVezes;
    for (int i = 0; i < 10; i++)
    {
        totalVezes = 0;
        for (int j = i + 1; j < 10; j++)
        {
            if (v[i] == v[j])
            {
                totalVezes++;
            }
        }
        if (totalVezes == 1)
        {
            totalRepetidos++;
        }
    }
    printf("Total de repetidos: %d\n", totalRepetidos);
}