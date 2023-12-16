#include <stdio.h>
#include <stdlib.h>

// Process structure
struct Process {
    int processId;
    int priority;
    int burstTime;
    int remainingTime;
};

// Multilevel Queue Scheduler structure
struct MultilevelQueueScheduler {
    struct Process ***queues;
    int *queueSizes;
    int numQueues;
    int timeQuantum;
};

// enqueue a process into the appropriate queue
void enqueueProcess(struct MultilevelQueueScheduler *scheduler, struct Process *process) {
    int priority = process->priority;
    int newSize = ++(scheduler->queueSizes[priority]);
    scheduler->queues[priority] = realloc(scheduler->queues[priority], newSize * sizeof(struct Process*));
    scheduler->queues[priority][newSize - 1] = process;
}

// Function to run the scheduler
void runScheduler(struct MultilevelQueueScheduler *scheduler) {
    int timeElapsed = 0;

    while (1) {
        int allQueuesEmpty = 1;

        for (int i = 0; i < scheduler->numQueues; i++) {
            int queueSize = scheduler->queueSizes[i];

            if (queueSize > 0) {
                allQueuesEmpty = 0;

                struct Process *currentProcess = scheduler->queues[i][0];

                if (currentProcess->remainingTime <= scheduler->timeQuantum) {
                    // Process completes execution
                    timeElapsed += currentProcess->remainingTime;
                    currentProcess->remainingTime = 0;
                    for (int j = 0; j < queueSize - 1; j++) {
                        scheduler->queues[i][j] = scheduler->queues[i][j + 1];
                    }
                    scheduler->queueSizes[i]--;

                    // Process completion logic
                    printf("Process %d completed.\n", currentProcess->processId);
                    free(currentProcess);
                } else {
                    // Process executes for a time quantum
                    timeElapsed += scheduler->timeQuantum;
                    currentProcess->remainingTime -= scheduler->timeQuantum;

                    // Move the process to the next lower priority queue
                    for (int j = 0; j < queueSize - 1; j++) {
                        scheduler->queues[i][j] = scheduler->queues[i][j + 1];
                    }
                    scheduler->queueSizes[i]--;
                    enqueueProcess(scheduler, currentProcess);
                }
            }
        }

        if (allQueuesEmpty) {
            break;
        }
    }

    printf("Simulation completed in %d units.\n", timeElapsed);
}

int main() {
    // Initialize the scheduler
    struct MultilevelQueueScheduler scheduler;
    printf("Enter the number of queues: ");
    scanf("%d", &scheduler.numQueues);

    printf("Enter the time quantum : ");
    scanf("%d", &scheduler.timeQuantum);

    scheduler.queues = (struct Process ***)malloc(scheduler.numQueues * sizeof(struct Process**));
    scheduler.queueSizes = (int *)malloc(scheduler.numQueues * sizeof(int));

    for (int i = 0; i < scheduler.numQueues; i++) {
        scheduler.queues[i] = NULL;
        scheduler.queueSizes[i] = 0;
    }

    // Create processes and enqueue them
    for (int i = 0; i < scheduler.numQueues; i++) {
        printf("Enter the number of processes for queue %d: ", i + 1);
        int numProcesses;
        scanf("%d", &numProcesses);

        for (int j = 0; j < numProcesses; j++) {
            struct Process *process = (struct Process *)malloc(sizeof(struct Process));
            process->processId = j + 1;
            process->priority = i;
            printf("Enter burst time for process %d: ", process->processId);
            scanf("%d", &process->burstTime);
            process->remainingTime = process->burstTime;

            enqueueProcess(&scheduler, process);
        }
    }
printf("-----------Simulation completed priority --------------\n");
    // Run the scheduler
    runScheduler(&scheduler);


    for (int i = 0; i < scheduler.numQueues; i++) {
        free(scheduler.queues[i]);
    }
    free(scheduler.queues);
    free(scheduler.queueSizes);

    return 0;
}
