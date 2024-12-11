#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ls_command.h"

#define MAX_LINE 80  // 입력 가능한 최대 라인 길이
#define MAX_ARGS 10  // 명령어의 최대 인자 개수

void my_cat(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);
}

void execute_program(char *program, char *args[]) {
    pid_t pid = fork();
    if (pid == 0) { // 자식 프로세스
        if (execvp(program, args) == -1) {
            perror("execvp");
        }
        exit(EXIT_FAILURE);
    } else if (pid > 0) { // 부모 프로세스
        int status;
        waitpid(pid, &status, 0); // 자식 프로세스 종료 대기
    } else {
        perror("fork");
    }
}

int main() {
    char input[MAX_LINE];
    char *token;
    char *argv[MAX_ARGS];

    while (1) {
        // 프롬프트 출력
        printf("myshell> ");

        // 명령어 입력
        if (fgets(input, MAX_LINE, stdin) == NULL) {
            break; // EOF 입력 시 종료
        }

        // 입력 토큰화
        token = strtok(input, " \n\t");
        int i = 0;
        while (token != NULL) {
            argv[i++] = token;
            token = strtok(NULL, " \n\t");
        }

        if (argv[0] == NULL) { // 빈 명령어 처리
            continue;
        }
        argv[i] = NULL;

        // exit 명령어
        if (strcmp(argv[0], "exit") == 0) {
            printf("Goodbye!\n");
            exit(0);
        }

        // cd 명령어
        else if (strcmp(argv[0], "cd") == 0) {
            if (argv[1] == NULL || chdir(argv[1]) != 0) {
                perror("chdir");
            }
        }

        // pwd 명령어
        else if (strcmp(argv[0], "pwd") == 0) {
            if (getcwd(input, MAX_LINE) != NULL) {
                printf("%s\n", input);
            } else {
                perror("getcwd");
            }
        }

        // ls 명령어
        else if (strcmp(argv[0], "ls") == 0) {
            my_ls();
        }

        // cat 명령어
        else if (strcmp(argv[0], "cat") == 0) {
            if (argv[1] == NULL) {
                printf("Usage: cat <filename>\n");
            } else {
                my_cat(argv[1]);
            }
        }

        // 외부 프로그램 실행
        else {
            if (access(argv[0], X_OK) == 0) {
                execute_program(argv[0], argv);
            } else {
                printf("command not found: %s\n", argv[0]);
            }
        }
    }

    return 0;
}
