/***********************************************************************
* Program:
*    Lab PageReplacement
*    Brother Jones, CS 345
* Author:
*    Your Name
* Summary:
*    Descriptive text ...
************************************************************************/

#include "vmalgorithm.h"
#include <queue>
#include <algorithm>

/**************************************************************************
 * Function:
 *    void LRU::execute() - Executes the LRU page replacement algorithm
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
void LRU::execute()
{
   vector<int> recentQ; // stores numbers from least to most recently used
   int front;
   int fill = 0; // Used to fill all frames
   int page = myPageFactory->getPage();

   while (page != -1) {
      int pageFound = hit(page);

      if (pageFound == -1) {
         if (fill != NUM_FRAMES) {
            frames[fill] = page;
            recentQ.push_back(fill);
            fill++;
         } else {
            front = recentQ[0];
            frames[front] = page;
            recentQ.erase(recentQ.begin());
            recentQ.push_back(front);
         }
         display(page, frames, true);
      } else {
         vector<int>::iterator it;
         it = std::find(recentQ.begin(), recentQ.end(), pageFound);

         recentQ.erase(it);
         recentQ.push_back(pageFound);
         display(page, frames, false);
      }

      page = myPageFactory->getPage();
   }
}

