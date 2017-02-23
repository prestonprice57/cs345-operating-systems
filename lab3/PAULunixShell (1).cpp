/***********************************************************************
 * Program:
 *    Lab UnixShell
 *    Brother Jones, CS 345
 * Author:
 *    Paul Mason
 * Summary:
 *    Implement unix shell.
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <signal.h>
#include <wait.h>
#include <sstream>
#include <cstdlib>

#define MAX_LINE 81
#define HIST_SIZE 10
#define MAX_NUM_CMDS 1000

using namespace std;

string commands[HIST_SIZE];
int goodStringCount = 0;


//function find command in history
bool findCommandInHistory(char c, string& stringFound)
{
   int temp = goodStringCount;
   stringFound = "";

   // If the character being looked for is a null, then return the most
   // recent command, if there was one.
   if (c == '\0')
   {
      if (goodStringCount)
      {
         stringFound = commands[ (goodStringCount - 1) % HIST_SIZE ];
         return true;
      }
      else
      {
         return false;
      }
   }

   // Look back through the history for a command staring with the letter
   // given in the first argument to this function.
   for (temp; temp >= 0 && temp != temp - HIST_SIZE; --temp)
   {
      if (commands[temp % HIST_SIZE][0] == c)
      {
         stringFound = commands[temp % HIST_SIZE];
         return true;
      }
   }

   return false;
}



//function to display the history of commands
void displayHistory( )
{
   int start;

   if (goodStringCount > HIST_SIZE)
      start = goodStringCount - HIST_SIZE;
   else
   start = 0;

   for (int i = start; i < goodStringCount; ++i)
   {
      cout << " " << setw(3) << i << "  " << commands[i % HIST_SIZE] << endl;
   }

   return;
}

//Function add2history
void add2History (string input)
{
   commands[goodStringCount % HIST_SIZE] = input;
   goodStringCount++;

   // Roll over the count if it hits the maximum allowed for printing
   if ( goodStringCount >= MAX_NUM_CMDS )
      goodStringCount = HIST_SIZE;    /* Leave a full buffer of commands */
}



//Fuction to get the command from shell, tokenize it and set the args parameter

