﻿#include <iostream>
#include <iterator>
#include <vector>
#include <array>
#include "preklady.h"
#include <algorithm>

using namespace std;

bool cmp::operator()(const Dvojka& left, const Dvojka& right) const
{
	{
		if (left.first == right.first)
		{
			if (left.second.length() < right.second.length())
			{
				return true;
			}
			else if (left.second.length() == right.second.length())
			{
				string l2 = left.second;
				string r2 = right.second;
				for (auto& c : l2)
				{
					c = tolower(c);
				}
				for (auto& c : r2)
				{
					c = tolower(c);
				}
				return l2 < r2;
			}
			else
			{
				return false;
			}
		}
		else
		{
			string l1 = left.first;
			string r1 = right.first;
			for (auto& c : l1)
			{
				c = tolower(c);
			}
			for (auto& c : r1)
			{
				c = tolower(c);
			}


			if (l1 == r1)
			{
				return left.first < right.first;
			}
			else
			{
				return l1 < r1;
			}	
		}
	}
}
bool mysort(const string& s1, const string& s2)
{
	if (s1.length() < s2.length())
	{
		return true;
	}
	else if (s1.length() == s2.length())
	{
		string l2 = s1;
		string r2 = s2;
		for (auto& c : l2)
		{
			c = tolower(c);
		}
		for (auto& c : r2)
		{
			c = tolower(c);
		}
		return l2 < r2;
	}
	else
	{
		return false;
	}
}

void Preklady::add(const string& slovo, const string& preklad)
{
	preklady.insert(Dvojka(slovo, preklad));
	if (slova.find(slovo)==slova.end())
	{
		slova.insert(pair < string, vector<string>>(slovo, vector{preklad}));
	}
	else
	{
		slova[slovo].push_back(preklad);
	}
}

void Preklady::del(const string& slovo, const string& preklad)
{
	preklady.erase(Dvojka(slovo, preklad));
}

void Preklady::del(const string& slovo)
{
	if (slova.find(slovo)!=slova.end())
	{
		int size = slova[slovo].size();
		for (int i = 0; i < size; i++)
		{
			preklady.erase(pair<string, string>(slovo, slova[slovo][i]));
		}
		slova.erase(slovo);
	}
}

Rozmezi Preklady::find(const string& slovo)
{
	set<Dvojka>::iterator it1 = preklady.end();
	set<Dvojka>::iterator it2 = preklady.end();

	if (slova.find(slovo) != slova.end())
	{
		sort(slova[slovo].begin(), slova[slovo].end(), mysort);
		it1 = preklady.find(Dvojka(slovo, slova[slovo][0]));
	}

	for (auto i = it1; i != preklady.end(); ++i)
	{
		if (i->first != slovo)
		{
			it2 = i;
			break;
		}
	}

	return { it1,it2 };
}
Rozmezi Preklady::prefix(const string& pre)
{
	set<Dvojka>::iterator it1 = preklady.end();
	set<Dvojka>::iterator it2 = preklady.end();
	bool found = false;
	int size = pre.size();

	for (auto i = preklady.begin(); i != preklady.end(); ++i)
	{
		string str = i->first;
		for (auto& c : str)
		{
			c = tolower(c);
		}

		if (!found && str.compare(0, size, pre) == 0)
		{
			found = true;
			it1 = i;
		}
		else if (found && str.compare(0, size, pre) != 0)
		{
			it2 = i;
			break;
		}
	}

	return { it1,it2 };
}

void print(const Rozmezi& interval)
{
	if (interval[0] == interval[1])
	{
		return;
	}
	for (auto i = interval[0]; i != interval[1]; ++i)
	{
		cout << i->second << " ";
	}
	cout << endl;
}

void print_pre(const Rozmezi& interval)
{
	if (interval[0] == interval[1])
	{
		return;
	}

	string slovo = interval[0]->first;
	cout << slovo + ": ";

	for (auto i = interval[0]; i != interval[1]; ++i)
	{
		if (i->first == slovo)
		{
			cout << i->second + " ";
		}
		else
		{
			cout << endl;
			slovo = i->first;
			cout << slovo + ": " << i->second + " ";
		}
	}
	cout << endl;
}