#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "ls_command.h"

#define MAX_LINE 80
#define MAX_ARGS 10

int main() {
    char input[MAX_LINE];
    char *token;
    char *argv[MAX_ARGS];

    while (1) {
        printf("myshell> ");
        fgets(input, MAX_LINE, stdin);

        token = strtok(input, " \n\t");
        int i = 0;
        while (token != NULL) {
            argv[i++] = token;
            token = strtok(NULL, " \n\t");
        }

        if (argv[0] == NULL) {
            continue; // 빈 명령어 처리
        }
        argv[i] = NULL;

        if (strcmp(argv[0], "exit") == 0) {
            printf("Goodbye!\n");
            exit(0);
        }
        else if (strcmp(argv[0], "cd") == 0) {
            if (chdir(argv[1]) != 0) {
                perror("cd failed");
            }
        }
        else if (strcmp(argv[0], "pwd") == 0) {
            getcwd(input, MAX_LINE);
            printf("%s\n", input);
        }
        else if (strcmp(argv[0], "ls") == 0) {
            my_ls();
        }
        else if (strcmp(argv[0], "cat") == 0) {
            // 'cat' 명령어 처리 (생략)
        }
        else {
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork failed");
            }
            else if (pid == 0) {
                execvp(argv[0], argv);
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
            else {
                int status;
                waitpid(pid, &status, 0);
            }
        }
    }

    return 0;
}
