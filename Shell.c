#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_LINE 80
#define HIST_SIZE 10

char *history[HIST_SIZE];
int hist_count = 0;


void add_hist(char *command) {
    if (hist_count >= HIST_SIZE) {
        for (int i = 1; i < HIST_SIZE; i++) {
            strcpy(history[i - 1], history[i]);
        }
        hist_count = HIST_SIZE - 1;
    }
    strcpy(history[hist_count++], command);

}

void show_hist() {
    for (int i = 0; i < hist_count; i++) {
        printf("%d %s"
               "\n", i + 1, history[i]);
    }
}

int main() {
    char input[MAX_LINE], *token, *args[MAX_LINE / 2 + 1];
    int should_run = 1;
    pid_t pid;
    int background, invalid;

    for (int i = 0; i < HIST_SIZE; i++) {
        history[i] = (char *)malloc(MAX_LINE);
    }

    while (should_run) {
        printf("Shell>");
        fflush(stdout);

        fgets(input, MAX_LINE, stdin);
        input[strlen(input) - 1] = '\0';

        token = strtok(input, " ");
        int i = 0;
        background = invalid = 0;

        while (token != NULL) {
            args[i] = token;
            if (strcmp(token, "&") == 0) {
                background = 1;
                args[i] = NULL;
            }
            token = strtok(NULL, " ");
            i++;
        }
        args[i] = NULL;

        if (i == 0) {
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            should_run = 0;
        } else if (strcmp(args[0], "history") == 0) {
            show_hist();
        } else if (strcmp(args[0], "!!") == 0) {
            if (hist_count == 0) {
                printf("No command to show.\n");
                invalid = 1;
            } else {
                strcpy(input, history[hist_count - 1]);
                token = strtok(input, " ");
                i = 0;
                background = invalid = 0;
                while (token != NULL) {
                    args[i] = token;
                    if (strcmp(token, "&") == 0) {
                        background = 1;
                        args[i] = NULL;
                    }
                    token = strtok(NULL, " ");
                    i++;
                }
                args[i] = NULL;
            }
        } else if (args[0][0] == '!') {
            int index = atoi(args[0] + 1);
            if (index > 0 && index <= hist_count) {
                args[0] = history[index - 1];
            } else {
                printf("Invalid index.\n");
                invalid = 1;
            }
        }

        if (!invalid) {
            add_hist(args[0]);
            pid = fork();
            if (pid == 0) {
                if (execvp(args[0], args) < 0) {
                    printf("Invalid command.\n");
		    exit(1); 
               }
                return 0;
            } else {
                if (!background) {
                    wait(NULL);
                }
            }
        }
    }
            for (int i = 0; i < HIST_SIZE; i++) {
                free(history[i]);
            }
    return 0;
}

