// Insertion Sort
// 
// Author: Rob Gysel
// ECS60, UC Davis
// Adapted from: Lysecky & Vahid "Data Structures Essentials", zyBooks

#include "insertionsort.h"

void InsertionSort(std::vector<int>* numbers, int& cmp_cnt, int& mem_access) {
   int i = 0;
   int j = 0;
   int temp = 0;  // Temporary variable for swap
   
   cmp_cnt = mem_access = 0; // Initializes the statistics parameter!!!
   
   for (i = 1; i < numbers->size(); ++i) {
      j = i;
      // Insert numbers[i] into sorted part
      // stopping once numbers[i] in correct position
      while (j > 0 && (*numbers)[j] < (*numbers)[j - 1]) {
         
         // Swap numbers[j] and numbers[j - 1]
         temp = (*numbers)[j];
         (*numbers)[j] = (*numbers)[j - 1];
         (*numbers)[j - 1] = temp;
         --j;
         
         // Statistical parameter!!!
         cmp_cnt++;
         mem_access += 6;

      }
      
      // Statistical parameter!!!
      if (j > 0) {
         cmp_cnt++;
         mem_access += 2;
      }

   }
   
   return;
}
