#include <bits/stdc++.h> // Universal header file
#include "json.hpp" // With version 3.11.2 from github
using namespace nlohmann; // for json object 
using namespace std; // for standard object

int main(int argc, char ** argv) {
	if (argc != 3) {
		cerr << "Parameter error!!!" << endl; // check the parameters
		return -1; // return with error status
	}
	
	ifstream ifile1, ifile2; // for file reading
	json js_obj1, js_obj2; // for storing JSON format files 
	
	ifile1.open(argv[1]); // open JSON file1
	if (ifile1.is_open()) {
		ifile1 >> js_obj1; // read JSON to object1
	}
	
	ifile2.open(argv[2]); // ditto with file2
	if (ifile2.is_open()) {
		ifile2 >> js_obj2; // ditto
	}
	
	int asize1, asize2, nsample1, nsample2, cnt_conflict = 0;
	// The above represent the stored array length 1~2, 
	// the number of samples 1~2 and the number of positions 
	// containing the conflicting data respectively
	
	asize1 = js_obj1["metadata"]["arraySize"]; // use json rules to assign values
	asize2 = js_obj2["metadata"]["arraySize"]; // ditto
	
	nsample1 = js_obj1["metadata"]["numSamples"]; // use json rules to assign values
	nsample2 = js_obj2["metadata"]["numSamples"]; // ditto
	
	json res_obj; // for storing JSON format result
	// iterate through the obj1's keys and find the confliction in each sample
	// assume nsample1 = nsample2 and asize1 = asize2
	for (auto it = js_obj1.begin(); it != js_obj1.end(); it++) {
		string key = it.key(); // define frequently used variables to simplify code
		string str; // for storing new key 
		bool is_conflict = false; // check whether the current sample has a conflict
		
		if (key == "metadata") {
			continue; // continue when there is a non-sample key-value pair
		}
		
		// iterate the samples
		for (int i = 0; i < asize1; i++) {
			str = to_string(i); // converts an integer type to a string type
			if (js_obj1[key][i] != js_obj2[key][i]) { // check if data is same 
				is_conflict = true; // flag this sample;
				res_obj[key]["Mismatches"][str] = {js_obj1[key][i], js_obj2[key][i]};
				// record mismatch pairs
			}
		}
		
		if (is_conflict) { // check if there is conflict data in this sample
			res_obj[key][argv[1]] = js_obj1[key]; // record the sample from file1 
			res_obj[key][argv[2]] = js_obj2[key]; // record the sample from file2 
			cnt_conflict++; // count the number of inversion samples
		}	
		
	}
	
	// construct the final JSON result
	res_obj["metadata"]["File1"]["arraySize"] = asize1; // save arraySize
	res_obj["metadata"]["File2"]["arraySize"] = asize2; // ditto
	
	res_obj["metadata"]["File1"]["name"] = argv[1]; // save file1 name
	res_obj["metadata"]["File2"]["name"] = argv[2]; // save file2 name
    
	res_obj["metadata"]["File1"]["numSamples"] = nsample1; // save the numble of samples
	res_obj["metadata"]["File2"]["numSamples"] = nsample2; // ditto
    
	res_obj["metadata"]["samplesWithConflictingResults"] = cnt_conflict; // save the numble of inversion samples
	cout << res_obj.dump(2) << endl; // use parameter 2 to output a human-readable format 
	
	ifile1.close(); // close file1
	ifile2.close(); // close file2
	
	return 0;  // normal exit of program 
	
}



