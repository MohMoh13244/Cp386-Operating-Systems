#include <stdio.h>

struct threadInfo{
    int p_id;
    int arr_time;
    int burst_time;
    int completion_time;
    int turn_around_time;
    int waiting_time;
};

int main(){
    FILE *inFile;
    inFile = fopen("sample_in_schedule.txt", "r");
    int numThreads = 0;
    char c;
    while(!feof(inFile)){
        c = fgetc(inFile);
        if(c == '\n') numThreads++;
    }
    rewind(inFile);
    struct threadInfo threads[numThreads];
    int i = 0;
    while(!feof(inFile)){
        fscanf(inFile, "%d, %d, %d", &threads[i].p_id, &threads[i].arr_time, &threads[i].burst_time);
        i++;
    }
    fclose(inFile);
    int current_time = 0;
    for(int i = 0; i < numThreads; i++){
        current_time += threads[i].burst_time;
        threads[i].completion_time = current_time;
        threads[i].turn_around_time = threads[i].completion_time - threads[i].arr_time;
        threads[i].waiting_time = threads[i].turn_around_time - threads[i].burst_time;
    }
    printf("Thread ID\tArrival Time\tBurst Time\tCompletion Time\tTurn-Around Time\tWaiting Time\n");
    for(int i = 0; i < numThreads; i++){
        printf("%d\t\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", threads[i].p_id, threads[i].arr_time, threads[i].burst_time, threads[i].completion_time, threads[i].turn_around_time, threads[i].waiting_time);
    }
    float total_wait_time = 0;
    float total_turnaround_time = 0;
    for(int i = 0; i < numThreads; i++){
        total_wait_time += threads[i].waiting_time;
        total_turnaround_time += threads[i].turn_around_time;
    }
    float avg_wait_time = total_wait_time/numThreads;
    float avg_turnaround_time = total_turnaround_time/numThreads;
    printf("The average waiting time: %.2f\n", avg_wait_time);
    printf("The average turn-around time: %.2f\n", avg_turnaround_time);
    return 0;
}