/*int formatCommand(char inputBuffer[], char *args[],int *flag)
{
   int length; // # of chars in command line
   int i;     // loop index for inputBuffer
   int start;  // index of beginning of next command
   int ct = 0; // index of where to place the next parameter into args[]
   int hist;

   bool goodCMD = true;
   string tmpString;
   //read user input on command line and checking whether the command is !! or !n

   length = read(STDIN_FILENO, inputBuffer, MAX_LINE);


   start = -1;
   if (length == 0)
      exit(0);   //end of command
   if (length < 0)
   {
      printf("Command not read\n");
      exit(-1);  //terminate
   }

   //examine each character
   for (i=0;i<length;i++)
   {
      switch (inputBuffer[i])
      {
         //case ' ':
         case '\t' :               // to seperate arguments
            if(start != -1)
            {
               args[ct] = &inputBuffer[start];
               ct++;
            }
            inputBuffer[i] = '\0'; // add a null char at the end
            start = -1;
            break;

         case '\n':                 //final char
            if (start != -1)
            {
               args[ct] = &inputBuffer[start];
               ct++;
            }
            inputBuffer[i] = '\0';
            args[ct] = NULL; // no more args
            break;

         default :
            if (start == -1)
               start = i;
            if (inputBuffer[i] == '&')
            {
               *flag  = 1; //this flag is the differentiate whether the child process is invoked in background
               inputBuffer[i] = '\0';
            }
      }
   }

   args[ct] = NULL; //if the input line was > 80

   //look for display history
   if (inputBuffer[0] == '.' && inputBuffer[1] == '\0')
   {
      cout << "displaying history ....\n";

      displayHistory();
      goodCMD = false;
   }

   //perform history command
   else if(inputBuffer[0] == 'r' &&
           (inputBuffer [1] == ' ' || inputBuffer[1] == '0'))
   {
      cout << "IN THE RIGHT PLACE\n";
      if(inputBuffer[1] == '\0')
         inputBuffer[2] = '\0';

      goodCMD = findCommandInHistory(inputBuffer[2], tmpString);

      if(goodCMD)
      {
         strncpy (inputBuffer, tmpString.c_str(), MAX_LINE);
      }
      else
      {
         cout << "Command not found\n";
         cout.flush();
      }
      
   }

   //looking for exit command
   else if(length == 0 || ( ! strncmp(inputBuffer, "exit", MAX_LINE)))
   {
      exit(0);
   }
   string temp;
   /*for (i = 9;i>0; i--) //Moving the history elements one step higher
   {
      temp = commands[i];
      
      commands[i] = commands[i-1];
      
      commands[i + 1] = temp;
      
   }

   //Updating the history array with input buffer
   commands[0] = inputBuffer;

   commands[goodStringCount] = inputBuffer;
   goodStringCount++;

}
*/
int main(void)
{
   
   //char inputBuffer[MAX_LINE]; /* buffer to hold the input command */
   //int flag; // equals 1 if a command is followed by "&"
   //char *args[MAX_LINE/2 + 1];/* max arguments */
   /*int should_run =1;

   pid_t pid,tpid;
   int i;

   
   while (1) //infinite loop for shell prompt
   {
      flag = 0; //flag =0 by default

      printf("COMMAND->");
      fflush(stdout);
      if(-1!=formatCommand(inputBuffer,args,&flag)) // get next command
      {
         pid = fork();

         if (pid < 0)//if pid is less than 0, forking fails
         {

            printf("Fork failed.\n");
            exit (1);
         }

         else if (pid == 0)//if pid ==0
         {

            //command not executed
            if (execvp(args[0], args) == -1)
            {

               printf("Error executing command\n");
            }
         }

         // if flag == 0, the parent will wait,
         // otherwise returns to the formatCommand() function.
         else
         {
            i++;
            if (flag == 0)
            {
               i++;
               waitpid(pid, NULL, 0);
            }
         }
      }
      }*/

   string tmpString;
   int length = -1;
   bool goodCmd = true;
   char inputBuffer[MAX_LINE]; /* buffer to hold the command entered */

   cout << "Use command 'h' for history. Could not get CTRL-/ to work. \n\n";
   while (1)
   {
      length = -1;
      goodCmd = true;

      while (length < 0)
      {
         cout << " COMMAND-> ";
         cout.flush();
         length = read(STDIN_FILENO, inputBuffer, MAX_LINE);
         // removes the newline and makes this a c-string
         inputBuffer[length - 1] = '\0';
      }

      if ( length == 1 )
      {
         // If the user just entered a newline, then the length of the
         // input will be 1 and we just want to give the prompt again.
         // (Ignoring the newline input.)
         goodCmd = false;
      }
      //else if (inputBuffer[0] == '^' && inputBuffer[1] == '_')
      else if(inputBuffer[0] == 'h')
      {
         displayHistory( );
         goodCmd = false;
      }
      else if ( length == 0 || (! strncmp( inputBuffer, "exit", MAX_LINE )) )
      {
         // End of file or exit command given
         exit(0);
      }
      else if (inputBuffer[0] == 'r' &&
               (inputBuffer[1] == '\0' || inputBuffer[1] == ' ') )
      {
         // If an 'r' command was given and a matching command is found in the
         // history, then put the command to be repeated in the buffer.
         // If an 'r' was given with no argument the most recent command
         // should be executed.  This is indicated with a null.
         if (inputBuffer[1] == '\0') inputBuffer[2] = '\0';

         goodCmd = findCommandInHistory(inputBuffer[2], tmpString);

         if ( goodCmd )
         {
            strncpy (inputBuffer, tmpString.c_str(), MAX_LINE);
         }
         else
         {
            cout << "Command not found\n";
            cout.flush();
         }

      }

      if ( goodCmd )
      {
         add2History( (string) inputBuffer );
         cout << "  command executed: " << inputBuffer << endl;
      }

      // end:  while(1)
   }

   // end of main()
   
}
