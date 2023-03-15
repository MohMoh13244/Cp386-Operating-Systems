#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>

#define MAX 1024
#define READ_END 0
#define WRITE_END 1
#define MAX_COMMANDS 1000
#define MAX_LENGTH 100



void writeOuput(char *command, char *output);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error: File name not provided\n");
        return 1;
    }

    // Read the contents of the file into a shared memory area
    int fd = shm_open("/shared_memory", O_CREAT | O_RDWR, 0666);
    ftruncate(fd, MAX_COMMANDS * MAX_LENGTH);
    char *ptr = mmap(NULL, MAX_COMMANDS * MAX_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0);

    int pid = fork();
    if (pid == 0) {
        // Child process reads the contents of the input file
        FILE *fp = fopen(argv[1], "r");
        if (fp == NULL) {
            printf("Error opening file\n");
            return 1;
        }


        // Write the contents of the file to shared memory
        char buffer[1000];
        int n = 0;
        while (fgets(buffer, MAX_LENGTH, fp) != NULL) {
            strcpy(ptr + n, buffer);
            n += strlen(buffer);
            
        }
        fclose(fp);
        exit(0);
        } else {
            // wait for child to finish reading to shared memory
            while(wait(&pid)>0);
        }
        char *content = (char *)mmap(0, 1000, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

        char *token = strtok(content, "\n");
        while (token != NULL) {
        // Create a pipe for communication between parent and child processes
        int fd[2];
        pipe(fd);

                pid = fork();
        if (pid == 0) {
            // Child process executing the command
            close(fd[READ_END]);
            dup2(fd[WRITE_END], 1);
            char *args[MAX];
            int j = 0;
            args[j] = strtok(token, " ");
            while (args[j] != NULL) {
                j++;
                args[j] = strtok(NULL, " ");
            }
            execvp(args[0], args);
            exit(0);
        } else {
            // Parent process reading the output from the pipe
            close(fd[WRITE_END]);
            char output[MAX];
            int n = read(fd[READ_END], output, MAX);
            output[n] = '\0';
            writeOuput(token, output);
            close(fd[READ_END]);
        }
        token = strtok(NULL, "\n");
    }

    return 0;
}


void writeOuput(char* command, char* output)
{
    FILE* fp = fopen("output.txt","a");
	fprintf(fp, "The output of: %s : is\n", command);
	fprintf(fp, ">>>>>>>>>>>>>>>\n%s<<<<<<<<<<<<<<<\n", output);	       
    fclose(fp);
    return;
}