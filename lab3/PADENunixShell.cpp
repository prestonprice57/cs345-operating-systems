/***********************************************************************
* Program:
*    Lab UnixShell  
*    Brother Jones, CS 345
* Author:
*    Paden Jones
* Summary:
*    Shell program with history feature. Handles processes being put
*    into the background.
************************************************************************/

#include <iomanip>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

using namespace std;

#define BUFFER_SIZE 80
#define HIST_SIZE 10
#define MAX_NUM_CMDS 1000

// A global array of strings and a count of the number of valid strings in it.
string commands[HIST_SIZE];
int goodStringCount = 0;

/*************************************************************************
* handle_SIGQUIT is the signal handler for the SIGQUIT signal  
**************************************************************************/
void handle_SIGQUIT(int junk)
{
   int start;

   if (goodStringCount > HIST_SIZE)
      start = goodStringCount - HIST_SIZE;
   else
      start = 0;

   write(STDOUT_FILENO, "\n", 1);
   for (int i = start; i < goodStringCount; i++)
   {
      
      int bufferSize = 7 + commands[i % HIST_SIZE].length();
      char buffer[bufferSize];
      sprintf(buffer, " %3d  %s\n", i, commands[i % HIST_SIZE].c_str());
      write(STDOUT_FILENO, buffer, bufferSize);
   }

   return; 
}


/*************************************************************************
* add2History() adds a command to the history buffer and increments the
* number of commands that have been entered up to a maximum count.  If the
* history buffer is full, the oldest command is overwritten.    
**************************************************************************/
void add2History (string input)
{
   commands[goodStringCount % HIST_SIZE] = input;
   goodStringCount++;
   
   // Roll over the count if it hits the maximum allowed for printing 
   if ( goodStringCount >= MAX_NUM_CMDS )
   {
      goodStringCount = HIST_SIZE; // Leave a full buffer of commands
   }
}

  
/*************************************************************************
* findCommandInHistory() searches backward for the first command that
* begins with a particular character (char c) in the history buffer.
* If a string is found that begins with the character, the string is
* passed back to the calling function.  If the character being looked
* for is a NULL then the most recent command is returned.  In both of
* the preceeding cases, the return value is set to true.  If no matching
* string is found, the return value is set to false.
**************************************************************************/
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


/*************************************************************************
* Display the contents of the history buffer of commands with the number
* of the command listed on the beginning of the line.
**************************************************************************/
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

/******************************************************************************
* setup() reads in the next command line, separating it into distinct tokens
* using whitespace as delimiters.
*
* setup() modifies the inputBuffer creating a set of null-terminated strings
* and places pointers into the args[] array that point to the beginning of
* each argument.  A NULL pointer is placed in the args[] array indicating
* the end of the argument list.  This is what is needed for using execvp().
*
* A ^d input at the beginning of a line, by a user, exits the shell.
******************************************************************************/
void setup(char inputBuffer[], char *args[], int *background)
{
   int length; // # of characters in the command line
   int i;      // loop index for accessing inputBuffer array
   int start;  // index where beginning of next command parameter is
   int ct;     // index of where to place the next parameter into args[]
   
   ct = 0;
    
   // read what the user enters on the command line
   length = read(STDIN_FILENO, inputBuffer, BUFFER_SIZE);  
   
   start = -1; // Used as a flag to indicate that we are looking
               // for the start of the command or an argument if
               // it is a -1, or stores the starting position of
               // the command or argument to be put in args[].
               
   if (length == 0)
   {
      exit(0); // ^d was entered, end of user command stream
   }
      
   if (length < 0)
   {
      perror("error reading the command");
      exit(-1); // terminate with error code of -1
   }

   // examine every character in the inputBuffer
   for (i = 0; i < length; i++)
   {
      switch (inputBuffer[i])
      {
         case ' ':
         case '\t' :               /* argument separators */
            if (start != -1)        /* found the end of the command or arg */
            {
               args[ct] = &inputBuffer[start]; /* set up pointer in args[] */
               ct++;
            }
            inputBuffer[i] = '\0'; /* add a null char; make a C string */
            start = -1;
            break;

         case '\n':                 /* should be the final char examined */
            if (start != -1)        /* if in the command or an argument */
            {
               args[ct] = &inputBuffer[start]; /* set up pointer in args[] */ 
               ct++;
            }
            inputBuffer[i] = '\0';
            args[ct] = NULL; /* no more arguments to this command */
            break;

         default :             /* some other character */
            if (start == -1 && inputBuffer[i] != '&')
            {
               start = i;      /* starting position of the command or arg */
            }
               
            if (inputBuffer[i] == '&')
            {
               *background  = 1;
               inputBuffer[i] = '\0';
            }
      }
   }
   
   args[ct] = NULL; /* just in case the input line was > 80 */
}

