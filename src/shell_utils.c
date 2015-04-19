#include "../include/shell_utils.h"
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <features.h>

#define LIM 100
#define _GNU_SOURCE

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
            printf("%-40s", dir_ent->d_name);
        }
        printf("\n");
        closedir(dir_path);
    }

    return 0;
}

int grep(char* searchString, char* fileName){
    if (searchString == NULL){
        printf("%s\n", "invalid arguments");
        return -1;
    }
    else if (fileName != NULL){
        //open file and dup it into stdin so 
    }

    char buffer[500];
    int fd = open(fileName, O_RDONLY, 0);
    if (fd < 0){
        printf("%s\n", "error opening file");
    }

    while (read(fd, buffer, 500) > 0){
        //tokenize(buffer);
    }

}

int exec(char* cmd, char** argv) {
    //check that the possible executable exists
    if (cmd == NULL || argv == NULL){
        printf("%s\n", "invalid arguments");
        return -1;
    }
    else if (access(cmd, F_OK) == -1){
        printf("%s\n", "file doesn't exist");
        return -1;
    }


    if (execv(cmd, argv) == -1){
        printf("%s\n", "cannot be executed");
        return -1;
    }
    else{
        return 0;
    }
}

int cat(char** args) {
    // write to new file
    if ( args[0] == NULL || strcmp(args[0], ">") == 0) {
        char* data = NULL;
        int length;

        while (1) {
            getline(&data, &length, stdin);
            if (strncmp(data, "^D", 2) == 0){
                break;
            }
            printf("%s\n", data);
        }

        free(data);
    // read from file already created
    } else {
        int fd = open(args[0], O_RDONLY);
        if (fd < 0) {
            printf("%s\n", "ERROR: Could not open file");
            return -1;
        }
        char* buffer = malloc(512);
        while (read(fd, buffer, 512) > 0) {
            printf("%s", buffer);
        }
    }
    return 0;
}

int validOp(char* op){
    if (op == NULL){
        return 0;
    }
    else if (strcmp(op, "cd") == 0){
        return 1;
    }
    else if (strcmp(op, "ls") == 0){
        return 1;
    }
    else if (strcmp(op, "pwd") == 0){
        return 1;
    }
    else if (strcmp(op, "exec") == 0){
        return 1;
    } 
    else if (strcmp(op, "cat") == 0) {
        return 1;
    }
    return 0;
}

int executeOp(char* op, int argc, char* argv[]){
    //op already validated  
    if (strcmp(op, "pwd") == 0){
        return pwd();
    }
    else if(strcmp(op, "cd") == 0){
        return cd(argv[0]);
    }
    else if (strcmp(op, "ls") == 0){
        if (argc < 2){
            return ls(NULL);
        }
        else{
            return ls(argv[0]);
        }
    } else if (strcmp(op, "exec") == 0) {
        exec(argv[0], argv + 1);
    } else if (strcmp(op, "cat") == 0) {
        cat(&argv[0]);
    }
    return 0;
}

int runCommand(char* op, int argc, char* argv[]){
    pid_t pid = fork();

    if (pid < 0){
        printf("%s\n", "fork failure");
        return -1;
    }

    if (pid == 0){
        executeOp(op, argc, argv);
        exit(1);
    }
    else{

    }

    waitForChild(pid);
    return 0;
}

int runCommandWithOutputRedirect(char* op, int argc, char* argv[], char* fileName){
    //op has already been validated

    //open file


    pid_t pid = fork();

    if (pid < 0){
        return -1;
    }   

    if (pid == 0){
        //child
        int outFd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (outFd < 0){
            return -1;
        }  

        //replace stdout with output file descriptor
        if (dup2(outFd, STDOUT_FILENO) == -1){
            exit(-1);
        }

        close(outFd);

        //execute command
        executeOp(op, argc, argv);
        exit(1);
    } // else parent do nothing

    waitForChild(pid);
    return 1;
}

int runCommandWithPipe(char* op1, int argc1, char* argv1[], char* op2, int argc2, char* argv2[]){
    //both ops already validated

    int fds[2];

    if (pipe(fds) < 0){
        printf("%s\n", "pipe creation failure");
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0){
        printf("%s\n", "fork failure");
        return -1;
    }

    if (pid == 0){
        pid_t pid2;

        pid2 = fork();

        if (pid < 0){
            printf("%s\n", "fork failure");
            return -1;
        }

        if (pid2 == 0){
            //replace stdin with reading end of pipe
            if (dup2(fds[0], STDIN_FILENO) == -1){
                printf("%s\n", "dup failure");  
            }

            executeOp(op2, argc2, argv2);
            exit(1);
        }
        else{
            //replace stdout with writing end of pipe
            if (dup2(fds[1], STDOUT_FILENO) == -1){
                printf("%s\n", "dup failure");  
            }

            executeOp(op1, argc2, argv2);
            printf("^D\n");
            waitForChild(pid2);
            exit(1);
        }
    } // else parent do nothing

    waitForChild(pid);
    return 0;
}

int waitForChild(pid_t pid){
    int status; // catch the status of the child

    do  // in reality, mulptiple signals or exit status could come from the child
    {
        pid_t w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
        if (w == -1) {
            break;
        }
        
        if (WIFEXITED(status) || WIFSIGNALED(status) || WIFSTOPPED(status) || 
            WIFCONTINUED(status)) {
            continue;
        }
    }
    while (!WIFEXITED(status) && !WIFSIGNALED(status));

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
    // if the str is NULL or doesn't start with / then it isn't the abs path
    if ((*path) == NULL || (*path)[0] != '/') {
        char* tmp = _pwd();
        // make sure that nothing got messed up in the _pwd call
        if (tmp == NULL) {
            printf("%s\n", "ERROR: NULL pwd passed to _abs_path");
            return;
        }
        // preprend the the abs path to the realative path
        strcat(tmp, "/");
        if ((*path) == NULL) {
            (*path) = malloc(strlen(tmp) + 1);
        } else {
            strcat(tmp, (*path));
        }
        (*path) = tmp;
    }
}

