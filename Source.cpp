#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

#define mdl 1000000007
using namespace std;

vector<pair<int, int>> bruteforce(string base, string s, int& cp)
{
	cp = 0;
	vector<pair<int, int>>rs;
	for (int i = 0; i < base.length() - s.length(); i++)
		for (int j = 0; j < s.length(); j++)
		{
			cp++;
			if (base[i + j] != s[j])
				break;
			if (j == s.length() - 1)
			{
				pair<int, int>a;
				a.first = i;
				a.second = i + s.length() - 1;
				rs.push_back(a);
			}
		}
	return(rs);
}

vector<pair<int, int>>rabinkarp(string base, string s, int& cp)
{
	unsigned long long hash_s(0), hash_base(0);
	int exponent(1);
	cp = 0;
	vector<pair<int, int>>rs;
	for (int i = s.length() - 1; i >= 0; i--)
	{
		hash_s += (s[i] * exponent) % mdl;
		hash_base += (base[i] * exponent) % mdl;
		hash_s %= mdl;
		hash_base %= mdl;
		exponent *= 10;
	}
	exponent /= 10;
	for (int i = 0; i < base.length() - s.length(); i++)
	{
		cp++;
		if (hash_base == hash_s)
		{
			pair<int, int>a;
			a.first = i;
			a.second = i + s.length() - 1;
			rs.push_back(a);
		}
		hash_base = 10 * (hash_base - (base[i] * exponent) % mdl) + base[i + s.length()] % mdl;
	}
	return(rs);
}

vector<int> partialmatch(string base, string s, int& cp)
{
	vector<int>rs;
	rs.resize(s.length());
	rs[0] = -1;
	rs[1] = 0;
	int val(0), run(2);
	while (run < s.length())
	{
		cp++;
		if (s[run - 1] == s[val])
		{
			val++;
			rs[run++] = val;
		}
		else if (val > 0)
			val = rs[val];
		else
			rs[run++] = 0;
	}
	return(rs);
}
vector<pair<int, int>>kmp(string base, string s, int& cp)
{
	cp = 0;
	vector<int>pmtable = partialmatch(base, s, cp);
	vector<pair<int, int>>rs;
	int i(0), j(0);
	while (i + j < base.length())
	{
		cp++;
		if (s[j] == base[i + j])
		{
			j++;
			if (j == s.length())
			{
				pair<int, int>a;
				a.first = i + j - s.length();
				a.second = i + j - 1;
				rs.push_back(a);
				i++;
				j = 0;
			}
		}
		else
		{
			if (pmtable[j] > -1)
			{	
				j = pmtable[j];
				i = i + j - pmtable[j];
			}
			else
			{
				i++;
				j = 0;
			}
		}
	}
	return(rs);
}

int main(int argc, char** argv)
{
	ifstream fin;
	fin.open(argv[2]);
	string base, s;
	getline(fin, base);
	getline(fin, s);
	fin.close();
	ofstream fout;
	fout.open(argv[3]);
	int cp(0), time;
	vector<pair<int, int>>rs;
	if (argv[1] == "-bf")
	{
		chrono::time_point<chrono::system_clock> start, end;
		time_t end_time;
		chrono::duration<double>elapsed_seconds;
		start = chrono::system_clock::now();
		rs = bruteforce(base, s, cp);
		end = chrono::system_clock::now();
		elapsed_seconds = end - start;
		end_time = chrono::system_clock::to_time_t(end);
		time = elapsed_seconds.count() * 1000;
	}
	else if (argv[1] == "-rk")
	{
		chrono::time_point<chrono::system_clock> start, end;
		time_t end_time;
		chrono::duration<double>elapsed_seconds;
		start = chrono::system_clock::now();
		rs = rabinkarp(base, s, cp);
		end = chrono::system_clock::now();
		elapsed_seconds = end - start;
		end_time = chrono::system_clock::to_time_t(end);
		time = elapsed_seconds.count() * 1000;
	}
	else if (argv[1] == "-kmp")
	{
		chrono::time_point<chrono::system_clock> start, end;
		time_t end_time;
		chrono::duration<double>elapsed_seconds;
		start = chrono::system_clock::now();
		rs = kmp(base, s, cp);
		end = chrono::system_clock::now();
		elapsed_seconds = end - start;
		end_time = chrono::system_clock::to_time_t(end);
		time = elapsed_seconds.count() * 1000;
	}
	if (argv[4] == "-t" || argv[4] == "-ind")
		fout << "Thoi gian thuc hien: " << time << '\n';
	if (argv[4] == "-n" || argv[4] == "-ind")
		fout << "So luong phep so sanh: " << cp << '\n';
	for (int i = 0; i < rs.size(); i++)
		fout << rs[i].first << ' ' << rs[i].second << '\n';
	fout.close();
}