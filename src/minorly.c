#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <setjmp.h>
#include <linux/limits.h> // Used for PATH_MAX

sig_atomic_t SIGINT_FLAG = 0;
jmp_buf env;

static void handle_SIGINT(int signum) {
        (void)signum;
        longjmp(env, 1);
        SIGINT_FLAG = 1;
}

int main (void) {
        // Registers sigation for SIGINT
        struct sigaction sigint_action;
        sigset_t sigint_set;
        sigemptyset(&sigint_set);
        sigaddset(&sigint_set, SIGCHLD);
        sigint_action.sa_flags = 0;
        sigint_action.sa_handler = handle_SIGINT;
        sigint_action.sa_mask = sigint_set;
        sigaction(SIGINT, &sigint_action, NULL);

        // Registers sigaction for SIGCHLD
        
        char *input;
        for (;;) {
                if (setjmp(env)) {
                        printf("Inside jump\n");
                        goto CLEAN;
                }
                printf("Enter your command\n");
                input = calloc(PATH_MAX, sizeof(char));
                if (!fgets(input, PATH_MAX, stdin)) {
                        perror("fgets");
                        errno = 0;
                }
                free(input);
        }
CLEAN:
        free(input);
EXIT:
        return 0;
}