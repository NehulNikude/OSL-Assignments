#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;
    int j = low;

    for (; j < high; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);

    return (i + 1);
}

void quicksort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

int main()
{
    int n;
    printf("\n");
    printf("Enter Number Of Array Elements: ");
    scanf("%d", &n);
    int *arr = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        printf("Enter Element %d: ", (i+1));
        scanf("%d", &arr[i]);
    }
    char **args = (char**)malloc((n+2) * sizeof(char*));
    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        printf("Fork Failed! \n");
        break;
    
    case 0:
        printf("\n");
        printf("Inside Child Process \n");
        printf("My PID = %d \n", getpid());
        printf("My Parent's PID = %d \n", getppid());
        printf("Original Array: ");
        for (int i = 0; i < n; i++) printf("%d ", arr[i]);
        printf("\n");
        printf("Now, Sorting The Array In Child Process \n");
        quicksort(arr, 0, (n - 1));
        printf("Sorted Array: ");
        for (int i = 0; i < n; i++) printf("%d ", arr[i]);
        printf("\n");
        for (int i = 0; i < n; i++)
        {
            char str[20];
            sprintf(str, "%d", arr[i]);
            args[i+1] = strdup(str);
        }
        args[0] = "./reverse";
        args[n+1] = NULL;
        printf("Calling Reverse Program Using Exec \n");
        execvp("./reverse", args);
        free(arr);
        exit(1);
        break;

    default:
        sleep(2);
        printf("\n");
        printf("Inside Parent Process \n");
        printf("My PID = %d \n", getpid());
        printf("My Child's PID = %d \n", pid);
        printf("Waiting For Child Process To Terminate..... \n");
        wait(NULL);
        printf("Child Process Has Been Terminated! \n");
        printf("Ending Parent Process Now \n\n");
        free(arr);
        exit(0);
        break;
    }
}