#include "process.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int run_process(char* const argv[]) {
    const pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return -1;
    }

    if (pid == 0) {
        execvp(argv[0], argv);
        perror("exec failed");
        exit(EXIT_FAILURE);
    }
    int status;
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        return -1;
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    return -1;
}

pid_t run_process_async(char* const argv[]) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return -1;
    }

    if (pid == 0) {
        execvp(argv[0], argv);
        perror("exec failed");
        exit(EXIT_FAILURE);
    }

    return pid;
}

char* run_command_capture(const char *command) {
    FILE *fp = popen(command, "r");
    if (!fp) {
        perror("popen failed");
        return NULL;
    }

    char *result = NULL;
    size_t size = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), fp)) {
        const size_t len = strlen(buffer);
        char *tmp = realloc(result, size + len + 1);
        if (!tmp) {
            free(result);
            pclose(fp);
            return NULL;
        }
        result = tmp;
        memcpy(result + size, buffer, len);
        size += len;
        result[size] = '\0';
    }

    pclose(fp);
    return result;
}