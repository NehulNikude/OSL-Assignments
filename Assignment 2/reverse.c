#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int n = argc;
    printf("Inside %s Program \n", argv[0]);
    printf("Printing The Numbers In Reverse Order \n");
    for (int i = n - 1; i > 0; i--)
    {
        int number = atoi(argv[i]);
        printf("%d ", number);
    }
    printf("\n");
    printf("Ending %s Program \n", argv[0]);
}