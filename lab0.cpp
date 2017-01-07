/***********************************************************************
* Program:
*    Lab 00, Practice Lab Submission
*    Brother Jones, CS345
* Author:
*    Preston Price
* Summary: 
*    Count number of integers entered by users.
************************************************************************/

#include <iostream>
using namespace std;

/**********************************************************************
 * Counts how many integers a user enters before they enter 0.
 ***********************************************************************/
int countIntegers()
{
   int input = -1;
   int count = -1;

   while (input != 0)
   {
      count++;
      cout << "Enter an integer: ";
      cin >> input;
   }

   return count;
}

/**********************************************************************
 * Calls countIntegers() and outputs how many integers were entered. 
 ***********************************************************************/
int main()
{
   int count = countIntegers();

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

