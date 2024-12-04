#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 80 // 터미널 입력 라인이 보통 80byte?
#define MAX_AEGS 10
int main()
{
    char input[MAX_LINE];
    char *token;
    char *argv[MAX_AEGS];
    while (1){
        //prompt
        printf("myshell> ");

        //read command
        fgets(input, MAX_LINE, stdin);
        printf("%s\n", input);

        //tokenize input
        token = strtok(input, "\n\t");
        int i = 0;
        while (token != NULL){
            argv[i++] = token;
            token = strtok(NULL, "\n\t");
        }
        argv[i] = NULL;

        if(argv[0] == NULL){ //empty command
            continue;
        }

        if(strcmp(argv[0], "exit") == 0){
            printf("Goodbye!\n");
            exit(0);
        }
    }

    return 0;
}