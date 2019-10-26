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
int fcfs(char **arr_fcfs, int pCounter, FILE *new_file)
{
    int finalTime = 0;
    int totalTime = 0;
    fprintf(new_file, "        Order of selection by CPU:  \n\n");
    fprintf(new_file, "            ");
    for (int i = 0; i < pCounter; i++) {
        fprintf(new_file, "P%d ", i+1);
    }
    fprintf(new_file, "\n\n        Individual waiting times for each process: \n\n");

    for (int j = 0; j < pCounter; j++) {
        fprintf(new_file, "            P%d = %d\n", j+1, finalTime);
        finalTime = finalTime + atoi(arr_fcfs[j]);
        totalTime = totalTime + finalTime;
    }
    totalTime = totalTime - finalTime;
    fprintf(new_file, "\n        Average waiting time = %0.1f \n\n", (double)totalTime/pCounter);
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
int sjf(char **arr_sjf, int pCounter, FILE *new_file)
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
    fprintf(new_file, "        Order of selection by CPU:  \n\n");
    fprintf(new_file, "            ");
    for (int i = 0; i < pCounter; i++)
    {
        fprintf(new_file,      "P%d ", objects[i].index + 1); //will give 1 2 0
    }
    fprintf(new_file, "\n\n        Individual waiting times for each process:   \n\n");
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
        fprintf(new_file, "            P%d = %d\n", objects[i].index + 1, finalTime);
        finalTime = finalTime + atoi(arr_sjf[i]);
        totalTime = totalTime + finalTime;
    }
    totalTime = totalTime - finalTime;
    fprintf(new_file, "\n        Average waiting time = %0.1f \n\n", (double)totalTime/pCounter);
    return 0;
}


/*
    Main rr function
*/
int rr(char **arr_rr, int pCounter, int tq, FILE *new_file)
{
    int finished = 0; // 0 = finished, 1 = not finished yet
    int process_remains;
    int pQueue [pCounter]; 
    fprintf(new_file, "            Order of selection by CPU:  \n\n");
    fprintf(new_file, "                ");
    for (int i = 0; i < pCounter; i++)
    {
        pQueue[i] = atoi(arr_rr[i]);
        fprintf(new_file, "P%d ", i + 1);
    }
    while (finished == 0)
    {
        process_remains = pCounter;
        for (int i = 0; i < pCounter; i++)
        {
            if (pQueue[i] > tq)
            {
                pQueue[i] = pQueue[i] - tq;
                fprintf(new_file, "P%d ", i + 1);
            }
            else if (pQueue[i] < tq)
            {
                process_remains --;
            }
            else if (pQueue[i] == tq)
            {
                process_remains --;
            }
            if (process_remains == 0)
            {
                finished = 1;
            }
        }
    }  
    fprintf(new_file, "\n\n");
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
    FILE *new_file;
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
    new_file = fopen("cpu_scheduling_output_file.txt", "a");
    if (!new_file)
    {
        fprintf(stderr, "error: could not open textfile: cpu_scheduling_output_file.txt\n");
        return EXIT_FAILURE;
    }
    while (fgets(line, sizeof(line), file) != NULL) 
    {
        lineArr[lineCounter] = malloc(sizeof(line));
        strcpy(lineArr[lineCounter], line);
        lineCounter++;
    }
    fprintf(new_file, "- - - - - - - - - - - - - - - - - - - - -  S - T - A - R - T - - - - - - - - - - - - - - - - - - - - - \n\n");
    fprintf(new_file, "%d ready queues have been read and stored\n", lineCounter);
    for (size_t j = 0; j < lineCounter; j++) 
    {
        fprintf(new_file, "\n- - - - - - - - - - - - Q - U - E - U - E - - %d - - - - - - - - - - - - \n\n", j + 1);
        // first, count number of P's in one line to determine its number of processes
        int pCounter = 0;
        for (size_t k = 0; k < strlen(lineArr[j]); k++) 
        {
            if(lineArr[j][k] == 'p')
            pCounter++;
        }
        fprintf(new_file, "Number of processes: %d\n\n", pCounter);
        int splitCounter = 0;
        split = strtok (lineArr[j], delim);
        while (split != NULL) 
        {
            splitArr[splitCounter] = malloc(strlen(lineArr[j]));
            strcpy(splitArr[splitCounter], split);
            splitCounter++;
            split = strtok(NULL, delim);
        }
        timeQuantum = atoi(splitArr[3]);
        fprintf(new_file, "Time quantum: %d\n\n", timeQuantum);
        // another array corresponding to its process times
        int proCounter = 0;
        for (int p = 5; p < splitCounter; p = p + 2) 
        {
        processArr[proCounter] = (char*)malloc(strlen(splitArr[p]) + 1);
        strcpy(processArr[proCounter], splitArr[p]);
        proCounter++;
        }
        fprintf(new_file, "    Ready Queue %lu Applying FCFS Scheduling:\n\n", j+1);
        fcfs(processArr, proCounter, new_file);
        fprintf(new_file, "    Ready Queue %zu Applying RR Scheduling...\n\n", j+1);
        rr(processArr, proCounter, timeQuantum, new_file);
        fprintf(new_file, "    Ready Queue %zu Applying SJF Scheduling...\n\n", j+1);
        sjf(processArr, proCounter, new_file);

    }


    fclose(new_file);
    fclose(file);

    return 0;
}

