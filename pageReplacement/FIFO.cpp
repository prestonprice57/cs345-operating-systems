/***********************************************************************
* Program:
*    Lab PageReplacement
*    Brother Jones, CS 345
* Author:
*    Preston Price
* Summary:
*    Uses the FIFO page replacement algorithm.
*
************************************************************************/

#include "vmalgorithm.h"

/**************************************************************************
 * Function:
 *    void FIFO::execute() - Executes the FIFO page replacement algorithm
 * Inherited Variables and Methods:
 *    int NUM_FRAMES: How many frames there are in 'memory'. (eg. 4)
 *    int frames[NUM_FRAMES]: The array that contains the pages resident
 *          in memory.  Use this array for the pages in memory.  
 *          See Also: NUM_FRAMES
 *    int hit(int page): Function that scans the 'frames[]' array, searching
 *          for 'page'.  If 'page' is found, it will return the index of 
 *          the 'frames[]' array.  If the page is not found, the function
 *          returns '-1'.
 *    int myPageFactory->getPage(): Returns the next page the psuedo-program 
 *          requests.  When there are no more pages to return, this function
 *          returns a -1.
 *    void display(int page, int frames[], bool fault): Function to display
 *          the information to stdout.  The parameter 'page' is the current
 *          page (the one returned from myPageFactory->getPage()), 'frames[]'
 *          is the array of the current resident set, and when 'fault' is 
 *          true, the display function will indicate it to the screen. 
 *          This function must be called for every page that is accessed.
 *************************************************************************/ 
void FIFO::execute()
{
   int first = 0; // Keeps track of which item was first
   int fill = 0; // Used to fill all frames
   int page = myPageFactory->getPage();

   while (page != -1) {
      int pageFound = hit(page);

      if (pageFound == -1) {
         if (fill != NUM_FRAMES) {
            frames[fill] = page;
            fill++;
         } else {
            frames[first] = page;
            first = (first + 1) % NUM_FRAMES;
         }
         display(page, frames, true);
      } else {
         display(page, frames, false);
      }
      
      page = myPageFactory->getPage();
   }
}

