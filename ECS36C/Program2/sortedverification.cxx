#include <bits/stdc++.h> // Universal header file
#include "json.hpp" // With version 3.11.2 from github
using namespace nlohmann; // for json object 
using namespace std; // for standard object

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
	
	int asize, nsample, cnt_inversions = 0;
	// The above represent the stored array length, 
	// the number of samples and the number of samples 
	// containing the reverse order respectively
	
	asize = js_obj["metadata"]["arraySize"]; // use json rules to assign values
	nsample = js_obj["metadata"]["numSamples"]; // ditto
	
	json res_obj; // for storing JSON format result
	// iterate through the keys and find the number of inversions in each sample
	for (auto it = js_obj.begin(); it != js_obj.end(); it++) {
		string key = it.key(); // define frequently used variables to simplify code
		string str; // for storing new key 
		bool is_inversion = false; // check whether the current sample has an inversion
		
		if (key == "metadata") {
			continue; // continue when there is a non-sample key-value pair
		}
		
		// iterate the samples
		for (int i = 0; i < asize - 1; i++) {
			str = to_string(i); // converts an integer type to a string type
			if (js_obj[key][i] >= js_obj[key][i+1]) { // check if is ascending 
				is_inversion = true; // flag this sample;
				res_obj[key]["ConsecutiveInversions"][str] = {it.value()[i], it.value()[i+1]}; 
				// record reverse pairs
			}
		}
		
		if (is_inversion) { // check if there is inversion data in this sample
			res_obj[key]["sample"] = it.value(); // record the inversion sample 
			cnt_inversions++; // count the number of inversion samples
		}	
		
	}
	
	// construct the final JSON result
	res_obj["metadata"]["arraySize"] = asize; // save arraySize
	res_obj["metadata"]["file"] = argv[1]; // save file name
	res_obj["metadata"]["numSamples"] = nsample; // save the numble of samples
	res_obj["metadata"]["samplesWithInversions"] = cnt_inversions; // save the numble of inversion samples
	cout << res_obj.dump(2) << endl; // use parameter 2 to output a human-readable format 
	
	ifile.close(); // close file
	
	return 0;  // normal exit of program 
	
}



