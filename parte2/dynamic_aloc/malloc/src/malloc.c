#include <stdio.h>
#include <unistd.h> // para sleep
#include <malloc.h>

#define SIZE 80000000 // 80 milhoes
int main()
{
    double* array = malloc(sizeof(double) * SIZE);
    printf("tam. mem: %zu\n", sizeof(double)*SIZE);
    for(int i=0; i<SIZE; i++)
        array[i] = i;
    sleep(10);
    free(array);
}
