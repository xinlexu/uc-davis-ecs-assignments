#include <fstream>
#include "json.hpp" // With version 3.11.2 from github
#include "teamdata.h"
using namespace nlohmann; // for json object 
using namespace std; // for standard object

TeamData::TeamData(string filename) {
	
	for (int i = 0; i <= 100; i++)
		for (int j = 0; j <= 100; j++)
			winPercentages_[i][j] = -1.;
	
	ifstream ifile(filename); // for file reading and open 
	json js_obj; // for storing JSON format files 
	
	if (ifile.is_open()) {
		ifile >> js_obj; // read JSON to object
	}
	
	numPlayers_ = js_obj["metadata"]["numPlayers"];
	vector<json> js_array = js_obj["teamStats"].get<vector<json> >();
	
	for (auto it = js_array.begin(); it != js_array.end(); it++) {
		int i = (*it)["playerOne"];
		int j = (*it)["playerTwo"];
		double win_pctg = (*it)["winPercentage"];
		
		winPercentages_[i][j] = winPercentages_[j][i] = win_pctg;
	}
	
}

int TeamData::numPlayers() const {
	return numPlayers_;
}



