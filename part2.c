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

/**
 * Main fcfs function
*/
int fcfs(char **arr_fcfs, int pCounter, FILE *new_file)
{
    int finalTime = 0;
    int totalTime = 0;
    fprintf(new_file, "        Order of selection by CPU:  \n\n");
    fprintf(new_file, "            ");
    // Print the order selected by the CPU as-is
    for (int i = 0; i < pCounter; i++) 
    {
        fprintf(new_file, "P%d ", i+1);
    }
    fprintf(new_file, "\n\n        Individual waiting times for each process: \n\n");
    // Loop through the array to calculate each process time
    for (int j = 0; j < pCounter; j++) 
    {
        fprintf(new_file, "            P%d = %d\n", j+1, finalTime);
        finalTime = finalTime + atoi(arr_fcfs[j]);
        totalTime = totalTime + finalTime;
    }
    // Calculate total time needed for average time
    totalTime = totalTime - finalTime;
    fprintf(new_file, "\n        Average waiting time = %0.1f \n\n", (double)totalTime/pCounter);
    return 0;
}


/**
 * A struct that defines two variables for the original value and their index
*/
struct sjf_struct
{
    int value;
    int index;
};

/**
 * A helper function for sjf, compare two pointers and set return accordingly
*/
int sjf_cmp(const void *a, const void *b)
{
    struct sjf_struct *a1 = (struct sjf_struct *)a;
    struct sjf_struct *a2 = (struct sjf_struct *)b;
    // Compare the first value to the second value
    if ((*a1).value < (*a2).value)
        // Return -1 if the first value is smaller than the second value
        return -1;
    else if ((*a1).value > (*a2).value)
        // Return 1 if the first value is larger than the second value
        return 1;
    else
        // Return 0 if the first value is equal to the second value
        return 0;
}

/**
 * Main sjf function, using qsort() to sort the array elements in ascending order
*/
int sjf(char **arr_sjf, int pCounter, FILE *new_file)
{
    int temp;
    int finalTime = 0;
    int totalTime = 0;
    int localArray [pCounter];
    struct sjf_struct objects[pCounter];
    // Preserve arr_sjf's original elements' index and value
    for (int i = 0; i < pCounter; i++)
    {
        objects[i].value = atoi(arr_sjf[i]);
        objects[i].index = i;
    }
    // Copy the elements from the original array to a local array to avoid overwriting
    for (int i = 0; i < pCounter; i++)
    {
        localArray[i] = atoi(arr_sjf[i]);
    }
    // Sort the array into ascending order
    qsort(objects, pCounter, sizeof(objects[0]), sjf_cmp);
    fprintf(new_file, "        Order of selection by CPU:  \n\n");
    fprintf(new_file, "            ");
    // Loop through the whole array and print the order selected by CPU
    for (int i = 0; i < pCounter; i++)
    {
        fprintf(new_file,      "P%d ", objects[i].index + 1); //will give 1 2 0
    }
    fprintf(new_file, "\n\n        Individual waiting times for each process:   \n\n");
    // Print individual waiting time in the order of selected by the CPU
    for (int i = 0; i < pCounter; i++)
    {
        for (int j = i + 1; j < pCounter; j++)
        {
            // Sort the local array in ascending order
            if (localArray[i] > localArray[j])
            {
                temp = localArray[i];
                localArray[i] = localArray[j];
                localArray[j] = temp;
            }
        }
        fprintf(new_file, "            P%d = %d\n", objects[i].index + 1, finalTime);
        finalTime = finalTime + localArray[i];
        totalTime = totalTime + finalTime;
    }
    // Calculate total time needed for average time
    totalTime = totalTime - finalTime;
    fprintf(new_file, "\n        Average waiting time = %0.1f \n\n", (double)totalTime/pCounter);
    return 0;
}


/**
 * Main rr function
*/
int rr(char **arr_rr, int pCounter, int tq)
{
    int finished = 0; // 0 = finished, 1 = not finished yet
    int process_remains;
    int pQueue [pCounter]; 
    int posArr [pCounter];      // Array stores the order of finishes       
    int taArr [pCounter];       // Array stores the turnaround time in the order of finishes
    printf( "        Order of selection by CPU:  \n\n");
    printf( "            ");
    // Copy each process time to a local queue and print them out as FCFS
    for (int i = 0; i < pCounter; i++)
    {
        pQueue[i] = atoi(arr_rr[i]);
        printf( "P%d ", i + 1);
    }
    // Set all elements in positionArray and turnaroundArray to 0
    for (int i = 0; i < pCounter; i++)
    {
        posArr[i] = 0;
        taArr[i] = 0;
    }
    // Loop until processes has a CPU burst time smaller than time quantum
    while (finished == 0)
    {
        // Set up remainding processes. Exit while loop when remainding processes are 0
        process_remains = pCounter;
        // Loop through each process stored in the local queue 
        for (int i = 0; i < pCounter; i++)
        { 
            // If the ith process has a CPU burst time greater than the time quantum
            if (pQueue[i] > tq)
            {
                // Print out its process number
                printf("P%d ", i + 1);
                // Add time quantum to ith element's turnaround time
                taArr[i] += tq;
                // Subtract time quantum from ith element's CPU burst time
                pQueue[i] -= tq;
            }
            // If the ith process has a CPU burst time smaller than or equals to the time quantum
            else if (pQueue[i] <= tq)
            {
                // Add (the rest of) CPU burst time to ith element's turnaround time
                taArr[i] += pQueue[i];
                // Add ith process's process number to the ith position in the position array
                posArr[i] = i + 1;
                // Set the CPU burst time to 0
                pQueue[i] = 0;
                // Decrement remaining processes number by 1
                process_remains --;
            }
            // Check if there are any processes left
            if (process_remains == 0)
            {
                // Set finished = 1 if there are none left
                finished = 1;
            }   
        }
    }  
    printf("\n\n        Turnaround time for each process: \n\n");
    for (int i = 0; i < pCounter; i++)
    {
        printf("            P%d = %d\n", posArr[i], taArr[i]);
    }
    
    return 0;
}


