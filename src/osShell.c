/**
osShell - parses user input and executes commands

Build:    gcc read_token_to_array.c -o osShell -lreadline   
*/
#include "../include/shell_utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>

int main(int argc, char * argv[])
{
   char * myArgv[101]; // Assume limit of 100 tokens, extra pointer on end for null terminatng argv
   myArgv[100] = NULL;
   char * cmdBuffer = NULL;
   char * heapBuffer = NULL;

   char indirectionOp;
   int indirectionOpIndx, errCode;

   // readline  will  read a line from the terminal and return it, using prompt as a prompt.  If prompt is NULL or the empty string, no
   // prompt is issued.  The line returned is allocated with malloc(3); the caller must free it when finished.  The line  returned  has
   // the final newline removed, so only the text of the line remains.
   cmdBuffer = readline("osShell >$ ");

   // WHILE NOT EXIT
   while ( 0 != strncmp("EXIT", cmdBuffer, 4))
   {
      // Our Token Parser
      unsigned char currentTokenIndex = 0;
      //does stdin or stdout need to be rerouted
      indirectionOp = '\0';

      // Get some heap to copy into so it is scoped proper for passes and forks
      heapBuffer = (char *) calloc(strlen(cmdBuffer), sizeof(char));
      strncpy(heapBuffer,cmdBuffer, strlen(cmdBuffer) * sizeof(char));

      /* get the first token */
      myArgv[ currentTokenIndex ] = strtok(heapBuffer, " ");

      /* walk through other tokens */
      while( myArgv[ currentTokenIndex ] != NULL && currentTokenIndex < 98) 
      {

         //printf( "%d] %s\n", currentTokenIndex, myArgv[ currentTokenIndex ] );

         //determine if one of the indirection operators is used 
         if (strncmp(myArgv[currentTokenIndex], "|", 2) == 0) {
            indirectionOp = '|';
            indirectionOpIndx = currentTokenIndex;
         }
         else if (strncmp(myArgv[currentTokenIndex], ">", 2) == 0) {
            indirectionOp = '>';
            indirectionOpIndx = currentTokenIndex;
         }

         currentTokenIndex ++;

         myArgv[ currentTokenIndex ] = strtok(NULL, " ");
      }

      myArgv[currentTokenIndex + 1] = NULL; // Null Term Arg Array

      //printf("Number of Tokens: %d \n ", currentTokenIndex);

      if (!validOp(myArgv[0])){
         fputs("osShell >$Invalid Operation\n", stderr);
      }
      else{

         // Now do something with an array like exec
         if (indirectionOp != '\0') {
            if (indirectionOp == '|' && !validOp(myArgv[indirectionOpIndx + 1])) {
               printf("osShell >$Invalid Operation");
               continue; 			
            }
            if (indirectionOp == '|'){
               errCode = runCommandWithPipe(myArgv[0], 
                  indirectionOpIndx -1, myArgv + 1, myArgv[indirectionOpIndx + 1],
                   currentTokenIndex - indirectionOpIndx, myArgv + indirectionOpIndx + 2);
            }
            else if (indirectionOp  == '>'){
               errCode = runCommandWithOutputRedirect(myArgv[0], 
                  indirectionOpIndx - 1, myArgv + 1, myArgv[indirectionOpIndx + 1]);
            }
         }
         else{
            if (strncmp(myArgv[0], "cd", 3) == 0){
               errCode = cd(myArgv[1]);
            }
            else{
               errCode = runCommand(myArgv[0], currentTokenIndex - 1,  myArgv + 1);
            }
         }
      }

      free( heapBuffer );
      free( cmdBuffer );
      // Read the next command
      cmdBuffer = readline("osShell >$ ");
   }

   free( cmdBuffer );

   return(0);
}