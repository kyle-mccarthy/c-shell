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

/**
 * Try to get the path of the current working directory.  If we are able to get the path
 * return it, otherwise return a NULL value.
 * @return char* - path to current working directory or NULL
 */
char* _pwd();

/**
 * If the path parameter is not currently an absolute path to a file.  Modify it such that
 * it will include the full system path.
 * @return void 
 */
void _abs_path(char** path);

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
    _abs_path(&path);
    // try to change the directory
    if (chdir(path)) {
        printf("%s\n", "ERROR: Could not change directory");
        return -1;
    }
    return 1;
}

int ls(char* path) {
    // get the full path
    _abs_path(&path);

    // directory entries
    DIR* dir_path;
    struct dirent* dir_ent;

    // get the dir
    dir_path = opendir(path);
    if (dir_path) {
        while ((dir_ent = readdir(dir_path)) != NULL) {
            printf("%s\n", dir_ent->d_name);
        }
        closedir(dir_path);
    }

    return 1;
}

int exec() {
    return 0;
}

/******************************************************************************
 * PRIVATE FUNCTIONS
 ******************************************************************************/

 char* _pwd() {
    char* cwd = malloc(LIM);
    if (getcwd(cwd, LIM) != NULL) {
        return cwd;
    } else {
        return NULL;
    }
}

void _abs_path(char** path) {
    if ((*path) == NULL || (*path)[0] != '/') {
        char* tmp = _pwd();
        if (tmp == NULL) {
            printf("%s\n", "ERROR: Can not get absolute path");
            return;
        }
        strcat(tmp, "/");
        if ((*path) == NULL) {
            (*path) = malloc(strlen(tmp) + 1);
        }
        strcat(tmp, (*path));
        (*path) = tmp;
    }
}