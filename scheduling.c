#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define BUFFER_MAX_LENGTH 1024
#define USER_SIZE 10

/*
    Main fcfs function
*/
int fcfs(char **arr_fcfs, int pCounter)
{
    int finalTime = 0;
    int totalTime = 0;
    printf("            Order of selection by CPU:  \n\n");
    printf("                ");
    for (int i = 0; i < pCounter; i++) {
        printf("P%d ", i+1);
    }
    printf("\n\n            Individual waiting times for each process: \n\n");

    for (int j = 0; j < pCounter; j++) {
        printf("                P%d = %d\n", j+1, finalTime);
        finalTime = finalTime + atoi(arr_fcfs[j]);
        totalTime = totalTime + finalTime;
    }
    totalTime = totalTime - finalTime;
    printf("\n            Average waiting time = %0.1f \n\n", (double)totalTime/pCounter);
    return 0;
}



/*
    A struct that defines two variables for the original value and their index
*/
struct sjf_struct
{
    int value;
    int index;
};

/*
    A helper function for sjf, compare two pointers and set return accordingly
*/
int sjf_cmp(const void *a, const void *b)
{
    struct sjf_struct *a1 = (struct sjf_struct *)a;
    struct sjf_struct *a2 = (struct sjf_struct *)b;
    if ((*a1).value < (*a2).value)
        return -1;
    else if ((*a1).value > (*a2).value)
        return 1;
    else
        return 0;
}

/*
    Main sjf function, using qsort() to sort the array elements in ascending order
*/
int sjf(char **arr_sjf, int pCounter)
{
    char *temp;
    int finalTime = 0;
    int totalTime = 0;
    struct sjf_struct objects[pCounter];
    for (int i = 0; i < pCounter; i++)
    {
        objects[i].value = atoi(arr_sjf[i]);
        objects[i].index = i;
    }
    qsort(objects, pCounter, sizeof(objects[0]), sjf_cmp);
    printf("        Order of selection by CPU:  \n\n");
    printf("            ");
    for (int i = 0; i < pCounter; i++)
    {
        printf("P%d ", objects[i].index + 1); //will give 1 2 0
    }
    printf("\n\n        Individual waiting times for each process:   \n\n");
    for (int i = 0; i < pCounter; i++)
    {
        for (int j = i + 1; j < pCounter; j++)
        {
            if (atoi(arr_sjf[i]) > atoi(arr_sjf[j]))
            {
                temp = arr_sjf[i];
                arr_sjf[i] = arr_sjf[j];
                arr_sjf[j] = temp;
            }
        }
        printf("            P%d = %d\n", objects[i].index + 1, finalTime);
        finalTime = finalTime + atoi(arr_sjf[i]);
        totalTime = totalTime + finalTime;
    }
    totalTime = totalTime - finalTime;
    printf("\n        Average waiting time = %0.1f \n\n", (double)totalTime/pCounter);
    return 0;
}


/*
    Main rr function
        1. FCFS, check process time to tq. 
            a. If process time < tq, proceed FCFS
            b. Else, time remains = process time - tq, proceed FCFS
        2. That's pretty much it lol
*/
int rr(char **arr_rr, int pCounter, int tq)
{
    int finalTime = 0;
    int totalTime = 0;
    int timeRemains = 0;
    printf("            Order of selection by CPU:  \n\n");
    printf("                ");
    for (int i = 0; i < pCounter; i++) {
        printf("P%d ", i+1);
    }
    return 0;
}







int main(int argc, char const *argv[])
{
    int processTime, processNum, timeQuantum;
    int row = 2, col;
    int lineCounter = 0;
    char const* const fileName = argv[1];
    char line [BUFFER_MAX_LENGTH];
    FILE *file;
    char *split;
    char delim[] = " ";
    char *lineArr[BUFFER_MAX_LENGTH];
    char *splitArr[BUFFER_MAX_LENGTH];
    char *processArr[BUFFER_MAX_LENGTH];
    file = fopen("cpu_scheduling_input_file.txt", "r");
    if (!file)
    {
         fprintf(stderr, "error: could not open textfile: cpu_scheduling_input_file.txt\n");
         return EXIT_FAILURE;
    }
    while (fgets(line, sizeof(line), file) != NULL) {
        lineArr[lineCounter] = malloc(sizeof(line));
        strcpy(lineArr[lineCounter], line);
        lineCounter++;
    }

    printf("\n- - - - - - - - - - - - S - T - A - R - T - - - - - - - - - - - - \n\n");
    printf("%d ready queues have been read and stored\n\n", lineCounter);

    for (size_t j = 0; j < lineCounter; j++) {
        printf("\n- - - - - - - - - - - - S - P - A - C - E - - - - - - - - - - - - \n\n");
        printf("Ready queue %zu\n\n", j+1);
        // first, count number of P's in one line to determine its number of processes
        int pCounter = 0;
        for (size_t k = 0; k < strlen(lineArr[j]); k++) {
            if(lineArr[j][k] == 'p')
            pCounter++;
        }
        printf("    Number of processes: %d\n\n", pCounter);
        // printf("    %s\n", lineArr[j]);
        int splitCounter = 0;
        split = strtok (lineArr[j], delim);
        while (split != NULL) {
            splitArr[splitCounter] = malloc(strlen(lineArr[j]));
            strcpy(splitArr[splitCounter], split);
            splitCounter++;
            split = strtok(NULL, delim);
        }
        timeQuantum = atoi(splitArr[3]);
        printf("    Time quantum: %d\n\n", timeQuantum);


        // another array corresponding to its process times
        int proCounter = 0;
        for (int p = 5; p < splitCounter; p = p + 2) {
        processArr[proCounter] = (char*)malloc(strlen(splitArr[p]) + 1);
        strcpy(processArr[proCounter], splitArr[p]);
        proCounter++;
        }
        // printf("        Ready Queue %lu Applying FCFS Scheduling:\n\n", j+1);
        // fcfs(processArr, proCounter);
        // printf("    Ready Queue %zu Applying SJF Scheduling...\n\n", j+1);
        // sjf(processArr, proCounter);
        printf("    Ready Queue %zu Applying RR Scheduling...\n\n", j+1);
        rr(processArr, proCounter, timeQuantum);
    }



    fclose(file);

    return 0;
}

