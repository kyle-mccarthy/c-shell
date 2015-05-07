#ifndef _SHELL_UTILS_H_
#define _SHELL_UTILS_H_

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
 * @return
 */
int ls();

 /**
  *
  * @return
  */

int exec();





#endif