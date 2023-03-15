#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/wait.h>

# define MAX 256



int calc_print_average(int num,int total_grades,int* grades);
// Each chapter will always have two assignments
// Dynamically allocate the TA process for each chapter
int main(int argc,char *argv[]){

    char filename[MAX];
    strcpy(filename,argv[1]);
    FILE* fd = fopen(filename,"r");
    if (fd == NULL){
        printf("Error: opening file\n");
        return 1;
    }
    int rows=0,cols=0;

    char line[MAX];
    fgets(line,MAX,fd);
    rows++;
    char *grade;
    grade = strtok(line," ");
    while(grade){
        cols++;  
        grade = strtok(NULL," ");

    }
    while(fgets(line,MAX,fd)){
        rows++;
    }

    int array[rows][cols];
    rewind(fd);

    int i=0,j=0;

    while(fgets(line,MAX,fd)){


        grade = strtok(line," ");
        i=0;

        while(grade){

            array[i][j] =  atoi(grade);

            grade = strtok(NULL," ");
            i++;
          
        }
        j++;

    }


    fclose(fd);
    int num = 1;

    // Create GradTAs
    for(i=0;i<cols/2;i++){

        int GTA = fork();
        if (GTA < 0){
            printf("Error: cant fork()\n");
            return 1;
        }

        //inside the GradTA child
        if(GTA == 0){

            int TA = fork();
            if (TA < 0){
                printf("Error: cant fork()\n");
                return 1;
            }

            for (j=0;j<2;j++){
                // inside the TA child
                if(TA == 0){
                    calc_print_average(num,rows,array[num-1]);
                    exit(0);
                }
                while(wait(&TA)>0);
                num++;
            }

            exit(0);
        }

        num+=2;
        while(wait(&GTA)>0);

    }
    
    return 0;   
}

int calc_print_average(int num,int total_grades,int* grades){

    
    float sum=0;
    for (int i =0;i<total_grades;i++){
        sum+=grades[i];
    }
    printf("Average for assignment %d: %f\n",num,sum/total_grades);
    return 0;
}