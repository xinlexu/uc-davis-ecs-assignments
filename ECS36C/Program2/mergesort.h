// Merge Sort
// 
// Author: Rob Gysel
// ECS60, UC Davis
// Adapted from: Lysecky & Vahid "Data Structures Essentials", zyBooks

#include <vector>

void MergeSort(std::vector<int>* numbers, int& cmp_cnt, int& mem_access);
void MergeSortRecurse(std::vector<int>* numbers, int i, int k, int& cmp_cnt, int& mem_access);
void Merge(std::vector<int>* numbers, int i, int j, int k, int& cmp_cnt, int& mem_access);
