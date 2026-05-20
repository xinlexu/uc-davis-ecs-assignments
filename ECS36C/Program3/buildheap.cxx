#include <bits/stdc++.h> // Universal header file
#include "json.hpp" // With version 3.11.2 from github
#include "priorityqueue.h" // using priorityqueue
using namespace nlohmann; // for json object 
using namespace std; // for standard object

const size_t MAX_SIZE = 30; // from test cases
int main(int argc, char** argv) {
	if (argc != 2) {
		cerr << "Parameter error!!!" << endl; // check the parameters
		return -1; // return with error status
	}
	
	string filename = argv[1];
	ifstream ifile(filename); // for file reading and open 
	json js_obj; // for storing JSON format files 
	
	if (ifile.is_open()) {
		ifile >> js_obj; // read JSON to object
	}
	
	PriorityQueue pq(MAX_SIZE); // Priority queue instance
	json res_obj; // for storing JSON format result
//	size_t max_heap_size = 0; // for finding the maximum size
	size_t num_operations = 0; // for counting operations
	
	// iterate through the obj's keys 
	for (auto it = js_obj.begin(); it != js_obj.end(); it++) {
		string key_json = it.key(); // define frequently used variables to simplify code
		
		
		if (key_json == "metadata") {
			continue; // continue when there is a non-sample key-value pair
		} else {
			num_operations++;
			string op = js_obj[key_json]["operation"];
			if (op == "insert") {
				Key key_pair = js_obj[key_json]["key"];
				pq.insert(key_pair);
			}
			else if (op == "removeMin") {
				pq.removeMin();
			}
			else {
				cerr << "Invalid operation!" << endl;
			}
//			if (max_heap_size < pq.size())
//				max_heap_size = pq.size();
		}
		
	}
	
	// Fill results as required
	res_obj = pq.JSON();
	res_obj["metadata"]["maxHeapSize"] = MAX_SIZE;
	res_obj["metadata"]["numOperations"] = num_operations;
	cout << res_obj.dump(2) << endl;
	
	return 0;
	
}
