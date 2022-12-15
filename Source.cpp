#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <ctime>

#define mdl 1000000007
using namespace std;

int max(int a, int b)
{
	if (a > b) return a;
	return b;
}

vector<pair<int, int>> bruteforce(string base, string s, int& cp)
{
	cp = 0;
	vector<pair<int, int>>rs;
	for (int i = 0; i < base.length() - s.length() + 1; i++)
	{
		for (int j = 0; j < s.length(); j++)
		{
			cp+=2;
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
		cp+=2;
	}
	cp++;
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
		cp++;
		hash_s += (s[i] * exponent) % mdl;
		hash_base += (base[i] * exponent) % mdl;
		hash_s %= mdl;
		hash_base %= mdl;
		exponent *= 10;
	}
	cp++;
	exponent /= 10;
	for (int i = 0; i < base.length() - s.length() + 1; i++)
	{
		cp += 2;
		if (hash_base == hash_s)
		{
			pair<int, int>a;
			a.first = i;
			a.second = i + s.length() - 1;
			rs.push_back(a);
		}
		cp++;
		if (i < s.length() < base.length())
			hash_base = 10 * (hash_base - (base[i] * exponent) % mdl) + base[i + s.length()] % mdl;
	}
	cp++;
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
		cp += 3;
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
	cp++;
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
		cp += 2;
		if (s[j] == base[i + j])
		{
			j++;
			cp++;
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
			cp++;
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
	cp++;
	return(rs);
}

//ham phu tro Boyer-Moore
void badCharHeuristic(string str, int badchar[]) //ham khoi tao mang voi phan tu khong thuoc pat thi gia tri la 1
{
	// khoi tao vi tri cho tat ca cac phan tu co the xuan hien trong path la -1
	for (int i = 0; i < 256; i++)
		badchar[i] = -1;

	//dien vi tri cuoi cung ma ki tu thi i cua pat xuat hien trong pat
	for (int i = 0; i < str.size(); i++)
		badchar[str[i]] = i;
}


vector<pair<int, int>> boyerm(string txt, string pat, int& cmp)
{
	vector<pair<int, int>> list;
	int m = pat.size();
	int n = txt.size();

	int badchar[256];

	//dien mang badChar
	badCharHeuristic(pat, badchar);

	int index = 0; // index la vi tri dang xet cua xau text

	while (++cmp && index <= (n - m))
	{
		int j = m - 1; //la vi tri dang xet cua xau pat, bat dau tu cuoi chuoi

		//bat dau xet match tu cuoi pat ve dau pat
		while (++cmp && ++cmp && j >= 0 && pat[j] == txt[index + j])
			j--;

		if (++cmp && j < 0) //truong hop tim duoc string match, j duyet qua het pat nen co gia tri -1
		{
			pair<int, int> a;
			a.first = index;
			a.second = index + m - 1;
			list.push_back(a);
			cout << "pattern occurs at shift = " << index << " - " << index + m - 1 << endl;
			if (++cmp && index + m < n) //dich vi tri dang xet o txt len vi tri xuat hien thu 2 cua no trong pat neu khong vuot qua txt.size nguoc lai da het txt thoat chuong trinh 
			{
				index += m - badchar[txt[index + m]];
			}
			else return list;
			//s += (s + m < n)? m-badchar[txt[s + m]] : 1;

		}
		else //truong hop xuat hien ki tu khong match thi ta dich con tro dang xet cua txt toi tri xuat hien miss match
			index += max(1, j - badchar[txt[index + j]]);
	}
	return list;
}

void commandHandler(string mode, string inputPath, string outputPath, string outputInfo)
{
	vector<pair<int, int>> list_index;
	int count_cmp = 0;
	int time;
	//2 bien tren la output, truyen vao ham xu ly roi tra ve
	ifstream fin;
	fin.open(inputPath);
	if (!fin.is_open())
	{
		cout << "Unable to open file!!!\n";
		return;
	}

	string txt, pat;
	getline(fin, txt);
	getline(fin, pat);
	fin.close();

	clock_t start, end;
	start = clock();
	cout << start << endl;

	//mode case
	if (mode == "-bf")
	{
		list_index = bruteforce(txt, pat, count_cmp);
	}
	else if (mode == "-rk")
	{
		list_index = rabinkarp(txt, pat, count_cmp);
	}
	else if (mode == "-kmp")
	{
		list_index = kmp(txt, pat, count_cmp);
	}

	else if (mode == "-bm")
	{
		list_index = boyerm(txt, pat, count_cmp);
	}
	/*
	else if
	{

	}
	*/
	else
		return;
	//calculate time
	end = clock();
	cout << end << endl;
	time = end - start;
	cout << time << endl;
	//write file out
	ofstream fout;
	fout.open(outputPath);
	for (int i = 0; i < list_index.size(); i++)
		fout << list_index[i].first << ' ' << list_index[i].second << '\n';
	fout.close();

	//output case

	if (outputInfo == "-t" || outputInfo == "-ind")
	{
		cout << "Running time: " << time << "ms\n";
	}

	if (outputInfo == "-n" || outputInfo == "-ind")
	{
		cout << "Comparisons: " << count_cmp;
	}


}

int main(int argc, char* argv[]) {
	// Xu ly truong hop khong truyen vao du hoac thua tham so
	if (argc != 5)
		return 0;
	// Lay du lieu tu command line
	string mode = string(argv[1]);
	string inputPath = string(argv[2]);
	string outputPath = string(argv[3]);
	string outputInfo = string(argv[4]);

	//Goi ham xu ly lenh
	commandHandler(mode, inputPath, outputPath, outputInfo);

	return 0;
}