/***********************************************************************
* Program:
*    Lab 00T2, Practice Lab Submission
*    Brother Jones, CS345
* Author:
*    Preston Price
* Summary: 
*    Count number of integers entered by users.
************************************************************************
************************************************************************
*
* Changes made to my code for the re-submission:
*     - Just did everything in main since the program is small.
*
************************************************************************/
   
#include <iostream>
using namespace std;

/**********************************************************************
 * Calls countIntegers() and outputs how many integers were entered. 
 ***********************************************************************/
int main()
{
   int input = -1;
   int count = -1;

   while (input != 0)
   {
      count++;
      cout << "Enter an integer: ";
      cin >> input;
   }
   
   if (count == 0)
   {
      cout << "No non-zero integers were entered.\n";
   }
   else if (count == 1)
   {
      cout << "You entered " << count << " integer.\n";
   }
   else
   {
      cout << "You entered " << count << " integers.\n";
   }

   return 0;
}

