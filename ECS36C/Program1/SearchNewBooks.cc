#include <bits/stdc++.h>
using namespace std;

class Timer 
{
public:
	chrono::high_resolution_clock::time_point start;
	
	// constructor function
	Timer() {start = chrono::high_resolution_clock::now(); }

	void reset() 
	{
        	start = chrono::high_resolution_clock::now();
    	}

   	 double current_time() const
	{
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed_us = chrono::duration<double, micro>(end - start).count();
        
        return elapsed_us;
    	}

	// overloading "<<" operator 
    friend ostream& operator<<(ostream& os, const Timer& timer) 
	{
        os << "CPU time: " << timer.current_time() << "microseconds." << endl;
        return os;
    }
};


class Book
{
public:
	int ISBN;
	string language;
	string type;
	
	// constructor function 
	Book() {ISBN = 0; language = ""; type = "";}
	Book(int isbn, string lan, string tp): 
		ISBN(isbn), language(lan), type(tp) {}
	
	// overloading "<<" operator 
    friend ostream& operator<<(ostream& os, const Book& book) 
	{
        os << "ISBN: " << book.ISBN << endl;
		os << "Language: " << book.language << endl;
		os << "Type: " << book.type << endl;
        return os;
    }
    
    // overloading "==" operator 
    bool operator==(const Book& other) const
	{
        return ISBN == other.ISBN && language == other.language && type == other.type;
    }
	
};


// overloading < operator for multi-keyword sorting
bool operator<(const Book& lbook, const Book& rbook) 
{
    if (lbook.ISBN != rbook.ISBN) 
        return lbook.ISBN < rbook.ISBN;
    else if (lbook.type != rbook.type)
    {
    	if (lbook.type == "new")
    		return true;
    	else if (rbook.type == "new")
    		return false;
    	else if (lbook.type == "used")
    		return true;
    	else if (rbook.type == "used")
    		return false;
    	else
    		throw "Type Error!";
    		
	}
    else
        return lbook.language < rbook.language;
}


// Function declaration
void read_book(string fname, vector<Book>& vBooks);
int linearSearch();
int binarySearch();
int recursiveBinarySearch();
int recursiveBinarySearch(int left, int right, const Book& reqBook);


// Use two vectors to save the raw data
vector<Book> v_newBooks, v_requestBooks;
string save_path = "found.dat";

int main(int argc, char* argv[]) 
{
    // Check if the command line arguments are right
    if (argc != 3 && argc != 4) 
	{
        cerr << "Usage: " 
			<< argv[0] 
			<< " <newBooks.dat> <request.dat> <result_file.dat>[optional]" 
			<< endl;
        return -1;
    }


    // Read books' file and exception detection
    try 
	{
    	read_book(argv[1], v_newBooks);
    } 
	catch (...) 
	{
        cerr << "Error: cannot open file " << argv[1] << endl;
        return -2;
    }
    
    try 
	{
        read_book(argv[2], v_requestBooks);
    } 
	catch (...) 
	{
        cerr << "Error: cannot open file " << argv[2] << endl;
        return -2;
    }

    // Execute the algorithm based on the search method and 
	// calculate the time
    char choice;
    Timer timer {};
    bool is_end = false;
    
    int res;
    
    while (!is_end) 
	{
        cout << "Choice of search method ([l]inear, [b]inary, [r]ecursiveBinary)? \n";
        cin >> choice;

        timer.reset();
        
        switch (choice) 
		{
            case 'l':
                res = linearSearch();
                is_end = true;
                break;
            case 'b':
                sort(v_newBooks.begin(), v_newBooks.end());
                res = binarySearch();
                is_end = true;
                break;
            case 'r':
                sort(v_newBooks.begin(), v_newBooks.end());
                res = recursiveBinarySearch();
                is_end = true;
                break;
            default:
                cerr << "Incorrect choice" << std::endl;
                break;
        }
	}
    
    if (argc == 4)
    	save_path = argv[3];
    ofstream file(save_path);
	cout << timer << endl;
	file << res << endl;
	
	file.close(); 
	
	return 0;
} 

void read_book(string fname, vector<Book>& vBooks) 
{
    ifstream file(fname);
    string line;
    while (getline(file, line)) 
	{
		int ISBN;
        string language, type;
        
        for (auto& ch : line)
        	if (ch == ',')
        		ch = ' ';
        
        istringstream iss(line);
        
        if (!(iss >> ISBN >> language >> type)) 
		{ 
			break; 
		}
        vBooks.push_back(Book(ISBN, language, type));
    }
}



// Linear search function 
int linearSearch() 
{
    int cnt = 0;
    for (const auto& reqBook : v_requestBooks) 
        for (const auto& newBook : v_newBooks) 
            if (reqBook == newBook) 
			{
                cnt++; 
                break; 
            }
    return cnt;
}

// Binary search function 
int binarySearch() 
{
    int cnt = 0;
    for (const auto& reqBook : v_requestBooks) 
	{
        int left = 0;
        int right = v_newBooks.size() - 1;
        while (left <= right) 
		{
            int mid = (left + right) / 2;
            if (v_newBooks[mid] == reqBook) 
			{
                cnt++;
                break;
            } 
			else if (v_newBooks[mid] < reqBook) 
			{
                left = mid + 1;
            } 
			else 
			{
                right = mid - 1;
            }
        }
    }
    return cnt;
}

// Recursive binary search function 
int recursiveBinarySearch(int left, int right, const Book& reqBook) 
{
    if (left > right) 
        return -1;
        
    int mid = (left + right) / 2;
    if (v_newBooks[mid] == reqBook) 
        return mid;
    else if (v_newBooks[mid] < reqBook) 
        return recursiveBinarySearch(mid + 1, right, reqBook);
    else 
        return recursiveBinarySearch(left, mid - 1, reqBook);
    
    return 0;
}

int recursiveBinarySearch() 
{
    int cnt = 0;
    
    for (const auto& reqBook : v_requestBooks) 
	{
		int end = v_newBooks.size() - 1;
        if (recursiveBinarySearch(0, end, reqBook) != -1) 
            cnt++;
    }
    
    return cnt;
}





