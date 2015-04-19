#ifndef _SHELL_UTILS_H_
#define _SHELL_UTILS_H_

#include <sys/types.h>
/**
 * Get the path to the current working directory.  
 * @return int > 0 on success int < 0 on failure
 */
int pwd();

/**
 * Try to change to CWD to the path passed as a paremeter.  This path does not need to
 * absolute and will be validated accordingly inside the function.
 * @param path - the path to cd to
 * @return int > 0 on success int < 0 on failure
 */ 
 int cd(char* path);

 /**
 *  
 *
 @return
 */
 int grep(char* searchString, char* fileName);

/**
 *
 * @return
 */
int ls();

 /**
  *
  * @return
  */
char** tokenize(char* buf);

 /**
  *
  * @return
  */

int exec(char* cmd, char** argv);

/**
 *  runCommand
 *
 @return
 */

 int runCommand(char*op, int argc, char* argv[]);

/**
 *  runCommandWithOutRedirect
 *
 @return
 */
 int runCommandWithOutputRedirect(char* op, int argc, char* argv[], char* fileName);

/**
 *  runCommandsWithPipe
 *
 @return
 */
 int runCommandWithPipe(char* op1, int argc1, char* argv1[], char* op2, int argc2, char* argv2[]);

/**
 *  executeOp
 *  
 @return
 */
 int executeOp(char* op, int argc, char* argv[]);

 /**
 *  
 *
 @return
 */
int waitForChild(pid_t pid);

 /**
 *  
 *
 @return
 */
 int validOp(char* op);
#endif