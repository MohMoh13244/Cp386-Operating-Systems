#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

int MAX = 256;

int main(int argc, char *argv[]){

    if (!argv[1] || !argv[2]){
        printf("Insuffecient paremeters passed.\n");
        return -1;
    }

    struct dirent *direct;
    DIR *dest = opendir(".");
    bool stat1 = false;
    bool stat2 = false;
    while ((direct = readdir(dest))){

        if (strcmp(direct->d_name,argv[1]) == 0){
            stat1 = true;
        }
        if (strcmp(direct->d_name,argv[2])==0){
            stat2 = true;
        }
        
    }
    // input file does not exist
    if(!stat1){
        return -1;
    }
    // output file does exist
    if(stat2){
        remove(argv[2]);
    }
    creat(argv[2],S_IRWXU);
    char path[MAX];
    char finalpath[MAX];
    char finalpath2[MAX];
    char *buffer = (char *) calloc(MAX, sizeof(char));

    getcwd(path,MAX);
    snprintf(finalpath,MAX,"%s/%s", path,argv[1]);
    int input = open(finalpath,O_RDWR);
    snprintf(finalpath2,MAX,"%s/%s", path,argv[2]);
    int output = open(finalpath2,O_WRONLY);

    while (read(input, buffer,MAX )){
        write(output,buffer,MAX);
    }
    printf("The contents of file %s have been successfully copied into the %s file\n",argv[1],argv[2]);
    return 0;
}