/**
 * This is the main function. 
 * What it does:
 * 
 *      - Take file input
 *      - Process file input
 *      - Create file output
 *      - Call functions to write to the output file
*/
int main(int argc, char const *argv[])
{
    char *split;
    char delim[] = " ";
    char const* const fileName = argv[1];
    int processTime, processNum, timeQuantum, col, row = 2, lineCounter = 0;
    char line [BUFFER_MAX_LENGTH];
    char *lineArr[BUFFER_MAX_LENGTH];
    char *splitArr[BUFFER_MAX_LENGTH];
    char *processArr[BUFFER_MAX_LENGTH];
    // File input
    FILE *file;
    file = fopen("cpu_scheduling_input_file.txt", "r");
    // Check if the file can be opened
    if (!file)
    {
        fprintf(stderr, "error: could not open textfile: cpu_scheduling_input_file.txt\n");
        return EXIT_FAILURE;
    }
    // File outpout
    FILE *new_file;
    new_file = fopen("cpu_scheduling_output_file.txt", "a");
    // Check if the file can be opened
    if (!new_file)
    {
        fprintf(stderr, "error: could not open textfile: cpu_scheduling_output_file.txt\n");
        return EXIT_FAILURE;
    }
    // Read from the input file line by line, and store them into their own array
    // e.g. line 1 will store in lineArr[0], line 2 will store in lineArr[1], so on
    while (fgets(line, sizeof(line), file) != NULL) 
    {
        // Dynamically allocate memory of lineCounter size, to each array
        lineArr[lineCounter] = malloc(sizeof(line));
        // Copy all elements (including white spaces) of each line to the corresponding array
        strcpy(lineArr[lineCounter], line);
        // Increment lineCounter by 1
        lineCounter++;
    }
    printf("\n    Output wrote to cpu_scheduling_output_file.txt \n\n");
    // Print to file start from here
    fprintf(new_file, "- - - - - - - - - - - - - - - - - - - - -  S - T - A - R - T - - - - - - - - - - - - - - - - - - - - - \n\n");
    fprintf(new_file, "%d ready queues have been read and stored\n", lineCounter);
    // This is the most important for loop ever (litrally)
    // Each line (queue) will enter this loop once
    // And magic happens, each queue will get processed and send to each queueing functions
    for (size_t j = 0; j < lineCounter; j++) 
    {
        fprintf(new_file, "\n- - - - - - - - - - - - Q - U - E - U - E - - %lu - - - - - - - - - - - - \n\n", j + 1);
        // Count number of P's in one line to determine its number of processes
        int pCounter = 0;
        for (size_t k = 0; k < strlen(lineArr[j]); k++) 
        {
            if(lineArr[j][k] == 'p')
            pCounter++;
        }
        fprintf(new_file, "Number of processes: %d\n\n", pCounter);
        int splitCounter = 0;
        // Tokenize the line by white spaces
        split = strtok (lineArr[j], delim);
        // Store tokenized string to a *NEW* array, called splitArr
        while (split != NULL) 
        {
            splitArr[splitCounter] = malloc(strlen(lineArr[j]));
            strcpy(splitArr[splitCounter], split);
            splitCounter++;
            split = strtok(NULL, delim);
        }
        // Get the time quantum
        timeQuantum = atoi(splitArr[3]);
        fprintf(new_file, "Time quantum: %d\n\n", timeQuantum);
        int proCounter = 0;
        // For each splitArray, start looping from the 5th position
        // 5th element is the P1's CUP burst time
        // Loop through the rest of the splitArr and get every other element.
        // Store them in a *NEW* array, called processArr
        for (int p = 5; p < splitCounter; p = p + 2) 
        {
            processArr[proCounter] = (char*)malloc(strlen(splitArr[p]) + 1);
            strcpy(processArr[proCounter], splitArr[p]);
            proCounter++;
        }
        // // Calls fcfs() and pass in processArr, proCounte and new_file
        // fprintf(new_file, "    Ready Queue %lu Applying FCFS Scheduling:\n\n", j+1);
        // fcfs(processArr, proCounter, new_file);
        // // Calls sjf() and pass in processArr, proCounte and new_file
        // fprintf(new_file, "    Ready Queue %zu Applying SJF Scheduling...\n\n", j+1);
        // sjf(processArr, proCounter, new_file);
        // Calls rr() and pass in processArr, proCounte, time quantum and new_file
        printf("    Ready Queue %zu Applying RR Scheduling...\n\n", j+1);
        rr(processArr, proCounter, timeQuantum);
    }
    // Close all files
    fclose(new_file);
    fclose(file);
    return 0;
}