/******************************************************************************
* setupNoInput() is used to convert a non-user inputted string to args array
******************************************************************************/
void setupNoInput(char inputBuffer[], char *args[], int *background)
{
   int length = BUFFER_SIZE; /* # of characters in the command line */
   int i;      /* loop index for accessing inputBuffer array */
   int start;  /* index where beginning of next command parameter is */
   int ct;     /* index of where to place the next parameter into args[] */
   
   ct = 0;
    
   
   start = -1;            /* Used as a flag to indicate that we are looking
                           * for the start of the command or an argument if
                           * it is a -1, or stores the starting position of
                           * the command or argument to be put in args[].
                           */
   if (length == 0)
      exit(0);            /* ^d was entered, end of user command stream */
   if (length < 0)
   {
      perror("error reading the command");
      exit(-1);           /* terminate with error code of -1 */
   }

   /* examine every character in the inputBuffer */
   for (i = 0; i < length; i++)
   {
      switch (inputBuffer[i])
      {
         case ' ':
         case '\t' :               /* argument separators */
            if (start != -1)        /* found the end of the command or arg */
            {
               args[ct] = &inputBuffer[start]; /* set up pointer in args[] */
               ct++;
            }
            inputBuffer[i] = '\0'; /* add a null char; make a C string */
            start = -1;
            break;

         case '\n':                 /* should be the final char examined */
            if (start != -1)        /* if in the command or an argument */
            {
               args[ct] = &inputBuffer[start]; /* set up pointer in args[] */ 
               ct++;
            }
            inputBuffer[i] = '\0';
            args[ct] = NULL; /* no more arguments to this command */
            break;

         default :             /* some other character */
            if (start == -1 && inputBuffer[i] != '&')
            {
               start = i;      /* starting position of the command or arg */
            }
               
            if (inputBuffer[i] == '&')
            {
               *background  = 1;
               inputBuffer[i] = '\0';
            }
      }
   }
   
   args[ct] = NULL; /* just in case the input line was > 80 */
}

/******************************************************************************
* Main loop
******************************************************************************/
int main(void)
{
   int background; 
   char inputBuffer[BUFFER_SIZE];
   char *args[BUFFER_SIZE / 2];

   signal(SIGQUIT, handle_SIGQUIT);   

   while (1)
   {
      background = 0;
      printf(" COMMAND-> ");
      fflush(stdout);

      setup(inputBuffer, args, &background);

      bool skip = false;
      bool search = false;
      char token = '\0';
      string historyString = "";
      
      // Skip if only a blank line is entered
      if (args[0] == NULL)
      {
         skip = true; 
      }
      
      // If user wants to search history, get the search token
      if (args[0] != NULL && inputBuffer[0] == 'r' && inputBuffer[1] == '\0')
      {
         search = true;
         
         if (args[1] != NULL)
         {
            token = args[1][0];
         }
         else
         {
            token = '\0';
         }
      }
      
      // Search for character in history
      if (search)
      {
         
         findCommandInHistory(token, historyString);
         
         if (historyString == "")
         {
            cout << "No matching command in history" << endl;
            skip = true;
         }
      }
      
      // The user: searched & found, or entered a command
      if (!skip)
      {
         // Entered a command (concat args and store in history)
         if (historyString == "")
         {
            for (int i = 0; args[i] != NULL; i++)
            {
               historyString += string(args[i]) + " ";
            }
            
            if (background)
            {
               historyString += "&";
            }
         }
         // Searched an old command (split up history string into args)
         else
         {
            char temp[BUFFER_SIZE];
            strcpy(temp, historyString.c_str());
            setupNoInput(temp, args, &background);
            cout << historyString << std::endl;
         }
      
         // Write resulting command to history
         add2History(historyString);
      }
      
      pid_t pid = fork();
      if (pid == -1)
      {
         printf("fork error");
      }
      if (pid == 0)
      {
         if (execvp(args[0], args) == -1)
         {
            printf("command not found\n");
            exit(EXIT_FAILURE);
         }
      }
      else
      {
         if (!background)
         {
            waitpid(pid, NULL, 0);
         }
      }
   }
}

