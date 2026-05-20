#include <bits/stdc++.h> // Universal header file
#include "json.hpp" // With version 3.11.2 from github
#include "insertionsort.h" // Import the insertion sort function
#include "mergesort.h" // Import the merge sort function
#include "quicksort.h" // Import the quick sort function
using namespace nlohmann; // for json object 
using namespace std; // for standard object

// use this function to calculate the time consumed in units CLOCKS_PER_SEC
double cal_time (clock_t s, clock_t e) {
	double res = (double)(e - s) / CLOCKS_PER_SEC; // convert time in seconds 
	return res; // return result in double
}

int main(int argc, char ** argv) {
	if (argc != 2) {
		cerr << "Parameter error!!!" << endl; // check the parameters
		return -1; // return with error status
	}
	
	ifstream ifile; // for file reading
	json js_obj; // for storing JSON format files 
	
	ifile.open(argv[1]); // open JSON file
	if (ifile.is_open()) {
		ifile >> js_obj; // read JSON to object
	}
	
	clock_t start, end; // for calculating time consumption
	int cmp_cnt, mem_access; // for storing comparing count and memory access result 
	
	// output the header of the CSV format first
	cout << "Sample" << "," << "InsertionSortTime" << "," << "InsertionSortCompares" << ","
		<< "InsertionSortMemaccess" << "," << "MergeSortTime" << "," << "MergeSortCompares" << ","
		<< "MergeSortMemaccess" << "," << "QuickSortTime" << "," << "QuickSortCompares" << "," 
		<< "QuickSortMemaccess" << endl;
	
	vector<int> raw_vec; // for storing the raw array 
	// iterate through the keys and do the statistical analysis
	for (auto it = js_obj.begin(); it != js_obj.end(); it++) {
		string key = it.key(); // define frequently used variables to simplify code
		vector<int> * new_vec; // a temporary vector used for each sort

		if (key == "metadata") {
			continue; // continue when there is a non-sample key-value pair
		}

		raw_vec = it.value().get<vector<int> >();  // Converts a JSON array to an integer vector	
	
		cout << key << ",";	// output the contents of each column as required 
		
		new_vec = new vector<int> (raw_vec);  // use initial array for sorting to ensure algorithm consistency
		start = clock(); // mark start time 
		InsertionSort(new_vec, cmp_cnt, mem_access); // first do insertion sorting
		end = clock(); // mark end time
		cout << cal_time(start, end) << "," << cmp_cnt << "," << mem_access << ",";
		// output the contents of each column as required 
		
		new_vec = new vector<int> (raw_vec); // use initial array for sorting to ensure algorithm consistency
		start = clock(); // mark start time
		MergeSort(new_vec, cmp_cnt, mem_access); // second do merge sorting
		end = clock(); // mark end time
		cout << cal_time(start, end) << "," << cmp_cnt << "," << mem_access << ",";
		// output the contents of each column as required 

		new_vec = new vector<int> (raw_vec); // use initial array for sorting to ensure algorithm consistency
		start = clock(); // mark start time
		QuickSort(new_vec, cmp_cnt, mem_access); // third do quick sorting
		end = clock(); // mark end time
		cout << cal_time(start, end) << "," << cmp_cnt << "," << mem_access;	
		// output the contents of each column as required 
		
		cout << endl; // end-of-row separator	
		
	}
	
	ifile.close(); // close file
	
	return 0;  // normal exit of program 
	
}



