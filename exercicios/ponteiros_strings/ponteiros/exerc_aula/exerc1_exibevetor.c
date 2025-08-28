#include <stdio.h>

int main()
{
    unsigned int nums[] = {4294967295, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    unsigned char *ptr = (unsigned char *)nums;
    int bytes;
    for (int i = 0, bytes = 0; i < 40; i++, bytes += 1)
        printf("%d: %p + %d bytes = %p ==> %d\n",
               i, ptr, bytes, (ptr + i), *(ptr + i));
    printf("nums[0]: %u\n", nums[0]);
    return 0;
}