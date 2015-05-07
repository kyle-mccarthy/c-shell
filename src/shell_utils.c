#include "../include/shell_utils.h"
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


#define LIM 100

/******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************/

char* _pwd() {
    char* cwd = malloc(LIM);
    if (getcwd(cwd, LIM) != NULL) {
        return cwd;
    } else {
        return NULL;
    }
}

 /******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/

int pwd() {
    char* cwd;
    if ((cwd = _pwd()) != NULL) {
        printf("%s\n", cwd);
    } else {
        printf("%s\n", "ERROR: Could not get current directory.");
        return -1;
    }
    return 1;
}

int cd(char* path) {
    // isn't an absolute path so we need to prepend the pwd to it
    if (path[0] != '/') {
        char* tmp = _pwd();
        strcat(tmp, "/");
        strcat(tmp, path);
        path = tmp;
    }
    // try to change the directory
    if (chdir(path)) {
        printf("%s\n", "ERROR: Could not change directory");
        return -1;
    }
    return 1;
}

int ls() {
    return 0;
}

int exec() {
    return 0;
}

/******************************************************************************
 * PRIVATE FUNCTIONS
 ******************************************************************************/