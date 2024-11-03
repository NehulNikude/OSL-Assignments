#include <stdio.h>

#define MAX_QUEUE 100

struct Process
{
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
};

struct Queue
{
    int items[MAX_QUEUE];
    int front;
    int rear;   
};

void initQueue(struct Queue *q)
{
    q->front = -1;
    q->rear = -1;
}

void enqueue(struct Queue *q, int value)
{
    if (q->front == -1)
    {
        q->front = 0;
    }
    q->rear++;
    q->items[q->rear] = value;
}

int dequeue(struct Queue *q)
{
    int item = q->items[q->front];
    q->front++;
    if (q->front > q->rear)
    {
        q->front = -1;
        q->rear = -1;
    }
    return item;
}

void inputProcessDetails(struct Process proc[], int n) 
{
    for(int i = 0; i < n; i++)
    {
        printf("\nEnter Details For Process %d:\n", i+1);
        proc[i].pid = i + 1;
        printf("Arrival Time: ");
        scanf("%d", &proc[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", &proc[i].burst_time);
        proc[i].remaining_time = proc[i].burst_time;
    }
}

void executeProcesses(struct Process proc[], int n, int quantum) {
    struct Queue readyQueue;
    initQueue(&readyQueue);
    
    int current_time = 0;
    int completed = 0;
    
    printf("\nExecution Order\n");
    printf("TIME | PROCESS\n");
    printf("--------------\n");

    while (completed != n) 
    {
        // Check for new arrivals
        int found_ready = 0;
        for(int i = 0; i < n; i++) 
        {
            if(proc[i].arrival_time <= current_time && proc[i].remaining_time > 0) 
            {
                if(readyQueue.front == -1) 
                {
                    enqueue(&readyQueue, i);
                }
                found_ready = 1;
            }
        }

        // If no process is ready, show IDLE and increment time
        if(!found_ready) 
        {
            printf("%d | IDLE\n", current_time);
            current_time++;
            continue;
        }

        // Process execution
        int current_process = dequeue(&readyQueue);
        printf("%d | P%d\n", current_time, current_process + 1);
        
        int execute_time = (proc[current_process].remaining_time < quantum) ? proc[current_process].remaining_time : quantum;
        
        current_time += execute_time;
        proc[current_process].remaining_time -= execute_time;

        // Check for new arrivals during execution
        for(int i = 0; i < n; i++) 
        {
            if(i != current_process && 
               proc[i].arrival_time <= current_time && 
               proc[i].remaining_time > 0 &&
               readyQueue.front == -1) 
            {
                enqueue(&readyQueue, i);
            }
        }

        // If current process is not finished, add it back to queue
        if(proc[current_process].remaining_time > 0) 
        {
            enqueue(&readyQueue, current_process);
        }
        else
        {
            completed++;
            proc[current_process].completion_time = current_time;
            proc[current_process].turnaround_time = proc[current_process].completion_time - proc[current_process].arrival_time;
            proc[current_process].waiting_time = proc[current_process].turnaround_time - proc[current_process].burst_time;
        }
    }
    printf("%d | END\n", current_time);
}

void displayResults(struct Process proc[], int n) 
{
    printf("\nProcess Details:\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("----------------------------------------\n");
    float avg_tat = 0, avg_wt = 0;
    
    for(int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
            proc[i].pid,
            proc[i].arrival_time,
            proc[i].burst_time,
            proc[i].completion_time,
            proc[i].turnaround_time,
            proc[i].waiting_time);
            
        avg_tat += proc[i].turnaround_time;
        avg_wt += proc[i].waiting_time;
    }
    
    avg_tat /= n;
    avg_wt /= n;
    
    printf("\nAverage Turnaround Time: %.2f", avg_tat);
    printf("\nAverage Waiting Time: %.2f\n", avg_wt);
}

int main()
{
    int n, quantum;
    printf("Enter Number of Processes: ");
    scanf("%d", &n);
    printf("Enter Time Quantum: ");
    scanf("%d", &quantum);
    
    struct Process proc[n];
    
    inputProcessDetails(proc, n);
    executeProcesses(proc, n, quantum);
    displayResults(proc, n);
    
    return 0;
}

