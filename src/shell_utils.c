#include "../include/shell_utils.h"
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
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
        printf("%s\n", "ERROR: Could not get current directory");
        return -1;
    }
    return 1;
}

int cd(char* path) {
    _abs_path(&path);
    // try to change the directory
    if (chdir(path)) {
        printf("%s %s\n", "ERROR: Could not change directory to", path);
        return -1;
    }
    return 1;
}

int ls(char* path) {
    // get the full path
    _abs_path(&path);

    if (path == NULL) {
        printf("%s\n", "ERROR: Could not list directory");
        return -1;
    }
    
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
    } else {
        perror(strerror(errno));
        printf("%s\n", "ERROR: Could not list directory");
    }

    return 1;
}

int exec() {
    return 0;
}

int validOp(char* op){
    return 1;
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
    }
    else{
        //executable
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
    int outFd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC);

    if (outFd < 0){
        return -1;
    }  

    pid_t pid = fork();

    if (pid < 0){
        return -1;
    }   

    if (pid == 0){
        //child

        //replace stdout with output file descriptor
        if (dup2(outFd, 1)){
            return -1;
        }

        //execute command
        executeOp(op, argc, argv);
        exit(1);
    } 
    else{
        //don't do anything 
    }

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
            if (dup2(fds[0], 0) == -1){
                printf("%s\n", "dup failure");  
            }

            executeOp(op2, argc2, argv2);
            exit(1);
        }
        else{
            if (dup2(fds[1], 1) == -1){
                printf("%s\n", "dup failure");  
            }

            executeOp(op1, argc2, argv2);
            waitForChild(pid2);
            exit(1);
        }
    }
    else{
        //parent do nothing
    }

    waitForChild(pid);
    return 0;
}

int waitForChild(pid_t pid){
    int status; // catch the status of the child

    do  // in reality, mulptiple signals or exit status could come from the child
    {

        pid_t w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
        if (w == -1)
        {
           // std::cerr << "Error waiting for child process ("<< pid <<")" << std::endl;
            break;
        }
        
        if (WIFEXITED(status))
        {
            if (status > 0)
            {
               // std::cerr << "Child process ("<< pid <<") exited with non-zero status of " << WEXITSTATUS(status) << std::endl;
                continue;
            }
            else
            {
               // std::cout << "Child process ("<< pid <<") exited with status of " << WEXITSTATUS(status) << std::endl;
                continue;
            }
        }
        else if (WIFSIGNALED(status))
        {
            //std::cout << "Child process ("<< pid <<") killed by signal (" << WTERMSIG(status) << ")" << std::endl;
            continue;           
        }
        else if (WIFSTOPPED(status))
        {
            //std::cout << "Child process ("<< pid <<") stopped by signal (" << WSTOPSIG(status) << ")" << std::endl;
            continue;           
        }
        else if (WIFCONTINUED(status))
        {
            //std::cout << "Child process ("<< pid <<") continued" << std::endl;
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
    if ((*path) == NULL || (*path)[0] != '/') {
        char* tmp = _pwd();
        if (tmp == NULL) {
            printf("%s\n", "ERROR: Can not get absolute path");
            return;
        }
        strcat(tmp, "/");
        if ((*path) == NULL) {
            (*path) = malloc(strlen(tmp) + 1);
        } else {
            strcat(tmp, (*path));
        }
        (*path) = tmp;
    }
}

