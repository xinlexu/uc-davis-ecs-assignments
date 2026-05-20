// Sequence.cpp
//

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
using namespace std;

class Sequence
{
public:
	Sequence(void);
	Sequence(const std::string& desc_str, const std::string& seq_str);
	std::string findNextMutation(const Sequence& s, const int& pos) const;
	friend Sequence findMUM(const Sequence& s1, const Sequence& s2);
	friend std::istream& operator >> (std::istream& is, Sequence& s);
	friend std::ostream& operator << (std::ostream& os, const Sequence& s);

private:
	void check(void) const;
	std::string desc;
	std::string seq;
};

Sequence::Sequence(void)
{
	desc = "";
	seq = "";
}

Sequence::Sequence(const string& desc_str, const string& seq_str)
{
	desc = desc_str;
	seq = seq_str;
}

string Sequence::findNextMutation(const Sequence& s, const int& pos) const
{
	string res = "";
	size_t i;
	for (i = pos-1; i < seq.size(); i++)
	{
		if (seq[i] != s.seq[i])
		{
			res = seq[i] + to_string(i+1) + s.seq[i];
			break;
		}
	}

	if (res == "")
		res = "no mutation found";

	return res;
}

void Sequence::check(void) const
{
	size_t i;
	for (i = 0; i < seq.size(); i++)
	{
		if (seq[i] > 'Z' || seq[i] < 'A')
			throw invalid_argument("invalid character in sequence");
	}

//	if (desc[len_desc-1] != '\n')
//		throw invalid_argument("EOF error");
}

Sequence findMUM(const Sequence& s1, const Sequence& s2)
{
	size_t len, i, pos1, pos2;
	size_t p2 = s2.seq.size() - 1;
	size_t first;
	size_t last;

	string seq_res = "";
	string desc_res = "";

	for (len = s2.seq.size(); len >= 20; len--)
	{
		for (i = 0; i + len - 1 <= p2; i++)
		{
			string subs = s2.seq.substr(i, len);
			first = s1.seq.find(subs);
			last = s1.seq.rfind(subs);
			if (first != string::npos && first == last)
			{
				pos1 = first + 1;
				pos2 = i + 1;
				seq_res = subs;
				break;
			}
		}
		if (seq_res != "")
			break;
	}

	if (seq_res == "")
		desc_res = ">No MUM found\n\n";
	else
	{
		desc_res = desc_res + ">MUM pos1=" + to_string(pos1) +" pos2=" + to_string(pos2);
		len = seq_res.size();
		for (i = 0; i < len; i++)
		{
			if (i % 70 == 0)
				desc_res += "\n";
			desc_res += seq_res[i];
		}
		desc_res += "\n";
		desc_res += "\n";
	}

	Sequence s(desc_res, seq_res);

	return s;
}

istream& operator >> (istream& is, Sequence& s)
{
	string line;

	while (getline(is, line))
	{
		if (line[0] != '>')
			s.seq += line;
		s.desc = s.desc + line + "\n";
		if (line == "")
			break;
	}
	s.check();
	return is;
}

ostream& operator << (std::ostream& os, const Sequence& s)
{
	os << s.desc;
	return os;
}

