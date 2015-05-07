#include "shell_utils.h"
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define LIM 100

int pwd() {
    char cwd[LIM];
    if (getcwd(cwd, LIM) != NULL) {
        printf("%s\n", cwd);
    } else {
        printf("%s\n", "ERROR: Could not get current directory.");
        return 0;
    }
    return 1;
}

int cd() {
    return 0;
}

int ls() {
    return 0;
}

int exec() {
    return 0;
}