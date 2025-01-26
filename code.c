#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct processSJF {
    int pid;
    int btime;
    int atime;
};

int SJFBurstTimeSort(const void *a, const void *b) {
    return ((struct processSJF*)a)->btime - ((struct processSJF*)b)->btime;
}

int SJFArrivalTimeSort(const void *a, const void *b) {
    return ((struct processSJF*)a)->atime - ((struct processSJF*)b)->atime;
}

struct process {
    int pid;
    int arrival_time;
    int burst_time;
    int start_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
};

void sjf() {
    struct processSJF a[1000];
    int n;
    system("clear");
    printf("\nShortest-Job First Scheduling Food Ordering System\n");
    printf("\nEnter the Total Number of Orders: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Error: Number of orders must be positive.\n");
        return;
    }

    for (int j = 0; j < n; j++) {
        printf("\n");
        printf("Enter Arrival Time of Order %d: ", j + 1);
        scanf("%d", &a[j].atime);
        printf("Enter Cooking Time of Order %d (in minutes): ", j + 1);
        scanf("%d", &a[j].btime);
        a[j].pid = j + 1;
    }

    qsort(a, n, sizeof(struct processSJF), SJFBurstTimeSort);
    qsort(a, n, sizeof(struct processSJF), SJFArrivalTimeSort);

    int ttime = 0, i;
    int j, tArray[n];

    for (i = 0; i < n; i++) {
        j = i;
        while (a[j].atime <= ttime && j != n) {
            j++;
        }
        qsort(a + i, j - i, sizeof(struct processSJF), SJFBurstTimeSort);
        tArray[i] = ttime;
        ttime += a[i].btime;
    }

    tArray[i] = ttime;
    float averageWaitingTime = 0;
    float averageResponseTime = 0;
    float averageTAT = 0;

    printf("\n%-15s%-15s%-20s%-20s%-25s%-20s\n", "#Order ID", "Arrival Time", "Cooking Time", "Completion Time", "TurnAround Time", "Waiting Time");

    int index = 1;

    while (index <= n) {
        for (int i = 0; i < n; i++) {
            if (a[i].pid == index) {
                int turnaround = 0, waiting = 0;
                if ((tArray[i] - a[i].atime) < 0) {
                    waiting = 0;
                } else {
                    waiting = tArray[i] - a[i].atime;
                }
                turnaround = waiting + a[i].btime;

                printf("O[%d]          %-15d%-20d%-20d%-25d%-20d\n", a[i].pid, a[i].atime, a[i].btime, tArray[i + 1], turnaround, waiting);
                averageTAT += turnaround;
                averageWaitingTime += waiting;
            }
        }
        index++;
    }

    printf("\nAverage Waiting Time = %.2f (min)\n", (float)averageWaitingTime / (float)n);
    printf("Average Turnaround Time = %.2f (min)\n", (float)averageTAT / (float)n);
}

