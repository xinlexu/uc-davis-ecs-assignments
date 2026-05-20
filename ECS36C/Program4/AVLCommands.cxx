#include <iostream>
#include <fstream>
#include <vector>
#include "json.hpp" 
#include "AVL.h"


int main(int argc, char** argv) {
	if (argc != 2) {
		cerr << "Parameter error!!!" << endl; // check the parameters
		return -1; // return with error status
	}

	ifstream ifile(argv[1]); // for file reading and open 
	json js_obj; // for storing JSON format files 

	if (ifile.is_open()) {
		ifile >> js_obj; // read JSON to object
	}

	AVLTree T;

	for (auto it = js_obj.begin(); it != js_obj.end(); it++) {
		string js_key = it.key(); // define frequently used variables to simplify code
		int AVL_key;

		if (js_key == "metadata") {
			continue; // continue when there is a non-sample key-value pair
		}

		if (js_obj[js_key]["operation"] != "Insert") {
			throw "Error!!!";
		}

		AVL_key = js_obj[js_key]["key"];
		T.Insert(AVL_key);

	}


	cout << T.JSON() << endl;

	return 0;

}
