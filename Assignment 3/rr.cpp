#include <iostream>
#include <queue>

using namespace std;

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

void inputProcessDetails(struct Process proc[], int n)
{
    for (int i = 0; i < n; i++)
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

void scheduler(struct Process proc[], int n, int quantum)
{
    queue<int> ready_queue;
    int current_time = 0;
    int completed = 0;

    printf("\nExecution Order\n");
    printf("TIME | PROCESS\n");
    printf("--------------\n");

    if (proc[0].arrival_time > 0)
    {
        printf("%d | IDLE\n", 0);
        current_time += proc[0].arrival_time;
    }
    
    ready_queue.push(0);

    while (completed != n)
    {
        if (ready_queue.empty())
        {
            printf("%d | IDLE\n", current_time);
            current_time++;
            for (int i = 0; i < n; i++)
            {
                if (proc[i].arrival_time == current_time && proc[i].remaining_time > 0)
                {
                    ready_queue.push(i);
                    break;
                }
            }
            continue;
        }
        
        int current_process = ready_queue.front();
        ready_queue.pop();
        printf("%d | P%d\n", current_time, current_process + 1);

        int execute_time = (proc[current_process].remaining_time < quantum) ? proc[current_process].remaining_time : quantum;
        current_time += execute_time;
        proc[current_process].remaining_time -= execute_time;

        for (int i = 0; i < n; i++)
        {
            if (proc[i].arrival_time > (current_time - execute_time) && proc[i].arrival_time <= current_time && proc[i].remaining_time > 0)
            {
                ready_queue.push(i);
            }
        }
        
        if (proc[current_process].remaining_time > 0)
        {
            ready_queue.push(current_process);
        }
        else
        {
            proc[current_process].completion_time = current_time;
            proc[current_process].turnaround_time = proc[current_process].completion_time - proc[current_process].arrival_time;
            proc[current_process].waiting_time = proc[current_process].turnaround_time - proc[current_process].burst_time;
            completed++;
        }
    }
    printf("%d | END\n", current_time);
}

void displayResults(struct Process proc[], int n)
{
    printf("\nProcess Details:\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("------------------------\n");
    float avg_tat = 0, avg_wt = 0;

    for (int i = 0; i < n; i++)
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

    printf("\nAverage Turnaround Time: %.2f\n", avg_tat);
    printf("Average Waiting Time: %.2f\n", avg_wt);
}

int main()
{
    int n, quantum;
    printf("\nEnter Number Of Processes: ");
    scanf("%d", &n);
    printf("Enter Time Quantum: ");
    scanf("%d", &quantum);

    struct Process proc[n];

    inputProcessDetails(proc, n);
    scheduler(proc, n, quantum);
    displayResults(proc, n);
}