#ifndef _SHELL_UTILS_H_
#define _SHELL_UTILS_H_

/**
 *
 * @return 
 */
int pwd();

/**
 *
 * @return
 */ 
 int cd();

/**
 *
 * @return
 */
int ls();

 /**
  *
  * @return
  */

int exec();

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
 int runCommandsWithPipe(char* op1, int argc1, char* argv1[], char* op2, int argc2, char* argv2[]);

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
#endif