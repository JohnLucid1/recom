#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <Windows.h>

#define KNRM  "\x1B[0m"
#define KGRN  "\x1B[32m"
#define KBLU  "\x1B[34m"


char* read_entire_file(const char* filename)
{
    FILE* file = fopen(filename, "rb"); 
    if (file == NULL)
    {
        perror("Error opening file");
        return NULL;
    }


    if (fseek(file, 0, SEEK_END) != 0)
    {
        perror("Error seeking in file");
        fclose(file);
        return NULL;
    }


    long fileSize = ftell(file);
    if (fileSize == -1)
    {
        perror("Error telling file size");
        fclose(file);
        return NULL;
    }


    rewind(file);


    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL)
    {
        perror("Error allocating buffer");
        fclose(file);
        return NULL;
    }


    size_t bytesRead = fread(buffer, 1, fileSize, file);
    if (ferror(file))
    {
        perror("Error reading file");
        fclose(file);
        free(buffer);
        return NULL;
    }


    buffer[bytesRead] = '\0';


    fclose(file);

    return buffer;
}

char* substring_after(const char *str, const char *delim ){
    const char *pos = strstr(str, delim);
    if (!pos) {
        return NULL;
    }
    
    
    pos += strlen(delim);

    char *result = malloc(strlen(pos) + 1);
    if (!result) {
        return NULL;
    }
    
    strcpy(result, pos);
    return result;
}


int recommand(const char *cmd){
    FILE *fp;
    
    char buff[2024];
    
    fp = popen(cmd, "r");
    if (fp == NULL) {
        return 1;
    }

    while (fgets(buff, sizeof(buff), fp) != NULL) {
        printf("%s", buff);
    }
    
    int status = pclose(fp);
    if (status == -1) {
        return 1;
    }
    return 0;
}

int main (int argc, char **argv){
    if (argc < 3) {
        printf("ERROR: not enough arguments.\nNeed at least 2.");
        exit(1);
    }
    

    if (argv[2][2] =='f'){
        char * path = substring_after(argv[2], "=");

        if (!path) {
            printf("ERROR");
            exit(1);
        }

        printf("%sStarted watching file: %s\n", KGRN, path);
        char * content = read_entire_file(path);
        if (!content) {
            printf("ERROR: no file :(");
            exit(1);
        }
        
        for(;;) {
            char* new_content = read_entire_file(path);
            if (!content) {
                printf("ERROR: no file :(");
                exit(1);
            }

            if (strcmp(content, new_content) != 0) {
                printf("\033[2J");
                printf("\033[H");
                printf("%sOUTPUT OF %s\n", KBLU, argv[1]);
                printf("%s", KNRM);

                int res = recommand(argv[1]);
                if (res == 1) {
                    printf("ERROR: executing a command");
                    exit(1);

                }

                content = new_content;
            }

            Sleep(1000);
        }
        
        free(content);
        free(path);
    }else if(argv[2][2] == 'd') {
        printf("unimplemented\n");
        exit(1);
    }
    
    
    return 0;
}