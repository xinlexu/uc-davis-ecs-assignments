#include <bits/stdc++.h>
using namespace std;

map <string, int> m;


bool check(string s)
{
	size_t i;
	for (i=0; i<s.size(); i++)
		if (!isalpha(s[i]))
			return false;
	return true;
}

bool my_cmp(const pair<string, int>& l, const pair<string, int>& r)
{
	if (l.second > r.second)
		return true;
	else if (l.second < r.second)
		return false;
	else if (l.first < r.first)
		return true;
	else
		return false;
}


int main()
{
	map <string, int>::iterator m_iter;
	
	string s = "";
	bool is_valid = false;
	
	while (cin >> s)
	{
		is_valid = check(s);
		if (!is_valid)
			continue;
		m_iter = m.find(s);
		if (m_iter == m.end())
			m.insert(pair<string, int> (s, 1));
		else
			m_iter->second++;
	}
	
	vector<pair<string, int> > v(m.begin(),m.end());
	vector<pair<string, int> >::iterator v_iter;
	
	sort(v.begin(), v.end(), my_cmp);
	
	for (v_iter=v.begin(); v_iter!=v.end(); v_iter++)
		cout << v_iter->first << ' ' << v_iter->second << endl;
		
	return 0;
}


