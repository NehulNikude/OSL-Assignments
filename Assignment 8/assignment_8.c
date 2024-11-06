#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_REQUESTS 100

void sort(int requests[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (requests[j + 1] < requests[j])
            {
                int temp = requests[j];
                requests[j] = requests[j + 1];
                requests[j + 1] = temp;
            }
        }
    }
}

int FCFS(int requests[], int n, int initial)
{
    int current = initial;
    int total_movement = 0;

    printf("\nFCFS Disk Scheduling\n");
    printf("Sequence: %d", initial);
    
    for (int i = 0; i < n; i++)
    {
        total_movement += abs(requests[i] - current);
        current = requests[i];
        printf(" -> %d", current);
    }
    printf("\nTotal Movements = %d\n", total_movement);
    return total_movement;
}

int SSTF(int requests[], int n, int initial)
{
    int current = initial;
    int total_movement = 0;
    int visited[MAX_REQUESTS] = {0};

    printf("\nSSTF Disk Scheduling\n");
    printf("Sequence: %d", initial);

    for (int i = 0; i < n; i++)
    {
        int min_distance = INT_MAX;
        int min_index = -1;

        for (int j = 0; j < n; j++)
        {
            if (!visited[j])
            {
                if (abs(current - requests[j]) < min_distance)
                {
                    min_distance = abs(current - requests[j]);
                    min_index = j;
                }
            }
        }
        
        visited[min_index] = 1;
        total_movement += min_distance;
        current = requests[min_index];
        printf(" -> %d", current);
    }
    
    printf("\nTotal Movements = %d\n", total_movement);
    return total_movement;
}

int SCAN(int requests[], int n, int initial, int tracks)
{
    int current = initial;
    int total_movement = 0;
    int temp[MAX_REQUESTS];
    int k = 0;

    for (int i = 0; i < n; i++)
        temp[k++] = requests[i];
    temp[k++] = tracks - 1;

    sort(temp, k);

    printf("\nSCAN Disk Scheduling\n");
    printf("Sequence: %d", initial);

    int pos;
    for (pos = 0; pos < k; pos++)
        if (temp[pos] > initial)
            break;
    
    for (int i = pos; i < k; i++)
    {
        total_movement += abs(temp[i] - current);
        current = temp[i];
        printf(" -> %d", current);
    }

    for (int i = pos - 1; i >= 0; i--)
    {
        total_movement += abs(temp[i] - current);
        current = temp[i];
        printf(" -> %d", current);
    }
    
    printf("\nTotal Movements = %d\n", total_movement);
    return total_movement;
}

int CSCAN(int requests[], int n, int initial, int tracks)
{
    int current = initial;
    int total_movement = 0;
    int temp[MAX_REQUESTS];
    int k = 0;

    for (int i = 0; i < n; i++)
        temp[k++] = requests[i];
    temp[k++] = tracks - 1;

    sort(temp, k);

    printf("\nCSCAN Disk Scheduling\n");
    printf("Sequence: %d", initial);

    int pos;
    for (pos = 0; pos < k; pos++)
        if (temp[pos] > initial)
            break;
    
    for (int i = pos; i < k; i++)
    {
        total_movement += abs(temp[i] - current);
        current = temp[i];
        printf(" -> %d", current);
    }

    total_movement += tracks - 1;
    current = 0;
    printf(" -> %d", current);

    for (int i = 0; i < pos; i++)
    {
        total_movement += abs(temp[i] - current);
        current = temp[i];
        printf(" -> %d", current);
    }
    
    printf("\nTotal Movements = %d\n", total_movement);
    return total_movement;
}

int LOOK(int requests[], int n, int initial)
{
    int current = initial;
    int total_movement = 0;
    int temp[MAX_REQUESTS];
    int k = 0;

    for (int i = 0; i < n; i++)
        temp[k++] = requests[i];

    sort(temp, k);

    printf("\nLOOK Disk Scheduling\n");
    printf("Sequence: %d", initial);

    int pos;
    for (pos = 0; pos < k; pos++)
        if (temp[pos] > initial)
            break;
    
    for (int i = pos; i < k; i++)
    {
        total_movement += abs(temp[i] - current);
        current = temp[i];
        printf(" -> %d", current);
    }

    for (int i = pos - 1; i >= 0; i--)
    {
        total_movement += abs(temp[i] - current);
        current = temp[i];
        printf(" -> %d", current);
    }
    
    printf("\nTotal Movements = %d\n", total_movement);
    return total_movement;
}

int CLOOK(int requests[], int n, int initial)
{
    int current = initial;
    int total_movement = 0;
    int temp[MAX_REQUESTS];
    int k = 0;

    for (int i = 0; i < n; i++)
        temp[k++] = requests[i];

    sort(temp, k);

    printf("\nC-LOOK Disk Scheduling\n");
    printf("Sequence: %d", initial);

    int pos;
    for (pos = 0; pos < k; pos++)
        if (temp[pos] > initial)
            break;
    
    for (int i = pos; i < k; i++)
    {
        total_movement += abs(temp[i] - current);
        current = temp[i];
        printf(" -> %d", current);
    }

    total_movement += abs(current - temp[0]);
    current = temp[0];
    printf(" -> %d", current);

    for (int i = 1; i < pos; i++)
    {
        total_movement += abs(temp[i] - current);
        current = temp[i];
        printf(" -> %d", current);
    }
    
    printf("\nTotal Movements = %d\n", total_movement);
    return total_movement;
}

int main()
{
    //int requests[] = {82, 170, 43, 140, 24, 16, 190};
    //FCFS(requests, 7, 50);
    //SSTF(requests, 7, 50);
    //SCAN(requests, 7, 50, 200);
    //CSCAN(requests, 7, 50, 200);
    //LOOK(requests, 7, 50);
    //CLOOK(requests, 7, 50);
    int requests[MAX_REQUESTS], n, initial, tracks, choice;

    printf("\nEnter The Number Of Requests: ");
    scanf("%d", &n);
    printf("Enter Request Sequence: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &requests[i]);
    printf("Enter Initial Head Position: ");
    scanf("%d", &initial);
    printf("Enter The Number Of Tracks: ");
    scanf("%d", &tracks);

    do
    {
        printf("\n--- Disk Scheduling Algorithms ---\n");
        printf("1. FCFS\n");
        printf("2. SSTF\n");
        printf("3. SCAN\n");
        printf("4. C-SCAN\n");
        printf("5. LOOK\n");
        printf("6. C-LOOK\n");
        printf("7. Exit\n");
        printf("\nEnter Your Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            FCFS(requests, n, initial);
            break;
        
        case 2:
            SSTF(requests, n, initial);
            break;

        case 3:
            SCAN(requests, n, initial, tracks);
            break;

        case 4:
            CSCAN(requests, n, initial, tracks);
            break;

        case 5:
            LOOK(requests, n, initial);
            break;

        case 6:
            CLOOK(requests, n, initial);
            break;

        default:
            break;
        }
    } while (choice != 7);

    return 0;
}