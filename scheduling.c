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

//这玩意好鸡巴难 我他妈透了 卧槽

/*
1. initialize variables
    a. number of processes
    b. tq
2. read string by string (queue by queue)
    a. create 2d array for each line
        - 1D: process number
        - 2D: process times
3. implement 3 functions
    a. each function will take a 2d array and a processNum as input, rr will take an extra "timeQuantum" parameter
    b. perform corresponding algorithms on this array
    c. output to main
4. main
    a. take all output and write to new file
*/


/*
FIRST COME FIRST SERVE
1. Make total process time counter
2. Loop through the array
3. Increment the processtime by its value
4. Print the order
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








int sjf(char **arr_sjf, int pCounter)
{
    char keys[pCounter];
    char *temp;
    int k;
    int finalTime = 0;
    int totalTime = 0;
    printf("        Order of selection by CPU:  \n\n");
    // for (k = 0; k < pCounter; k++)
    // {
    //     keys[atoi(arr_sjf[k])-1] = k;
    //     // keys[k] = atoi(arr_sjf[k]);
    //     // printf("%d - %d\n", atoi(arr_sjf[k]), k+1);
    // }
    for (k = 0; k < pCounter; k++)
    {
        keys[k] = atoi(arr_sjf[k]);
        printf("P%d = %d\n", k + 1, keys[k] );
    }

    printf("           ");
    // for (k = 0; k < pCounter; k++) {
    //     printf("P%d ", keys[k]+1);
    // }
    printf("\n\n        Individual waiting times for each process:   \n\n");
    for (int i = 0; i < pCounter; i++)
    {
        keys[atoi(arr_sjf[i]) - 1] = i;
        for (int j = i + 1; j < pCounter; j++)
        {
            if (atoi(arr_sjf[i]) > atoi(arr_sjf[j]))
            {
                temp = arr_sjf[i];
                arr_sjf[i] = arr_sjf[j];
                arr_sjf[j] = temp;
            }
        }
        // printf("            P%d = %d\n", keys[i]+1, finalTime);
        printf("            P%d = %d\n", keys[i], finalTime);
        finalTime = finalTime + atoi(arr_sjf[i]);
        totalTime = totalTime + finalTime;
    }
    totalTime = totalTime - finalTime;
    printf("\n        Average waiting time = %0.1f \n\n", (double)totalTime/pCounter);
    return 0;
}









int rr(char **arr_rr, int pCounter, int tq)
{
    printf("        Array received by rr, time quantum = %d: \n", tq);
    for (int i = 0; i < pCounter-1; i++) {
        printf("            %s\n", arr_rr[i]);
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
    if (argc != 2)
    {
        printf("Invalid number of arguments\n\n");
        exit(0);
    } else
    {
        file = fopen(fileName, "r");
    }

    if (!file)
    {
         fprintf(stderr, "error: could not open textfile: %s\n", argv[1]);
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
        printf("    Ready Queue %zu Applying SJF Scheduling...\n\n", j+1);
        sjf(processArr, proCounter);
        // printf("    Ready Queue %zu Applying RR Scheduling...\n", j+1);
        // rr(processArr, proCounter, timeQuantum);
    }



    fclose(file);

    return 0;
}