void srtf() {
    int n;
    struct process p[1000];
    double avg_turnaround_time, avg_waiting_time;

    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_idle_time = 0;

    int burst_remaining[100];
    int is_completed[100];
    memset(is_completed, 0, sizeof(is_completed));

    printf("\nShortest-Remaining Time First Food Ordering System\n");
    printf("\nEnter Total number of Orders: ");
    scanf("%d", &n);
    fflush(stdin);

    if (n <= 0) {
        printf("Error: Number of orders must be positive.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        printf("\n");
        printf("Enter Arrival Time of Order %d: ", i + 1);
        scanf("%d", &p[i].arrival_time);
        printf("Enter Cooking Time of Order %d (in minutes): ", i + 1);
        scanf("%d", &p[i].burst_time);
        p[i].pid = i + 1;
        burst_remaining[i] = p[i].burst_time;
    }

    int current_time = 0;
    int completed = 0;
    int prev = 0;

    while (completed != n) {
        int idx = -1;
        int mn = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && is_completed[i] == 0) {
                if (burst_remaining[i] < mn) {
                    mn = burst_remaining[i];
                    idx = i;
                }
                if (burst_remaining[i] == mn) {
                    if (p[i].arrival_time < p[idx].arrival_time) {
                        mn = burst_remaining[i];
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            if (burst_remaining[idx] == p[idx].burst_time) {
                p[idx].start_time = current_time;
                total_idle_time += p[idx].start_time - prev;
            }
            burst_remaining[idx] -= 1;
            current_time++;
            prev = current_time;

            if (burst_remaining[idx] == 0) {
                p[idx].completion_time = current_time;
                p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
                p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;

                total_turnaround_time += p[idx].turnaround_time;
                total_waiting_time += p[idx].waiting_time;

                is_completed[idx] = 1;
                completed++;
            }
        } else {
            current_time++;
        }
    }

    int min_arrival_time = INT_MAX;
    int max_completion_time = -1;

    for (int i = 0; i < n; i++) {
        min_arrival_time = min_arrival_time < p[i].arrival_time ? min_arrival_time : p[i].arrival_time;
        max_completion_time = max_completion_time > p[i].completion_time ? max_completion_time : p[i].completion_time;
    }

    avg_turnaround_time = (double) total_turnaround_time / n;
    avg_waiting_time = (double) total_waiting_time / n;

    printf("\n%-15s%-15s%-15s%-15s%-20s%-20s%-15s\n", "#Order ID", "Arrival Time", "Cooking Time", "Start Time", "Completion Time", "Turnaround Time", "Waiting Time");

    for (int i = 0; i < n; i++) {
        printf("O[%d]          %-15d%-15d%-15d%-20d%-20d%-15d\n", p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].start_time, p[i].completion_time, p[i].turnaround_time, p[i].waiting_time);
    }

    printf("\nAverage Waiting Time = %.2f (min)\n", avg_waiting_time);
    printf("Average Turnaround Time = %.2f (min)\n", avg_turnaround_time);
}

void priority() {
    int bt[20], p[20], wt[20], tat[20], pr[20], i, j, n, pos, temp;
    double total = 0, avg_wt, avg_tat;
    printf("\nPriority Scheduling Food Ordering System\n");
    printf("\nEnter Total Number of Orders: ");
    scanf("%d", &n);
    fflush(stdin);

    if (n <= 0) {
        printf("Error: Number of orders must be positive.\n");
        return;
    }

    printf("\nEnter Cooking Time and Priority\n");

    for (i = 0; i < n; i++) {
        printf("\nOrder[%d]\n", i + 1);
        printf("Cooking Time (in minutes): ");
        scanf("%d", &bt[i]);
        printf("Priority: ");
        scanf("%d", &pr[i]);
        p[i] = i + 1; //contains process number
    }

    //sorting burst time, priority and process number in ascending order using selection sort
    for (i = 0; i < n; i++) {
        pos = i;
        for (j = i + 1; j < n; j++) {
            if (pr[j] < pr[pos])
                pos = j;
        }

        temp = pr[i];
        pr[i] = pr[pos];
        pr[pos] = temp;

        temp = bt[i];
        bt[i] = bt[pos];
        bt[pos] = temp;

        temp = p[i];
        p[i] = p[pos];
        p[pos] = temp;
    }

    wt[0] = 0; //waiting time for first process is zero

    //calculate waiting time
    for (i = 1; i < n; i++) {
        wt[i] = 0;
        for (j = 0; j < i; j++)
            wt[i] += bt[j];

        total += wt[i];
    }

    avg_wt = total / n; //average waiting time
    total = 0;

    printf("\n%-15s%-15s%-15s%-15s%-15s\n", "#Order ID", "Cooking Time", "Waiting Time", "Turnaround Time", "Priority");

    for (i = 0; i < n; i++) {
        tat[i] = bt[i] + wt[i]; //calculate turnaround time
        total += tat[i];
        printf("O[%d]          %-15d%-15d%-15d%-15d\n", p[i], bt[i], wt[i], tat[i], pr[i]);
    }

    avg_tat = total / n; //average turnaround time
    printf("\nAverage Waiting Time = %.2f (min)\n", avg_wt);
    printf("Average Turnaround Time = %.2f (min)\n", avg_tat);
}


void menu() {
    system("clear");
    printf("<<<<< WELCOME TO AUTOMATED FOOD ORDERING SYSTEM (SJF, SRTF, PRIORITY) >>>>>>\n\n");
    printf("System's Purposes: \n");
    printf("i.    To minimize customer waiting time and turnaround time by using different algorithms of process scheduling.\n");
    printf("ii.   To discover the best efficient algorithm among this three process scheduling algorithms.\n\n");
    printf("Menu\n");
    printf("[1] Shortest-Job First Scheduling\n");
    printf("[2] Shortest-Remaining Time First\n");
    printf("[3] Priority Scheduling\n");
    printf("[4] Exit\n");
}

int main() {
    int choice;

here:
    menu();

    do {
        printf("Choice (1-4): ");
        if (scanf("%d", &choice) != 1) {
            printf("Error: Invalid input. Please enter a number.\n");
            scanf("%*[^\n]");
            scanf("%*c");
        }
    } while (choice < 1 || choice > 4);

    switch (choice) {
        case 1:
            sjf();
            break;
        case 2:
            srtf();
            break;
        case 3:
            priority();
            break;
        case 4:
            exit(0);
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            goto here;
    }

    return 0;
}
