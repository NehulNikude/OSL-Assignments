#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    int choice;
    printf("\n");
    printf("**********MENU**********\n");
    printf("1. Normal Execution \n");
    printf("2. Demonstrate Zombie State \n");
    printf("3. Demonstrate Orphan State \n");
    printf("\n");
    printf("Enter Your Choice: ");
    scanf("%d", &choice);
    printf("\n");

    switch (choice)
    {
    case 1:
        int n;
        printf("Enter Number Of Array Elements: ");
        scanf("%d", &n);
        int *arr = (int*)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++)
        {
            printf("Enter Element %d: ", (i+1));
            scanf("%d", &arr[i]);
        }
        printf("\n");
        
        pid_t pid = fork();
        switch (pid)
        {
        case -1:
            printf("Fork Failed! \n");
            break;
        
        case 0:
            sleep(1);
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
            printf("Exiting Child Process Now! \n\n");
            exit(0);
            break;

        default:
            printf("Inside Parent Process \n");
            printf("My PID = %d \n", getpid());
            printf("My Child's PID = %d \n", pid);
            printf("Original Array: ");
            for (int i = 0; i < n; i++) printf("%d ", arr[i]);
            printf("\n");
            printf("Now, Sorting The Array In Parent Process \n");
            quicksort(arr, 0, (n - 1));
            printf("Sorted Array: ");
            for (int i = 0; i < n; i++) printf("%d ", arr[i]);
            printf("\n");
            printf("Waiting For Child Process To Terminate..... \n\n");
            wait(NULL);
            printf("Child Process Has Been Terminated! \n");
            printf("Ending Parent Process Now \n\n");
            exit(0);
            break;
        }
        break;
    
    case 2:
        printf("Demonstrating Zombie State \n");
        printf("\n");
        pid_t pid1 = fork();

        switch (pid1)
        {
        case -1:
            printf("Fork Failed! \n");
            break;
        
        case 0:
            printf("Inside Child Process \n");
            printf("My PID = %d \n", getpid());
            printf("My Parent's PID = %d \n", getppid());
            printf("Exiting Child Process Now \n\n");
            exit(0);
        default:
            sleep(5);
            printf("Inside Parent Process \n");
            printf("My PID = %d \n", getpid());
            printf("My Child's PID = %d \n\n", pid1);
            system("ps | grep a.out");
            printf("\n");
            printf("Child Process Has Completed Its Execution \n");
            printf("It Has Become A Zombie Process <Defunct> \n\n");
            break;
        }
        break;
    
    case 3:
        printf("Demonstrating Orphan State \n");
        printf("\n");
        pid_t pid2 = fork();

        switch (pid2)
        {
        case -1:
            printf("Fork Failed! \n");
            break;
        
        case 0:
            sleep(3);
            printf("\n");
            printf("Inside Child Process \n");
            printf("My PID = %d \n", getpid());
            printf("My (Adoptive) Parent's PID = %d \n\n", getppid());
            break;

        default:
            printf("Inside Parent Process \n");
            printf("My PID = %d \n", getpid());
            printf("My Child's PID = %d \n\n", pid2);
            printf("My Child Process Will Get Orphaned Now & Will Be Adopted By Another Process \n");
            printf("Ending Parent Process Now \n\n");
            break;
        }
        break;

    default:
        printf("Invalid Choice! \n\n");
        break;
    }   
}