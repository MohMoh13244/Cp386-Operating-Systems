#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

int MAX = 256;

int main(){
    
    
    char input;
    


    do{
printf("Select the option(s) appropriately by entering the number:\n \
Enter 1 for creating a directory\n \
Enter 2 for removing a directory\n \
Enter 3 for printing working directory\n \
Enter 4 for changing directory one level up\n \
Enter 5 for reading the contents of directory\n \
Enter 6 for closing the current directory\n \
Enter q to exit the program\n");
    scanf(" %c",&input);
    

    switch(input){

            // Case 1
            case '1':
            {
                char title[MAX];
                char buffer[MAX];
                char path[MAX];
                printf("Enter the directory name you want to create: \n");
                scanf(" %s",title);
                getcwd(path,MAX);
                snprintf(buffer,MAX,"%s/%s", path,title);
                int result = mkdir(buffer, S_IRWXU);
                    if (result == 0){
                        printf("Directory is Created Successfully.\n");
                    }
            }
            break;

            // Case 2
            case '2':
            {
                char title[MAX];
                char path[MAX];
                char buffer[MAX];
                printf("Enter the Directory name you want to remove: \n");
                scanf(" %s",title);
                getcwd(path,MAX);
                snprintf(buffer,MAX,"%s/%s", path,title);
                int result = rmdir(buffer);
                    if (result == 0){
                        printf("Directory is removed Successfully.\n");
                    }
            }
            break;


            // Case 3
            case '3':
            {
                printf("Current Working Directory is: %s\n",getcwd(NULL,MAX));
            }
            break;
            

            // Case 4
            case '4':
            {
                printf("Current Working Directory Before Operation: %s\n",getcwd(NULL,MAX));
                if (chdir("..") == 0){
                    printf("Directory Changed Successfully.\n");
                    printf("Working Directory After Operation: %s\n",getcwd(NULL,MAX));
                }
            }
            break;

            // Case 5
            case '5':
            {
                struct dirent *direct;
                DIR *dest = opendir(".");
                while ( (direct = readdir(dest)) != NULL){
                    printf("%s\n",direct->d_name);
                    
                    
                }
                
            }
            break;

            //Case 6
            case '6':
            {
                DIR *dest = opendir(".");
                if (closedir(dest)==0){
                    printf("Directory Closed Successfully.\n");
                }
            }
            break;
        }
        
    
    }while(input != 'q');

    
    return 0;
}