#include <stdio.h>

void incArray(int v[]);

int main(int argc, char const *argv[])
{
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    incArray(data);
    printf("Array after incrementing each element:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
    return 0;
}

void incArray(int v[]){
    int c;
    for (c=0; c<10 ;c++)
    {
        v[c]++;
    }
    
}
