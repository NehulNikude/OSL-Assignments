#include <stdio.h>
#include <limits.h>

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

int findShortestJob(struct Process proc[], int n, int current_time)
{
    int shortest = -1;
    int min_burst = INT_MAX;

    for (int i = 0; i < n; i++)
    {
        if (proc[i].arrival_time <= current_time && proc[i].remaining_time > 0)
        {
            if (proc[i].remaining_time < min_burst)
            {
                min_burst = proc[i].remaining_time;
                shortest = i;
            }
            if (proc[i].remaining_time == min_burst)
            {
                if (proc[i].arrival_time < proc[shortest].arrival_time)
                {
                    shortest = i;
                }
            }
        }
    }
    return shortest;
}

void updateProcessMetrics(struct Process *proc, int current_time)
{
    proc->completion_time = current_time;
    proc->turnaround_time = proc->completion_time - proc->arrival_time;
    proc->waiting_time = proc->turnaround_time - proc->burst_time;
}

void displayTimeline(int time, int process_id)
{
    if (process_id == -2)
    {
        printf("%d | IDLE\n", time);
    }
    else
    {
        printf("%d | P%d\n", time, (process_id + 1));
    }
}

void executeProcesses(struct Process proc[], int n)
{
    int current_time = 0;
    int completed = 0;
    int prev = -1;

    printf("\nExecution Order\n");
    printf("TIME | PROCESS\n");
    printf("--------------\n");

    while (completed != n)
    {
        int shortest = findShortestJob(proc, n, current_time);

        if (shortest == -1)
        {
            if (prev != -2)
            {
                displayTimeline(current_time, -2);
                prev = -2;
            }
            current_time++;
        }
        else
        {
            if (prev != shortest)
            {
                displayTimeline(current_time, shortest);
                prev = shortest;
            }
            proc[shortest].remaining_time--;
            current_time++;

            if (proc[shortest].remaining_time == 0)
            {
                completed++;
                updateProcessMetrics(&proc[shortest], current_time);
            }
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
    int n;
    printf("\nEnter Number Of Processes: ");
    scanf("%d", &n);

    struct Process proc[n];

    inputProcessDetails(proc, n);
    executeProcesses(proc, n);
    displayResults(proc, n);
}
