#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int available[MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int maximum[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];

int num_processes;
int num_resources;

void initializeSystem();
bool isSafe();
bool requestResource(int process_id, int request[]);

int main()
{
    initializeSystem();

    if (!isSafe())
    {
        printf("\nInitial State Is Unsafe.\n");
        printf("Exiting.\n");
        return 1;
    }

    int choice;
    do
    {
        printf("\n1. Request Resources\n");
        printf("2. Exit\n");
        printf("\nEnter Your Choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            int process_id;
            int request[MAX_RESOURCES];

            printf("\nEnter Process ID (0-%d): ", num_processes - 1);
            scanf("%d", &process_id);
            if (process_id < 0 || process_id >= num_processes)
            {
                printf("Invalid Process ID!\n");
                continue;
            }
            
            printf("Enter Resource Request:\n");
            for (int i = 0; i < num_resources; i++)
            {
                scanf("%d", &request[i]);
            }

            requestResource(process_id, request);
        }
    } while (choice != 2);
    
    return 0;
}

void initializeSystem()
{
    printf("\nEnter Number Of Processes: ");
    scanf("%d", &num_processes);

    printf("Enter Number Of Resources: ");
    scanf("%d", &num_resources);

    printf("Enter Available Resources:\n");
    for (int i = 0; i < num_resources; i++)
    {
        scanf("%d", &available[i]);
    }

    printf("Enter Maximum Matrix:\n");
    for (int i = 0; i < num_processes; i++)
    {
        for (int j = 0; j < num_resources; j++)
        {
            scanf("%d", &maximum[i][j]);
        }
    }

    printf("Enter Allocation Matrix:\n");
    for (int i = 0; i < num_processes; i++)
    {
        for (int j = 0; j < num_resources; j++)
        {
            scanf("%d", &allocation[i][j]);
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

bool isSafe()
{
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = {false};
    int safe_sequence[MAX_PROCESSES];
    int count = 0;

    for (int i = 0; i < num_resources; i++)
    {
        work[i] = available[i];
    }
    
    while (count != num_processes)
    {
        bool found = false;

        for (int i = 0; i < num_processes; i++)
        {
            if (!finish[i])
            {
                bool possible = true;

                for (int j = 0; j < num_resources; j++)
                {
                    if (need[i][j] > work[j])
                    {
                        possible = false;
                        break;
                    }
                }

                if (possible)
                {
                    for (int j = 0; j < num_resources; j++)
                    {
                        work[j] += allocation[i][j];
                    }
                    
                    safe_sequence[count] = i;
                    finish[i] = true;
                    count++;
                    found = true;
                }
            }
        }
        
        if (!found)
        {
            printf("\nSystem Is In Unsafe State!\n");
            return false;
        }
    }

    printf("\nSystem Is In Safe State.\n");
    printf("Safe Sequence: ");
    for (int i = 0; i < num_processes; i++)
    {
        printf("P%d ", safe_sequence[i]);
    }
    printf("\n");

    return true;
}

bool requestResource(int process_id, int request[])
{
    for (int i = 0; i < num_resources; i++)
    {
        if (request[i] > need[process_id][i])
        {
            printf("\nError: Request Exceeds Maximum Claim!\n");
            return false;
        }
        if (request[i] > available[i])
        {
            printf("\nError: Requested Resources Are Currently Unavailable!\n");
            printf("Process Must Wait!\n");
            return false;
        }
    }

    for (int i = 0; i < num_resources; i++)
    {
        available[i] -= request[i];
        allocation[process_id][i] += request[i];
        need[process_id][i] -= request[i];
    }
    
    if (isSafe)
    {
        printf("\nResources Allocated Successfully!\n");
        return true;
    }
    
    for (int i = 0; i < num_resources; i++)
    {
        available[i] += request[i];
        allocation[process_id][i] -= request[i];
        need[process_id][i] += request[i];
    }
    
    printf("\nResource Allocation Denied - Would Lead To Unsafe State!\n");
    return false;
}