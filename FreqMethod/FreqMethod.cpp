// FreqMethod.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>

using namespace std;

/**
 * constants
 */

string const SOLS_FILE_NAME = "ouR.txt";
string const TEMP_FILE_NAME = "tmp.txt";

string const PRIOR_METH = "+[MAP]";
int pr = 0;
int M[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
int N = 4;

/**
* classes
*/

struct letter //letter with char and frequency
{
	letter(char const ch, bool const pr) : ch_(ch), pr_(pr) {}
	char get_ch() const { return ch_; }
	char get_freq() const { return freq_; }
	bool get_pr() const { return pr_; }
	void inc() { freq_++; }
private:
	char ch_;
	int freq_ = 1;
	bool pr_;
};

/**
* functions
*/

void read_frag(ifstream & fin, ofstream & fout, int & n_sols, vector<string> & sols) //reading all solutions from fragment, return № of priority method
{
	string stemp;
	int itemp, prior = 0;
	fin >> stemp; //skip _i_
	fout << stemp << endl;
	for (int i = 0; i < n_sols; i++)
	{
		fin >> stemp; //ABC
		fout << stemp << '\t';
		sols.push_back(stemp);
		
		fin >> itemp; //skip №
		fout << itemp << '\t';
		fin >> itemp; //skip №
		fout << itemp << '\t';
		fin >> stemp; //skip [***]
		if (stemp.find('+') != string::npos) fout << stemp << endl;
		else fout << ' ' << stemp << endl;
	}

}

void add_letter(vector<letter> & letters, char & ch, bool prior, string & solution)
{
	if (solution.find(ch) != string::npos) //if symbol is already used
	{
		return;
	}
	bool found = false;
	for (int i = 0; i < letters.size(); i++)
	{
		if (letters[i].get_ch() == ch)
		{
			letters[i].inc();
			found = true;
			break;
		}
	}
	if (!found) letters.push_back(letter(ch, prior));
}

char find_best_letter(vector<letter> & letters, string & candidates)
{
	char res_ch;
	if ( letters.size() == 0)
	{
		res_ch = candidates[0];
		candidates.erase(0, 1);
		return res_ch;
	}
	if (letters.size() == 1)
	{
		res_ch = letters[0].get_ch();
		if (auto const pos = candidates.find(res_ch) != string::npos) candidates.erase(pos, 1);
		return res_ch;
	}
	int maxi = 0, maxf = letters[maxi].get_freq();
	for (int i = 0; i < letters.size(); i++)
	{
		if (letters[i].get_freq() > maxf) { maxi = i; maxf = letters[i].get_freq(); }
		else if (letters[i].get_freq() == maxf && letters[i].get_pr() == true) { maxi = i; maxf = letters[i].get_freq(); }
		if (candidates.find(letters[i].get_ch()) == string::npos) candidates += letters[i].get_ch();
	}
	res_ch = letters[maxi].get_ch();
	candidates.erase(candidates.find(res_ch), 1);
	return res_ch;
}

string most_freq_sol(vector<string> & sols, int & n_sols, int const len_sols, int & prior) //computing solution with the most frequent characters
{
	string solution;
	string candidates;
	for (int i = 0; i < len_sols; ++i)
	{
		vector<letter> letters;

		for (int it_sol = 0; it_sol < n_sols; ++it_sol)
		{
			add_letter(letters, sols[it_sol][i], prior == it_sol, solution);
		}
		solution += find_best_letter(letters, candidates);
	}
	return solution;
}


void copy_file(ifstream &f, ofstream &t) 
{
		string const s((istreambuf_iterator<char>(f)),
			istreambuf_iterator<char>());
	t << s;
}

bool is_include(set<int> nums, int i) { return nums.count(i + 1) != 0; }

int main()
{
	setlocale(LC_ALL, "rus");

	set<int> nums;
	if (N == 0) { nums.insert(1); }
	for (int k = 0; k < N; k++) {
		nums.insert(M[k]);
	}

	ifstream fin(SOLS_FILE_NAME); //init ouR file
	if (!fin.is_open()) 
		return 1;
	
	ofstream fout(TEMP_FILE_NAME);

	int n_frags = 0, n_sols = 0;

	fin >> n_frags >> n_sols;
	for (int it_frags = 0; it_frags < n_frags; ++it_frags)
	{
		system("cls");
		cout << it_frags + 1 << "/" << n_frags;
		vector<string> sols;
		read_frag(fin, fout, n_sols, sols);
		
		int i = 0;
			for (auto it = sols.begin(); it != sols.end(); )
			{

				if (!is_include(nums, i))
					it = sols.erase(it);
				else
					++it;
				i++;
			}
		
		int size = sols.size();
		fout << most_freq_sol(sols, size, sols[0].length(), pr) << '\t' << n_sols+1 << '\t' << "0\t [FRQ]" << endl;
	}
	
	fout.close(); 
	fin.close();
	//rewrite

	ifstream fint(TEMP_FILE_NAME);
	if (!fint.is_open())
		return 1;
	ofstream foutt(SOLS_FILE_NAME);

	foutt << n_frags << " " << n_sols+1 << endl;
	copy_file(fint, foutt);

	foutt.close();
	fint.close();
	remove(TEMP_FILE_NAME.c_str());
	system("pause");
    return 0;
}

