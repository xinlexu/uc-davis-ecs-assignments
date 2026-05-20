// WordleGame.cpp
//

#include <iostream>
#include <fstream>
#include <string>
#include <set>
using namespace std;

class WordleGame
{
  public:
  WordleGame(const std::string& filename);
  std::string score(const std::string& input);
  void select_key(int index);
  bool over(void) const;

  private:
  std::set<std::string> dictionary;
  std::string key;
  bool done;
};

WordleGame::WordleGame(const string& filename)
{
	ifstream fin{ filename };
	string s;
	while (fin >> s)
	{
		dictionary.insert(s);
	}
	key = 1;
	done = false;
}

string WordleGame::score(const std::string& input)
{
	string s = "-----";
	set<string>::iterator iter;
	iter = dictionary.find(input);
	if (iter == dictionary.end())
		return "not in dictionary";

	string no_match{ input };

	size_t i;
	for (i = 0; i < key.size(); i++)
	{
		if (key[i] == input[i])
		{
			s[i] = '*';
			no_match[i] = '0';
		}
	}

	for (i = 0; i < key.size(); i++)
	{
		if (no_match[i] != '0')
		{
			size_t pos = no_match.find(key[i]);
			if (pos != string::npos && no_match[pos] != '1')
			{
				no_match[pos] = '1';
				s[pos] = '+';
			}
		}
	}


	if (s == "*****")
	{
		done = true;
		return s;
	}
	return s;
	
}

void WordleGame::select_key(int index)
{
	if (index < 1)
		return;

	size_t size = dictionary.size();
	int pos = index % size;
	if (pos == 0)
		pos = size;

	set<string>::iterator iter = dictionary.begin();
	int i = 1;
	while (i < pos)
	{
		iter++;
		i++;
	}

	key = *iter;
	return;

}

bool WordleGame::over(void) const
{
	return done;
}

