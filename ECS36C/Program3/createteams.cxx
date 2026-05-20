#include <bits/stdc++.h> // Universal header file
#include "json.hpp" // With version 3.11.2 from github
#include "priorityqueue.h" // using priorityqueue
#include "teamdata.h"
using namespace nlohmann; // for json object 
using namespace std; // for standard object

const size_t MAX_SIZE = 1000; // from test cases
bool bl[MAX_SIZE]; // Check if the element is already in use
int main(int argc, char** argv) {
	if (argc != 2) {
		cerr << "Parameter error!!!" << endl; // check the parameters
		return -1; // return with error status
	}
	
	PriorityQueue pq(MAX_SIZE); // Priority queue instance
	TeamData td(argv[1]);
	
	json res_obj; // for storing JSON format result
	
	// Create priority queue
	for (int i = 0; i < td.numPlayers() - 1; i++)
		for (int j = i+1; j < td.numPlayers(); j++) {
			Key key = td.winPercentages(i, j);
			key = fabs(key - 50);
			
			Value value = make_pair(i, j);
			pq.insert(make_pair(key, value));
		}
	
	
	int cnt = 0; // Record the number of successful matches
	vector<Value> res_vec;
	
	while (cnt < td.numPlayers() / 2)
	{
		auto ele = pq.removeMin();
		int i = ele.second.first;
		int j = ele.second.second;	
		
		if (!bl[i] && !bl[j]) {
			res_vec.push_back(ele.second);
			cnt++; 
			bl[i] = bl[j] = true; 
		}
		
	}
	res_obj["teams"] = res_vec;

	cout << res_obj.dump(2) << endl;
	
	return 0;
	
}
