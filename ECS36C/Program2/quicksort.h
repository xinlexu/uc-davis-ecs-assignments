// Quicksort
// 
// Author: Rob Gysel
// ECS60, UC Davis
// Adapted from: Lysecky & Vahid "Data Structures Essentials", zyBooks

#include <vector>

void QuickSort(std::vector<int>* numbers, int& cmp_cnt, int& mem_access);
void QuickSortRecurse(std::vector<int>* numbers, int i, int k, int& cmp_cnt, int& mem_access);
int Partition(std::vector<int>* numbers, int i, int k, int& cmp_cnt, int& mem_access);
