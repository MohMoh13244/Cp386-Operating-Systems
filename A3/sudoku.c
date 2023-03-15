#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
// assumptions: the file will always contain a 9x9 grid of numbers in proper format

void * checkcol();
void *checkrow();
void *checksquare(void*loc);


int puzzle[9][9];
bool columns[9] = {false,false,false,false,false,false,false,false,false};
bool rows[9] = {false,false,false,false,false,false,false,false,false};
bool squares[9] = {false,false,false,false,false,false,false,false,false};


typedef struct {
    int col;
    int row;
    int index;
}location;


int main(int argc,char *argv[]){

    // int puzzle[9][9];
    
    char line[60];
    if (argc != 2){
        printf("Error improper amount of arguments\n");
        return 1;
    }
    FILE * fp = fopen(argv[1],"r");
    // open the file
    if (fp == NULL){
        printf("Error: opening file\n");
        return 1;
    }
    // format the sudoku file into a 2D array
    int i=0,k=0;
    char *token;
    while (fgets(line,60,fp)){

        token = strtok(line," ");
        k = 0;
        while (token){
            puzzle[i][k] = atoi(token);
            token = strtok(NULL," ");
            k++;
        }
        i++;
    }

    // print the tables
    printf("Sudoku Puzzle Solution is: \n");
    for(i=0;i<9;i++){

        for(k=0;k<9;k++){

            printf("%d ",puzzle[i][k]);
        }
        printf("\n");
    }
    //create and execute threads to check columns
    //each thread will be responsible for a column
    pthread_t threads[9];

    // check all columns
    int colsindex[9] = {0,1,2,3,4,5,6,7,8};
    for(int i=0;i<9;i++){
        pthread_create(&threads[i],NULL,checkcol, (void*) &colsindex[i]);

    }

    for(int i=0;i<9;i++){
        pthread_join(threads[i],NULL);
    }
    //done checking columns

    // check all rows
    
    int rowsindex[9] = {0,1,2,3,4,5,6,7,8};
    for(int i=0;i<9;i++){
        pthread_create(&threads[i],NULL,checkrow, (void*) &rowsindex[i]);

    }

    for(int i=0;i<9;i++){
        pthread_join(threads[i],NULL);
    }

    // done checking rows

    // check all squares
    // create the parameters for the squares
    
    location params[9];
    int row = 0;
    int col = 0;
    int index = 0;
    
    while (index<9){
        (params[index]).row = row;
        (params[index]).col = col;
        (params[index]).index = index;
        col+=3;
        if (col ==9){
            row+=3;
            col = 0;
        }
        index++;
    }

    for(int i=0;i<9;i++){
        pthread_create(&threads[i],NULL,checksquare, (void*) &params[i]);
    }

    for(int i=0;i<9;i++){
        pthread_join(threads[i],NULL);
    }



    // walk through the boolean arrays
    for(int i=0;i<9;i++){
        if (columns[i] == 0){
            printf("Sudoku puzzle is invalid\n");
            return 0;
        }
    }

    for(int i=0;i<9;i++){
        if (rows[i] == 0){
            printf("Sudoku puzzle is invalid\n");
            return 0;
        }
    }

    for(int i=0;i<9;i++){
        if (squares[i] == 0){
            printf("Sudoku puzzle is invalid\n");
            return 0;
        }
    }

    printf("Sudoku puzzle is valid\n");

    return 0;
}

// runner for checking columns
void *checkcol(void * col){
    int loc = (*(int*) col);

    bool checks[9] = {false,false,false,false,false,false,false,false,false};
    for(int i =0;i<9;i++){

        if (checks[puzzle[i][loc]] == false) {
            checks[puzzle[i][loc]] = true;
        }else{
            columns[loc] = false;
            pthread_exit(0);
        }

    }
    
    columns[loc] = true;
    pthread_exit(0);
}

//runner for checking rows
void *checkrow(void *row){

    int loc = (*(int*)row);
    bool checks[9] = {false,false,false,false,false,false,false,false,false};
    for(int i = 0;i<9;i++){

        if (checks[puzzle[loc][i]] == false){
            checks[puzzle[loc][i]] == true;
        }else{
            rows[loc] = false;
            pthread_exit(0);
        }

    }

    rows[loc] = true;
    pthread_exit(0);

}

// runner for checking squares
void *checksquare(void*loc){
    int row = ((location*)loc)->row;
    int col = ((location*)loc)->col;
    int index = ((location*)loc)->index;
    bool checks[9] = {false,false,false,false,false,false,false,false,false};
    for (int i =row;i<row+3;i++){

        for(int j=col;j<col+3;j++){
            if (checks[puzzle[i][j]-1] ==  false){
                checks[puzzle[i][j]-1] = true;
            }else{
                squares[index] = false;
                pthread_exit(0);
            }
        }

    }
    squares[index] = true;
    pthread_exit(0);
}
